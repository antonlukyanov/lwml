#include "apphome.h"

#include "debug.h"
#include "syssrv.h"
#include "filename.h"
#include "llogsrv.h"

#include <windows.h>

/*#lake:stop*/

namespace lwml {

namespace {
  const int APP_PATH_BUFLEN = 1024;

  cstrng get_app_path() //!! TODO: вычислять строку один раз в начале?
  {
    if( getenv("LWML_APP_HOME") ){
      return filename::norm_path(getenv("LWML_APP_HOME"));
    } else {
      char buf[APP_PATH_BUFLEN]; // локальный буфер потоко-безопасней

      int res = GetModuleFileName(0, buf, APP_PATH_BUFLEN);
      if( res == 0 || res == APP_PATH_BUFLEN )
        fail_syscall("win32::GetModuleFileName()");

      for( char* pch = buf; *pch; pch++ ){
        if( *pch == '\\' )
          *pch = '/';
      }
      char * psl = strrchr(buf, '/');
      if( psl == 0 )
        return cstrng();
      *psl = 0;

      return cstrng(buf);
    }
  }
};

strng apphome::mk_fname( const char* fnm )
{
  return get_app_path().concat('/').concat(fnm);
}

strng apphome::mk_dump_v( const char* fmt, va_list va )
{
  strbuf<512> fnm;
  fnm.vprintf(fmt, va);

  strng fmt_nm = strng::form("_%08x_%04u__%s", llogsrv::get_id(), llogsrv::get_ct(), fnm.get());
  zzz_ex("lwml:dump", "dumping <%s>", fmt_nm.ascstr());

  strbuf<512> buf;
  buf.printf("%s/dump/%s", get_app_path().ascstr(), fmt_nm.ascstr());
  return strng(buf.get());
}

strng apphome::mk_dump( const char* fmt, ... )
{
  va_list va;
  va_start(va, fmt);
  strng res;
  try {
    res = mk_dump_v(fmt, va);
  } catch( ... ){
    va_end(va);
    throw;
  }
  va_end(va);
  return res;
}

}; // namespace lwml
