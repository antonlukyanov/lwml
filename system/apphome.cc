#include "lwml/system/apphome.h"

#include "lwml/utils/debug.h"
#include "lwml/system/syssrv.h"
#include "lwml/io/filename.h"
#include "lwml/utils/llogsrv.h"

#if OS_WIN
  #include <sys/types.h>
  #include <windows.h>
#endif

#if OS_LINUX
  #include <unistd.h>
#elif OS_OSX
  #include <mach-o/dyld.h>
#endif

/*#lake:stop*/

namespace lwml {

namespace {
  const int APP_PATH_BUFLEN = 1024;

  void norm_path( char* buf )
  {
    for( char* pch = buf; *pch; pch++ ){
      if( *pch == '\\' )
        *pch = '/';
    }
  }

  void remove_last_part(char* s)
  {
    char* psl = strrchr(s, '/');
    if( psl != 0 )
      *psl = 0;
  }

  cstrng get_app_path() //!! TODO: вычислять строку один раз в начале?
  {
    if( getenv("LWML_APP_HOME") ){
      return filename::norm_path(getenv("LWML_APP_HOME"));
    } else {
      // Локальный буфер потоко-безопасней.
      char buf[APP_PATH_BUFLEN];

#if OS_WIN
      int res = GetModuleFileName(0, buf, APP_PATH_BUFLEN);
      if( res == 0 || res == APP_PATH_BUFLEN )
        fail_syscall("win32::GetModuleFileName()");
#elif OS_OSX
      uint32_t size = APP_PATH_BUFLEN;
      if( _NSGetExecutablePath(buf, &size) != 0 ){
        fail_syscall("osx::_NSGetExecutablePath()");
      }
#else
      const char* link_name = "/proc/self/exe";
      const int ret = static_cast<int>(readlink(link_name, buf, APP_PATH_BUFLEN - 1));

      if( ret == -1 ){
        fail_syscall("linux::readlink()");
      }

      buf[ret] = 0;
#endif

      norm_path(buf);

#if OS_OSX
      static char out[APP_PATH_BUFLEN];
      realpath(buf, out);
      strcpy(buf, out);
#endif

      // Убираем имя исполнимого файла.
      remove_last_part(buf);

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
