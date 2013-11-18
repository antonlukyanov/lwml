#include "stream.h"
#include "stdmem.h"
#include "basex.h"
#include "debug.h"

#include <fcntl.h>

/*#lake:stop*/

namespace lwml {

strng stream::mkmode( int mode ) const
{
  strng sm;
  sm.set_zero();
  switch( mode & 0x000F ){
    case fmREAD:
      sm.concat('r');
      break;
    case fmWRITE:
      sm.concat('w');
      break;
    case fmAPPEND:
      sm.concat('a');
      break;
    default:
      fail_assert("incorrect read/write mode");
  }
  if( mode & 0x0010 )
    sm.concat('+');
  switch( mode & 0x0F00 ){
    case fmTEXT:
      sm.concat('t');
      break;
    case fmBINARY:
      sm.concat('b');
      break;
    default:
      fail_assert("incorrect text/binary mode");
  }
  return sm;
}

stream::stream()
{
  _file = tmpfile();
  _is_text = false;
  if( _file == NULL )
    fail_syscall("libc::tmpfile()");
}

stream::stream( const char* name, int wrmode, int btmode )
{
  strng mode = mkmode(wrmode | btmode);
  _file = fopen(name, mode.ascstr());
  _is_text = (btmode == fmTEXT);
  zzz_ex("lwml:io", "open <%s> (%s) %s", name, mode.ascstr(), _file ? "ok" : "fail");
  if( _file == 0 )
    throw ex_fopen("%s", name);
}

stream::~stream()
{
  fclose(_file);
}

int stream::id()
{
  return fileno(_file);
}

int stream::size()
{
  int saved_pos = ftell(_file);
  fseek(_file, 0, SEEK_END);
  int res = ftell(_file);
  fseek(_file, saved_pos, SEEK_SET);
  return res;
  // return filelength(fileno(_file));
}

int stream::tell()
{
  return ftell(_file);
}

void stream::rewind()
{
  ::rewind(_file);
}

void stream::seek( int pos )
{
  if( fseek(_file, pos, SEEK_SET) != 0 )
    fail_syscall("libc::fseek()");
}

void stream::skip( int ofs )
{
  if( fseek(_file, ofs, SEEK_CUR) != 0 )
    fail_syscall("libc::fseek()");
}

void stream::flush()
{
  fflush(_file);
}

void stream::read( void *ptr, int len )
{
  if( _is_text )
    fail_assert("can't read binary block from text file, sorry...");
  if( fread(ptr, sizeof(char), len, file()) != static_cast<size_t>(len) )
    throw ex_fread();
}

void stream::write( const void *ptr, int len )
{
  if( _is_text )
    fail_assert("can't write binary block into text file, sorry...");
  if( fwrite(ptr, sizeof(char), len, file()) != static_cast<size_t>(len) )
    fail_syscall("libc::fwrite()");
}

void stream::printf( const char* fmt, ... )
{
  if( !_is_text )
    fail_assert("can't printf() into binary stream, sorry...");
  va_list va;
  va_start(va, fmt);
  vfprintf(file(), fmt, va);
  va_end(va);
}

void stream::put_str( const char* str )
{
  if( _is_text )
    fail_assert("can't put_str() into text stream, sorry...");
  else
    write(str, strlen(str) + 1);
}

bool stream::get_str( cstrng& str )
{
  char lim = _is_text ? 0x0a : 0;
  int len = 0;
  str.set_zero();
  while( 1 ){
    int ch = fgetc(file());
    if( ch == EOF ) break;
    len++;
    if( ch == lim ) break;
    str.concat(static_cast<char>(ch));
  }
  return len != 0;
}

int stream::count_lines()
{
  if( !_is_text )
    fail_assert("can't run count_lines() for binary stream, sorry...");
  int saved_pos = ftell(file());
  strng buf;
  int res;
  for( res = 0; get_str(buf); res++ ) (void)0;
  fseek(file(), saved_pos, SEEK_SET);
  return res;
}

}; // namespace lwml
