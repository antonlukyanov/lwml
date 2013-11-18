#include "dload.h"

#include "debug.h"
#include "cstrng.h"

#include <windows.h>

/*#lake:stop*/

namespace lwml {

void* dl_load( const char* dl_name )
{
  void* res = LoadLibrary(dl_name);
  zzz_ex("lwml:dload", "loading <%s> %s", dl_name, res ? "ok" : "fail");
  return res;
}

dproc dl_proc( void* dll, const char* fn )
{
  if( !dll )
    fail_assert("dll is not loaded");
  HINSTANCE hinst = reinterpret_cast<HINSTANCE>(dll);
  dproc res = reinterpret_cast<dproc>(GetProcAddress(hinst, fn));
  if( !res )
    throw ex_dll("%s", fn);
  return res;
}

}; // namespace lwml
