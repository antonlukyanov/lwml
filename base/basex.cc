#include "lwml/base/basex.h"

#include "lwml/utils/llogsrv.h"
#include "lwml/system/syssrv.h"

/*#lake:stop*/

namespace lwml {

#if _USE_WIN32API_
namespace {
  const char runtime_msg[] =
    "Application has generated runtime exception:\n" \
    "\n" \
    "  message: <%s>\n" \
    "\n" \
    "Now you may activate GDB as a JIT debugger:\n"
    "\n" \
    "  gdb executable-name %lu\n"
    "\n" \
    "Do you wish to invoke GDB using JIT exception?";
  const int EX_MSG_BUFLEN = sizeof(runtime_msg) + EX_BUFLEN + 256;
};
#endif

void i_exception::raise_jit( const char* msg )
{
#if _USE_WIN32API_
  if( llogsrv::is_jit() ){
    strbuf<EX_MSG_BUFLEN> buf;
    buf.printf(runtime_msg, msg, syssrv::get_pid());
    bool is_jit = syssrv::ask("Runtime Exception", buf.get());
    if( is_jit )
      syssrv::call_jit();
  }
#endif
}

// runtime_location

runtime_location::runtime_location( const char* fname, const char* func, int line )
{
  const char* psl = strrchr(fname, '/');
  const char* prsl = strrchr(fname, '\\');
  const char* fn = psl ? psl + 1 : (prsl ? prsl + 1 : fname);
  _fname.put(fn);
  _func.put(func);
  _line = line;
}

void runtime_location::operator()( const char* fmt, ... )
{
  strbuf<EX_BUFLEN> msg;

  va_list va;
  va_start(va, fmt);
  msg.vprintf(fmt, va);
  va_end(va);

  throw ex_runtime("%s:%s:%d: %s", _fname.get(), _func.get(), _line, msg.get());
}

}; // namespace lwml
