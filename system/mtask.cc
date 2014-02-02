#include "mtask.h"

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
  _readers_counter++;
  if( _readers_counter == 1 )
    _no_readers.set(false);
  _no_writer.unlock();
}

void swmr_locker::read_done()
{
  _no_writer.lock();
  if( --_readers_counter == 0 )
    _no_readers.set(true);
  _no_writer.unlock();
}

}; // namespace lwml
