#include "lwml/utils/debug.h"

#include "lwml/base/basex.h"
#include "lwml/utils/llogsrv.h"
#include "lwml/system/syssrv.h"
#include "lwml/system/apphome.h"

/*#lake:stop*/

namespace lwml {

// zzz_ctx

zzz_ctx_holder::zzz_ctx_holder( const char* ctx )
{
  zzz_ex(">>>", "%s", ctx);
}

zzz_ctx_holder::~zzz_ctx_holder()
{
  zzz_ex("<<<", "%s", "");
}

// log

void zzz_ex_v( const char* asp, const char* msg, va_list va )
{
  strbuf<1024> buf;
  buf.vprintf(msg, va);
  llogsrv::out_log(asp, buf.get());
}

void zzz_ex( const char* asp, const char* msg, ... )
{
  va_list va;
  va_start(va, msg);
  zzz_ex_v(asp, msg, va);
  va_end(va);
}

void zzz( const char *fmt, ... )
{
  va_list va;
  va_start(va, fmt);
  zzz_ex_v("", fmt, va);
  va_end(va);
}

// dump

bool zzz_dump()
{
  return llogsrv::is_dump();
}

strng zzz_dump_name( const char* fmt, ... )
{
  va_list va;
  va_start(va, fmt);
  strng res;
  try {
    res = apphome::mk_dump_v(fmt, va);
  } catch( ... ) {
    va_end(va);
    throw;
  }
  va_end(va);

  return res;
}

// Реализация потока вывода поверх потока отладочного вывода

void zstream::out( const strng& s )
{
  if( s.search('\n') == -1 )
    llogsrv::out_log(_asp.ascstr(), s.ascstr());
  else {
    cstrng ss(s);
    do {
      llogsrv::out_log(_asp.ascstr(), ss.get_word("\n").ascstr());
    }while( !ss.is_empty() );
  }
}

// system log service

void zzz_sys_v( const char* msg, va_list va )
{
  strbuf<1024> buf;
  buf.vprintf(msg, va);
  syssrv::debug_message(buf.get());
}

void zzz_sys( const char *fmt, ... )
{
  va_list va;
  va_start(va, fmt);
  zzz_sys_v(fmt, va);
  va_end(va);
}

}; // namespace lwml
