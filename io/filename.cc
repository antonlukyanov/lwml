#include "filename.h"

#include "stdmem.h"
#include "alphabet.h"
#include "lwmlconf.h"

#include <io.h>
#include <sys/stat.h>

#ifdef __BORLANDC__
#include <dir.h>
#endif

#ifdef _MSC_VER
#include <direct.h>
#endif

/*#lake:stop*/

namespace lwml {

void filename::norm_path_x( char* buf )
{
  for( char* p = buf; *p; p++ )
    if( *p == '\\' ) *p = '/';
}

void filename::parse( const char* fullname )
{
  int buflen = strlen(fullname)+1;
  char* fn = raw_alloc<char>(buflen);
  prot_strcpy(fn, fullname, buflen); // перестраховка
  norm_path_x(fn);

  char* pbs = strrchr(fn, '/');   // самый правый слэш
  char* pnm = pbs ? pbs+1 : fn;   // имя файла
  char* pdt = strrchr(fn, '.');   // самая правая точка

  _path.set_zero();
  if( pbs ){
    *pbs = 0;
    _path = fn;
    _path.concat("/");
  }

  _extn.set_zero();
  if( pdt ) _extn = pdt;

  _name.set_zero();
  if( pdt ) *pdt = 0;
  _name = pnm;

  mem::free(fn);
}

filename::filename( const char* fnm )
{
  parse(fnm);
}

strng filename::pn() const
{
  strng res = _path;
  res.concat(_name);
  return res;
}

strng filename::pne() const
{
  strng res = _path;
  res.concat(_name);
  res.concat(_extn);
  return res;
}

strng filename::ne() const
{
  strng res = _name;
  res.concat(_extn);
  return res;
}

strng filename::chext( const char* nm, const char* ext )
{
  filename fn(nm);
  fn.set_extn(ext);
  return fn.pne();
}

strng filename::chrelname( const char* nm, const char* nnm )
{
  filename fn(nm);
  filename nfn(nnm);
  fn.set_name(nfn.name().ascstr());
  fn.set_extn(nfn.extn().ascstr());
  return fn.pne();
}

strng filename::index( const char* nm, int idx, int width )
{
  filename fn(nm);
  fn.set_name(strng::form("%s-%0*d", fn.name().ascstr(), width, idx).ascstr());
  return fn.pne();
}

strng filename::suffix( const char* nm, const char* suff )
{
  filename fn(nm);
  fn.set_name(strng::form("%s-%s", fn.name().ascstr(), suff).ascstr());
  return fn.pne();
}

bool filename::is_exists( const char* fname )
{
  return access(denorm(fname).ascstr(), 0) == 0;
}

strng filename::denorm( const char* fname )
{
  strng fn(fname);
  int pos = fn.len()-1;
  if( fn.at(pos) == '/' || fn.at(pos) == '\\' )
    fn.setchar(pos, 0);
  return fn;
}

bool filename::is_dir( const char *fname )
{
  if( !is_exists(fname) )
    throw ex_nosuchfile("%s", fname);
  struct stat sst;
  if( ::stat(denorm(fname).ascstr(), &sst) != 0 )
    fail_syscall("libc::stat()");
  return (S_IFDIR & sst.st_mode) != 0;
}

//!! TODO: проверить корректность вызова fail_syscall()
void filename::remove( const char* fname )
{
  if( !is_exists(fname) ) return;
  if( is_dir(fname) ){
#if defined(__BORLANDC__) || defined(_MSC_VER)
    if( ::_rmdir(denorm(fname).ascstr()) != 0 )
#else
    if( ::rmdir(denorm(fname).ascstr()) != 0 )
#endif
      fail_syscall("libc::rmdir()");
  }else{
    if( ::chmod(denorm(fname).ascstr(), S_IWRITE) != 0 )
      fail_syscall("libc::chmod()");
    if( ::remove(denorm(fname).ascstr()) != 0 )
      fail_syscall("libc::remove()");
  }
}

void filename::rename( const char* fname, const char* new_fname )
{
  if( !is_exists(fname) )
    throw ex_nosuchfile("%s", fname);
  if( is_exists(new_fname) )
    remove(new_fname);
  if( ::rename(denorm(fname).ascstr(), denorm(new_fname).ascstr()) != 0 )
    fail_syscall("libc::rename()");
}

time_t filename::mod_time( const char* fname )
{
  if( !is_exists(fname) )
    throw ex_nosuchfile("%s", fname);
  struct stat st;
  if( ::stat(denorm(fname).ascstr(), &st) != 0 )
    fail_syscall("libc::stat()");
  return st.st_mtime;
}

bool filename::is_exists()
{
  return is_exists(pne().ascstr());
}

bool filename::is_dir()
{
  return is_dir(pne().ascstr());
}

void filename::remove()
{
  remove(pne().ascstr());
}

void filename::rename( const char* new_fname )
{
  rename(pne().ascstr(), new_fname);
}

time_t filename::mod_time()
{
  return mod_time(pne().ascstr());
}

strng filename::norm_path( const char* path, delimiter delim, case_sensitivity cs )
{
  char slash = '/';
  if( delim == WIN_DELIM )
    slash = '\\';
  strng res(path);
  for( int j = 0; j < res.len(); j++ )
    if( res[j] == '\\' || res[j] == '/' )
      res.setchar(j, slash);
  if( res.len() != 0 && res.at(res.len()-1) != slash )
    res.concat(slash);
  return (cs == CASE_SENS) ? res : res.tolower(_FILENAME_CODEPAGE_);
}

// Размер буфера под текущий путь.
// Размер указан для полного размера буфера, включая завершающий нуль.

const int CWD_BUFLEN = 1024;

strng filename::current_path()
{
  char buf[CWD_BUFLEN];

#if defined(_MSC_VER)
  if( _getcwd(buf, CWD_BUFLEN) == 0 ){
#else
  if( getcwd(buf, CWD_BUFLEN) == 0 ){
#endif
    if( errno == ERANGE )
      fail_assert("too long directory name");
    else
      fail_syscall("libc::getcwd()");
  }
  return strng(buf);
}

}; // namespace lwml
