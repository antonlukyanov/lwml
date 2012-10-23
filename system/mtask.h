// Высокоуровневые примитивы, связанные с многопоточностью.
// lwml, (c) ltwood

#ifndef _MTASK_
#define _MTASK_

#include "defs.h"
#include "mdefs.h"

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

// Класс swmr_locker реализует защиту данных в модели
// "один писатель, много читателей".
// Писатели перед изменением данных вызывают метод write_wait(),
// который ожидает возможности записи.
// После изменения данных писатели вызывают метод write_done().
// Читатели в аналогичной ситуации для чтения используют методы
// read_wait() и read_done().

class swmr_locker : public value {
public:
  swmr_locker();

  void write_wait();
  void write_done();

  void read_wait();
  void read_done();

private:
  locker _no_writer;
  state _no_readers;
  int _readers_counter;
};

}; // namespace lwml

#endif // _MTASK_
