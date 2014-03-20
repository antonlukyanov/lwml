// ¬ысокоуровневые примитивы, св€занные с многопоточностью.
// lwml, (c) ltwood

#ifndef _MTASK_
#define _MTASK_

#include "defs.h"
#include "mdefs.h"

#include "mthread.h"

/*#build_stop*/

namespace lwml {

// RAII-обертка дл€ класса locker

class stack_locker : public value {
public:
  stack_locker( locker* loc ) : _loc(loc) {
    _loc->lock();
  }

  ~stack_locker(){
    _loc->unlock();
  }

private:
  locker* _loc;
};

}; // namespace lwml

#endif // _MTASK_
