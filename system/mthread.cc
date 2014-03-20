#include "mthread.h"
#include "basex.h"
#include "stdmem.h"

namespace lwml {

/*#lake:stop*/

// thread

void* thread::code( void* arg )
{
  reinterpret_cast<thread*>(arg)->_thr_func->func();
  return NULL;
}

}; // namespace lwml
