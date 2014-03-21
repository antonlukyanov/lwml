// Высокоуровневые примитивы, связанные с многопоточностью.
// lwml, (c) ltwood

#ifndef _MTASK_
#define _MTASK_

#include "defs.h"
#include "stdmem.h"
#include "refcount.h"
#include "refowner.h"
#include "t_array.h"

#include "mthread.h"

/*#build_stop*/

namespace lwml {

// RAII-обертка для класса locker

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

// Связка потоков для распараллеливания набора однородных задач.
//
// Предполагается, что есть N одинаковых задач, которые можно
// выполнять независимо и M потоков, в которых можно их выполнять.
// В каждом потоке выполняется примерно N/M задач, причем
// задачи представлены реализацией интерфейса i_tbtask, которая
// содержит все данные, необходимые для решения задачи.
// Собственно решение j-й задачи происходит при выполнении метода func(),
// которому в качестве аргумента передается номер решаемой задачи.

class i_tbtask : public interface, public refcount {
public:
  virtual void func( int idx ) = 0;
};

class tb_thread_func : public i_thread_function {
  tb_thread_func() : _do_stop(false) {}

public:
  static referer<tb_thread_func> create(){
    return referer<tb_thread_func>(new(lwml_alloc) tb_thread_func());
  }

  virtual void func();

  void start( referer<i_tbtask> task, int first, int num );
  void wait();
  void stop();

private:
  referer<i_tbtask> _task;
  int _first, _num;
  bool _do_stop;
  event _wakeup, _is_ready;
};

class thread_bundle {
public:
  thread_bundle( int th_num );
  ~thread_bundle();

  void calc( referer<i_tbtask> task, int num );

private:
  int _th_num;

  t_array< referer<tb_thread_func> > _tfunc;
  t_array< refowner<thread> > _thr;
};

}; // namespace lwml

#endif // _MTASK_
