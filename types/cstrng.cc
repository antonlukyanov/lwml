#include "lwml/types/cstrng.h"
#include "lwml/memory/stdmem.h"
#include "lwml/system/alphabet.h"

/*#lake:stop*/

namespace lwml {

// Размеры буферов для вывода значений специфических типов
// Во всех случаях указан размер полного буфера, включая место под завершающий нуль.

const int NUM_BUFSZ  = 64;
const int TIME_BUFSZ = 64;

void cstrng::alloc( int sz )
{
  _size = sz;
  _data = raw_alloc<char>(_size+1);
}

void cstrng::realloc( int sz )
{
  _size = sz;
  _data = raw_realloc<char>(_data, _size+1);
}

void cstrng::dealloc()
{
  mem::free(_data);
}

cstrng& cstrng::_assign( const char* src, int slen )
{
  if( _size < slen )
    realloc(slen);
  prot_strcpy(_data, src, _size+1); // перестраховка
  return *this;
}

cstrng& cstrng::_add( char ch )
{
  int ln = len();
  if( ln + 1 > size() )
    realloc(size() + CSTRNG_STEP);
  _data[ln++] = ch;
  _data[ln] = 0;
  return *this;
}

cstrng& cstrng::_add( const char* src, int slen )
{
  int ln = len();
  if( ln + slen > size() )
    realloc(ln + slen);
  strcat(_data, src);
  return *this;
}

int cstrng::_cmp( const char* src ) const
{
  return strcmp(_data, src);
}

cstrng::cstrng( int sz )
{
  alloc(sz);
  set_zero();
}

cstrng::cstrng( const char* src )
{
  alloc(strlen(src));
  prot_strcpy(_data, src, _size+1); // перестраховка
}

cstrng::cstrng( const cstrng& src )
{
  alloc(src.len());       // отводим память только под реальное содержимое
  prot_strcpy(_data, src._data, _size+1); // перестраховка
}

cstrng::~cstrng()
{
  dealloc();
}

bool cstrng::str2int_ex( int* res, const char* str )
{
  return (sscanf(str, "%d", res) == 1);
}

bool cstrng::str2real_ex( real* res, const char* str )
{
  return (sscanf(str, "%lf", res) == 1);
}

int cstrng::str2int( const char* str )
{
  int buf;
  if( !str2int_ex(&buf, str) )
    throw ex_convert("<%s> as int", str);
  return buf;
}

real cstrng::str2real( const char* str )
{
  double buf;
  if( !str2real_ex(&buf, str) )
    throw ex_convert("<%s> as real", str);
  return buf;
}

uint cstrng::str2hash( const char *str )
{
  uint res = 0;
  while( *str )
    res = (res << 1) ^ (*str++);
  return res;
}

cstrng cstrng::int2str( int x )
{
  char buf[NUM_BUFSZ];
  prot_sprintf(buf, NUM_BUFSZ, "%d", x);
  return strng(buf);
}

cstrng cstrng::uint2str( uint x )
{
  char buf[NUM_BUFSZ];
  prot_sprintf(buf, NUM_BUFSZ, "%u", x);
  return strng(buf);
}

cstrng cstrng::hex2str( uint x )
{
  char buf[NUM_BUFSZ];
  prot_sprintf(buf, NUM_BUFSZ, "%0x", x);
  return strng(buf);
}

cstrng cstrng::real2str( double x, int prec, real_format nf )
{
  char buf[NUM_BUFSZ];
  if( nf == nfSCNT )
    prot_sprintf(buf, NUM_BUFSZ, "%.*e", prec, x);
  else
    prot_sprintf(buf, NUM_BUFSZ, "%.*f", prec, x);
  return strng(buf);
}

cstrng cstrng::time2str( float sec )
{
  char buf[TIME_BUFSZ];
  int hour = int_cast(sec / 3600.0);
  sec -= hour * 3600;
  int min = int_cast(sec / 60.0);
  sec -= min * 60;
  prot_sprintf(buf, TIME_BUFSZ, "%d:%d:%.2f", hour, min, sec);
  return cstrng(buf);
}

cstrng cstrng::vform( const char *fmt, va_list va )
{
  char* buf = mem::vform(fmt, va);
  cstrng res(buf);
  raw_free<char>(buf);
  return res;
}

cstrng cstrng::form( const char *fmt, ... )
{
  va_list va;
  va_start(va, fmt);
  strng res;
  try {
    res = vform(fmt, va);
  }catch( ex_base& ){
    va_end(va);
    throw;
  }
  va_end(va);
  return res;
}

void cstrng::_shl( int pos )
{
  if( pos == 0 ) return; // not needed, only for speed
  int j;
  for( j = 0; _data[j + pos]; j++ )
    _data[j] = _data[pos + j];
  _data[j] = _data[pos + j]; // copy last NUL
}

cstrng& cstrng::lstrip()
{
  int j;
  for( j = 0; _data[j] && isspace(_data[j]); j++ );
  _shl(j);
  return *this;
}

cstrng& cstrng::rstrip()
{
  int ln = len();
  if( ln == 0 ) return *this; // essential for the next line
  char* plast = _data + ln - 1;
  while( plast >= _data && isspace( *plast ) )
    *(plast--) = 0;
  return *this;
}

cstrng& cstrng::strip()
{
  lstrip();
  rstrip();
  return *this;
}

cstrng& cstrng::strip_quot( char quot )
{
  strip();
  int ln = len();
  if( ln < 2 ) return *this;

  if( _data[0] == quot && _data[ln-1] == quot ){
    _data[ln-1] = 0;
    _shl(1);
  }

  return *this;
}

cstrng& cstrng::tolower( int cp )
{
  for( int j = 0; j < len(); j++ )
    _data[j] = alphabet::tolower(_data[j], cp);
  return *this;
}

cstrng& cstrng::toupper( int cp )
{
  for( int j = 0; j < len(); j++ )
    _data[j] = alphabet::toupper(_data[j], cp);
  return *this;
}

cstrng cstrng::get_word( const char* delim )
{
  int j = 0;
  while( _data[j] != 0 && strchr(delim, _data[j]) == 0 ) j++;
  cstrng res;
  if( _data[j] != 0 ){
    _data[j] = 0;
    res = _data;
    _shl(j+1);
  }else{
    res = _data;
    set_zero();
  }
  return res;
}

int cstrng::search( char ch ) const
{
  char* ptr = strchr(_data, ch);
  if( ptr == 0 )
    return -1;
  else
    return ptr - _data;
}

int cstrng::revsearch( char ch ) const
{
  char* ptr = strrchr(_data, ch);
  if( ptr == 0 )
    return -1;
  else
    return ptr - _data;
}

int cstrng::search( const char* str ) const
{
  char* ptr = strstr(_data, str);
  if( ptr == 0 )
    return -1;
  else
    return ptr - _data;
}

cstrng cstrng::subs( int pos, int len ) const
{
  int slen = this->len();
  test_index(pos, slen+1); // позволяем адресовать завершающий нуль

  cstrng res(_data + pos);
  if( res.len() > len )
    res.setchar(len, 0);
  return res;
}

}; // namespace lwml
