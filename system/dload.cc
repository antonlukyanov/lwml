#include "dload.h"

#include "debug.h"
#include "cstrng.h"

#ifdef _WIN32
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif

/*#lake:stop*/

namespace lwml {

void* dl_load( const char* dl_name )
{
  void* res;

#ifdef _WIN32
  res = LoadLibrary(dl_name);
#else
  res = dlopen(dl_name, RTLD_LAZY);
#endif

  zzz_ex("lwml:dload", "loading <%s> %s", dl_name, res ? "ok" : "fail");
  return res;
}

dproc dl_proc( void* dll, const char* fn )
{
  if( !dll )
    fail_assert("dll is not loaded");

  dproc res;

#ifdef _WIN32
  HINSTANCE hinst = reinterpret_cast<HINSTANCE>(dll);
  res = reinterpret_cast<dproc>(GetProcAddress(hinst, fn));
#else
  res = reinterpret_cast<dproc>(dlsym(dll, fn));
#endif

  if( !res )
    throw ex_dll("%s", fn);

  return res;
}

}; // namespace lwml
