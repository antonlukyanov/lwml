#include "mtask.h"
#include "basex.h"
#include "stdmem.h"

namespace lwml {

#include <process.h> // _beginthreadex
#include <windows.h>

/*#lake:stop*/

// thread

unsigned __stdcall thread::code( void* ptr )
{
  reinterpret_cast<thread*>(ptr)->_thr_func->func();
  return 0;
}

thread::thread( referer<i_thread_function> thr_func )
{
  _thr_func = thr_func;
  // Создает поток вызовом CreateThread(),
  // вызывает _threadstart для инициализации по-поточных данных libc,
  // вызывает функцию потока.
  // По завершению функции потока _сама_ вызывает _endthreadex
  // для освобождения по-поточных данных libc,
  // но _не_ закрывает описатель потока.
  unsigned ret = _beginthreadex(
    0,                // security_attrib
    0,                // stack_size
    code,             // start_address
    this,             // arglist
    CREATE_SUSPENDED, // create_flags
    &_th_id           // thread_id
  );
  if( ret == 0 )
    fail_syscall("libc::_beginthreadex()");
  _hnd = reinterpret_cast<void*>(ret);
  _not_yet_started = true;
}

thread::~thread()
{
  // закрыть описатель потока
  if( CloseHandle(_hnd) == 0 )
    fail_syscall("win32::CloseHandle()");
}

void thread::start()
{
  if( ResumeThread(_hnd) == 0xFFFFFFFF )
    fail_syscall("win32::ResumeThread()");
  _not_yet_started = false;
}

void thread::kill()
{
  if( TerminateThread(_hnd, 0) == 0 )
    fail_syscall("win32::TerminateThread()");
}

bool thread::is_active()
{
  if( _not_yet_started )
    return false;
  else{
    DWORD exitcode;
    if( GetExitCodeThread(_hnd, &exitcode) == 0 )
      fail_syscall("win32::GetExitCodeThread()");
    return exitcode == STILL_ACTIVE;
  }
}

void thread::wait()
{
  if( WaitForSingleObject(_hnd, INFINITE) != WAIT_OBJECT_0 )
    fail_syscall("win32::WaitForSingleObject()");
}

// locker

locker::locker()
{
  _hnd = new(lwml_alloc) CRITICAL_SECTION;
  InitializeCriticalSection(static_cast<LPCRITICAL_SECTION>(_hnd));
}

locker::~locker()
{
  DeleteCriticalSection(static_cast<LPCRITICAL_SECTION>(_hnd));
  delete static_cast<LPCRITICAL_SECTION>(_hnd);
}

void locker::lock()
{
  EnterCriticalSection(static_cast<LPCRITICAL_SECTION>(_hnd));
}

bool locker::test()
{
  return TryEnterCriticalSection(static_cast<LPCRITICAL_SECTION>(_hnd));
}

void locker::unlock()
{
  LeaveCriticalSection(static_cast<LPCRITICAL_SECTION>(_hnd));
}

// event

event::event( state st )
{
  _hnd = CreateEvent(
    0,                          // security attributes
    FALSE,                      // manual-reset flag
    FALSE,                      // initial state flag
    0                           // event-object name
  );
  if( _hnd == NULL )
    fail_syscall("win32::CreateEvent()");
}

event::~event()
{
  if( CloseHandle(_hnd) == 0 )
    fail_syscall("win32::CloseHandle()");
}

void event::set()
{
  if( SetEvent(_hnd) == 0 )
    fail_syscall("win32::SetEvent()");
}

void event::wait()
{
  if( WaitForSingleObject(_hnd, INFINITE) != WAIT_OBJECT_0 )
    fail_syscall("win32::WaitForSingleObject()");
}

/*
// swimer

swimer::swimer()
  : _no_readers(event::ON), _readers_counter(0)
{
}

void swimer::write_wait()
{
  _no_writer.lock();
  _no_readers.wait();
}

void swimer::write_done()
{
  _no_writer.unlock();
}

void swimer::read_wait()
{
  _no_writer.lock();
  _readers_counter++;
  if( _readers_counter == 1 )
    _no_readers.reset();
  _no_writer.unlock();
}

void swimer::read_done()
{
  _no_writer.lock();
  if( --_readers_counter == 0 )
    _no_readers.set();
  _no_writer.unlock();
}
*/

// intex

int32 intex::inc()
{
  return InterlockedIncrement(&_data);
}

int32 intex::dec()
{
  return InterlockedDecrement(&_data);
}

int32 intex::set( int32 val )
{
  return InterlockedExchange(&_data, val);
}

}; // namespace lwml
