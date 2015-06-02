#include "stdmem.h"

#include "lwmlconf.h"
#include "debug.h"

/*#lake:stop*/

namespace lwml {

/*
  if( mem == 0 ){
    if( size == 0 )
      return 0;
    else
      ptr = ::malloc(size);
  }else{
    if( size == 0 ){
      ::free(mem);
      return 0;
    }else
      ptr = ::realloc(mem, size);
  }
*/

void* mem::alloc( int size )
{
  void* ptr = ::malloc(size);
  if( size != 0 && ptr == 0 )
    throw ex_mem();
#if _LOG_MEM_
  zzz_ex("lwml:mem", "alloc %p %d", ptr, size);
#endif
  return ptr;
}

void mem::free( void* ptr )
{
  ::free(ptr);
#if _LOG_MEM_
  if( ptr )
    zzz_ex("lwml:mem", "free %p", ptr);
#endif
}

void* mem::realloc( void* mem, int size )
{
  void* ptr = ::realloc(mem, size);
  if( size != 0 && ptr == 0 )
    throw ex_mem();
#if _LOG_MEM_
  zzz_ex("lwml:mem", "realloc %p %p %d", mem, ptr, size);
#endif
  return ptr;
}

char* mem::strdup( const char *src )
{
  int buflen = strlen(src) + 1;
  //!! SIC: здесь вызывается mem::alloc(), поэтому вывод лога не выполняется
  char *res = raw_alloc<char>(buflen);
  prot_strcpy(res, src, buflen); // перестраховка
  return res;
}

const int FORM_STRLEN = 32;

char* mem::vform( const char* fmt, va_list va )
{
  int size = FORM_STRLEN;
  char *buf = raw_alloc<char>(size);

  while( 1 ){
    int nchars;

#if defined(_MSC_VER)
  nchars = _vsnprintf(buf, size, fmt, va);
#else
  #if OS_OSX
    va_list va_fmt;
    va_copy(va_fmt, va);
    nchars = vsnprintf(buf, size, fmt, va_fmt);
  #else
    nchars = vsnprintf(buf, size, fmt, va);
  #endif
#endif

    if( nchars >= 0 && nchars < size )
      return buf;
    size = (size * 3) / 2;
    buf = raw_realloc<char>(buf, size);
  }
}

char* mem::form( const char* fmt, ... )
{
  char* res;
  va_list va;
  va_start(va, fmt);
  try{
    res = vform(fmt, va);
  }catch( ex_base& ){
    va_end(va);
    throw;
  }
  va_end(va);
  return res;
}

}; // namespace lwml

const lwml_alloc_t lwml_alloc = {};
