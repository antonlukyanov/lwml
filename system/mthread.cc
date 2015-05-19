#include "mthread.h"
#include "basex.h"
#include "stdmem.h"

#include <pthread.h>

namespace lwml {

/*#lake:stop*/

// locker

#define p_MUTEX reinterpret_cast<pthread_mutex_t*>(&_mutex)

locker::locker()
{
  pthread_mutex_init(p_MUTEX, NULL);
}

locker::~locker()
{
  pthread_mutex_destroy(p_MUTEX);
}

void locker::lock()
{
  pthread_mutex_lock(p_MUTEX);
}

bool locker::test()
{
  return pthread_mutex_trylock(p_MUTEX) == 0;
}

void locker::unlock()
{
  pthread_mutex_unlock(p_MUTEX);
}

// rwlocker

#define p_RWLOCK reinterpret_cast<pthread_rwlock_t*>(&_rwlock)

rwlocker::rwlocker()
{
  pthread_rwlock_init(p_RWLOCK, NULL);
}

rwlocker::~rwlocker()
{
  pthread_rwlock_destroy(p_RWLOCK);
}

void rwlocker::write_wait()
{
  pthread_rwlock_wrlock(p_RWLOCK);
}

void rwlocker::write_done()
{
  pthread_rwlock_unlock(p_RWLOCK);
}

void rwlocker::read_wait()
{
  pthread_rwlock_rdlock(p_RWLOCK);
}

void rwlocker::read_done()
{
  pthread_rwlock_unlock(p_RWLOCK);
}

// event

#define p_COND reinterpret_cast<pthread_cond_t*>(&_cond)

event::event(){
  _event = false;
  pthread_mutex_init(p_MUTEX, NULL);
  pthread_cond_init(p_COND, NULL);
}

event::~event(){
  pthread_cond_destroy(p_COND);
  pthread_mutex_destroy(p_MUTEX);
}

void event::signal() {
  pthread_mutex_lock(p_MUTEX);
  _event = true;
  pthread_mutex_unlock(p_MUTEX);
  pthread_cond_signal(p_COND);
}

void event::wait(){
  pthread_mutex_lock(p_MUTEX);
  while( !_event )
    pthread_cond_wait(p_COND, p_MUTEX);
  _event = false;
  pthread_mutex_unlock(p_MUTEX);
}

// thread

void* thread::code( void* arg )
{
  reinterpret_cast<thread*>(arg)->_thr_func->func();
  return NULL;
}

#define p_THREAD reinterpret_cast<pthread_t*>(_thread_ptr)

thread::thread( referer<i_thread_function> thr_func ) : _thr_func(thr_func)
{
  _thread_ptr = raw_alloc<pthread_t>();
  pthread_create(p_THREAD, NULL, code, this);
}

thread::~thread()
{
  wait();
  raw_free<>(_thread_ptr);
}

void thread::wait()
{
  void* exit_status;
  pthread_join(*p_THREAD, &exit_status);
}

}; // namespace lwml
