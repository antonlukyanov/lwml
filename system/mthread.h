// Поддержка многопоточности.
// Содержит классы, инкапсулирующие запуск потоков и их синхронизацию.
// lwml, (c) ltwood

#ifndef _MTHREAD_
#define _MTHREAD_

#include "defs.h"
#include "refcount.h"

#include <pthread.h>

/*#lake:stop*/

namespace lwml {

// Монопольный захват ресурса.

class locker : public value {
public:
  locker() {
    pthread_mutex_init(&_mutex, NULL);
  }
  ~locker() {
    pthread_mutex_destroy(&_mutex);
  }

  // Захватить ресурс
  void lock() {
    printf("lock\n"); fflush(stdout);
    pthread_mutex_lock(&_mutex);
  }

  // Попытаться захватить ресурс
  // Возвращает флаг удачи.
  bool test() { return pthread_mutex_trylock(&_mutex) == 0; }

  // Освободить ресурс
  void unlock() {
    printf("unlock\n"); fflush(stdout);
    pthread_mutex_unlock(&_mutex);
  }

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
  rwlocker() { pthread_rwlock_init(&_rwlock, NULL); }
  ~rwlocker() { pthread_rwlock_destroy(&_rwlock); }

  void write_wait() { pthread_rwlock_wrlock(&_rwlock); }
  void write_done() { pthread_rwlock_unlock(&_rwlock); }

  void read_wait() { pthread_rwlock_rdlock(&_rwlock); }
  void read_done() { pthread_rwlock_unlock(&_rwlock); }

private:
  pthread_rwlock_t _rwlock;
};

// Класс event реализует событие поверх condition variable.
// На событие пробуждается ровно один ждущий его поток.
// Если ни один поток не ждет событие, то событие останется взведенным.

class event : public value {
public:
  event(){
    _event = false;
    pthread_mutex_init(&_mutex, NULL);
    pthread_cond_init(&_cond, NULL);
  }

  ~event(){
    pthread_cond_destroy(&_cond);
    pthread_mutex_destroy(&_mutex);
  }

  void signal() {
    pthread_mutex_lock(&_mutex);
    _event = true;
    pthread_mutex_unlock(&_mutex);
    pthread_cond_signal(&_cond);
  }

  void wait(){
    pthread_mutex_lock(&_mutex);
    while( !_event )
      pthread_cond_wait(&_cond, &_mutex);
    _event = false;
    pthread_mutex_unlock(&_mutex);
  }

private:
  bool _event;
  pthread_mutex_t _mutex;
  pthread_cond_t _cond;
};

// Обертка для независимо выполняемого потока.
// Клиентский код предоставляет реализацию интерфейса i_thread_function,
// содержащего метод func(), код которого выполняется создаваемым потоком.

class i_thread_function : public refcount {
public:
  virtual void func() = 0;
};

class thread : public value {
public:
  thread( referer<i_thread_function> thr_func ) : _thr_func(thr_func) {
    pthread_create(&_thread, NULL, code, this);
  }

  ~thread() { wait(); }

  void wait() {
    void* exit_status;
    pthread_join(_thread, &exit_status);
  }

private:
  referer<i_thread_function> _thr_func;
  pthread_t _thread;

  static void* code( void* arg );
};

}; // namespace lwml

#endif // _MTHREAD_
