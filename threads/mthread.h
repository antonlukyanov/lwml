// Поддержка многопоточности.
// Содержит классы, инкапсулирующие запуск потоков и их синхронизацию.
// lwml, (c) ltwood

#ifndef _MTHREAD_
#define _MTHREAD_

#include "lwml/base/defs.h"
#include "lwml/base/refcount.h"
#include <pthread.h>

/*#lake:stop*/

namespace lwml {

// Монопольный захват ресурса.

class locker : public value {
public:
  locker();
  ~locker();

  // Захватить ресурс
  void lock();

  // Попытаться захватить ресурс
  // Возвращает флаг удачи.
  bool test();

  // Освободить ресурс
  void unlock();

private:
  pthread_mutex_t _mutex;
};

// Класс rw_locker реализует защиту данных в модели
// "один писатель, много читателей".
// Писатели перед изменением данных вызывают метод write_wait(),
// который ожидает возможности записи.
// После изменения данных писатели вызывают метод write_done().
// Читатели в аналогичной ситуации для чтения используют методы
// read_wait() и read_done().

class rwlocker : public value {
public:
  rwlocker();
  ~rwlocker();

  void write_wait();
  void write_done();

  void read_wait();
  void read_done();

private:
  pthread_rwlock_t _rwlock;
};

// Класс event реализует событие поверх condition variable.
// На событие пробуждается ровно один ждущий его поток.
// Если ни один поток не ждет событие, то событие останется взведенным.

class event : public value {
public:
  event();
  ~event();

  void signal();
  void wait();

private:
  bool _event;
  pthread_mutex_t _mutex;
  pthread_cond_t _cond;
};

// Обертка для независимо выполняемого потока.
// Клиентский код предоставляет реализацию интерфейса i_thread_function,
// содержащего метод func(), код которого выполняется создаваемым потоком.

class i_thread_function : public interface, public refcount {
public:
  virtual void func() = 0;
};

class thread : public value {
public:
  thread( referer<i_thread_function> thr_func );
  ~thread();

  void wait();

private:
  referer<i_thread_function> _thr_func;
  pthread_t _thread;

  static void* code( void* arg );
};

}; // namespace lwml

#endif // _MTHREAD_
