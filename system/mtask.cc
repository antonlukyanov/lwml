#include "mtask.h"

/*#build_stop*/

namespace lwml {

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

}; // namespace lwml
