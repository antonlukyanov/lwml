#include "mtask.h"

#include <windows.h>

/*#build_stop*/

namespace lwml {

// swmr_locker

swmr_locker::swmr_locker()
  : _readers_counter(0)
{
  _no_readers.set(true);
}

void swmr_locker::write_wait()
{
  _no_writer.lock();
  _no_readers.wait();
}

void swmr_locker::write_done()
{
  _no_writer.unlock();
}

void swmr_locker::read_wait()
{
  _no_writer.lock();
  if( InterlockedIncrement(&_readers_counter) == 1 )
    _no_readers.set(false);
  _no_writer.unlock();
}

void swmr_locker::read_done()
{
  if( InterlockedDecrement(&_readers_counter) == 0 )
    _no_readers.set(true);
}

}; // namespace lwml
