#include "syssrv.h"

#include "lwmlconf.h"

/*#lake:stop*/

#if _USE_WIN32API_
  #include <windows.h>
#endif

namespace lwml {

void syssrv::message( const char *title, const char *msg ){
#if _USE_WIN32API_
  MessageBox(0, msg, title, MB_SYSTEMMODAL | MB_OK | MB_ICONHAND);
#else
  exit("Can't show message in current configuration");
#endif
}

int syssrv::ask( const char *title, const char *msg )
{
#if _USE_WIN32API_
  return MessageBox(0, msg, title, MB_SYSTEMMODAL | MB_YESNO | MB_ICONQUESTION) == IDYES;
#else
  exit("Can't ask user in current configuration");
#endif
  return 0;
}

uint32 syssrv::get_pid()
{
#if _USE_WIN32API_
  return GetCurrentProcessId();
#else
  exit("Can't get PID in current configuration");
#endif
  return 0;
}

uint32 syssrv::get_tid()
{
#if _USE_WIN32API_
  return GetCurrentThreadId();
#else
  exit("Can't get TID in current configuration");
#endif
  return 0;
}

void syssrv::call_jit()
{
#if _USE_WIN32API_
  DebugBreak();
#endif
}

void syssrv::debug_message( const char* msg )
{
#if _USE_WIN32API_
  OutputDebugString(msg);
#endif
}

void syssrv::exit( const char* msg )
{
  printf("%s\n", msg);
  ::exit(EXIT_FAILURE);
}

}; // namespace lwml
