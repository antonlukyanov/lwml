#include "lwml/system/dload.h"

#include "lwml/utils/debug.h"
#include "lwml/types/cstrng.h"

#if OS_WIN
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif

/*#lake:stop*/

namespace lwml {

void* dl_load( const char* dl_name )
{
  void* res;

#if OS_WIN
  res = LoadLibrary(dl_name);
#else
  res = dlopen(dl_name, RTLD_LAZY);
#endif

  zzz_ex("lwml:dload", "loading <%s> %s", dl_name, res ? "ok" : "fail");
  return res;
}

void* dl_load_nozzz( const char* dl_name )
{
  void* res;

#if OS_WIN
  res = LoadLibrary(dl_name);
#else
  res = dlopen(dl_name, RTLD_LAZY);
#endif

  return res;
}

dproc dl_proc( void* dll, const char* fn )
{
  if( !dll )
    fail_assert("dll is not loaded");

  dproc res;

#if OS_WIN
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
