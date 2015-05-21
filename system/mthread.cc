#include "mthread.h"
#include "basex.h"
#include "stdmem.h"

#include <pthread.h>

namespace lwml {

/*#lake:stop*/

// locker

locker::locker()
{
  pthread_mutex_init(&_mutex, NULL);
}

locker::~locker()
{
  pthread_mutex_destroy(&_mutex);
}

void locker::lock()
{
  pthread_mutex_lock(&_mutex);
}

bool locker::test()
{
  return pthread_mutex_trylock(&_mutex) == 0;
}

void locker::unlock()
{
  pthread_mutex_unlock(&_mutex);
}

// rwlocker

rwlocker::rwlocker()
{
  pthread_rwlock_init(&_rwlock, NULL);
}

rwlocker::~rwlocker()
{
  pthread_rwlock_destroy(&_rwlock);
}

void rwlocker::write_wait()
{
  pthread_rwlock_wrlock(&_rwlock);
}

void rwlocker::write_done()
{
  pthread_rwlock_unlock(&_rwlock);
}

void rwlocker::read_wait()
{
  pthread_rwlock_rdlock(&_rwlock);
}

void rwlocker::read_done()
{
  pthread_rwlock_unlock(&_rwlock);
}

// event

event::event(){
  _event = false;
  pthread_mutex_init(&_mutex, NULL);
  pthread_cond_init(&_cond, NULL);
}

event::~event(){
  pthread_cond_destroy(&_cond);
  pthread_mutex_destroy(&_mutex);
}

void event::signal() {
  pthread_mutex_lock(&_mutex);
  _event = true;
  pthread_mutex_unlock(&_mutex);
  pthread_cond_signal(&_cond);
}

void event::wait(){
  pthread_mutex_lock(&_mutex);
  while( !_event )
    pthread_cond_wait(&_cond, &_mutex);
  _event = false;
  pthread_mutex_unlock(&_mutex);
}

// thread

void* thread::code( void* arg )
{
  reinterpret_cast<thread*>(arg)->_thr_func->func();
  return NULL;
}

thread::thread( referer<i_thread_function> thr_func ) : _thr_func(thr_func)
{
  pthread_create(&_thread, NULL, code, this);
}

thread::~thread()
{
  wait();
}

void thread::wait()
{
  void* exit_status;
  pthread_join(_thread, &exit_status);
}

}; // namespace lwml
