#include "mtask.h"

/*#build_stop*/

namespace lwml {

// tb_thread_func

void tb_thread_func::func(){
  while( true ){
    _wakeup.wait();
    if( _do_stop ){
      _is_ready.signal();
      return;
    }
    for( int j = 0; j < _num; j++ )
      _task->func(_first + j);
    _is_ready.signal();
  }
}

void tb_thread_func::start( referer<i_tbtask> task, int first, int num ) {
  _task = task;
  _first = first;
  _num = num;
  _wakeup.signal();
}

void tb_thread_func::wait() {
  _is_ready.wait();
}

void tb_thread_func::stop() {
  _do_stop = true;
  _wakeup.signal();
}

// thread_bundle

thread_bundle::thread_bundle( int th_num )
  : _th_num(th_num), _tfunc(th_num), _thr(th_num)
{
  for( int j = 0; j < _th_num; j++ ){
    _tfunc[j] = tb_thread_func::create();
    _thr[j].reset(new(lwml_alloc) thread(_tfunc[j]));
  }
}

thread_bundle::~thread_bundle()
{
  for( int j = 0; j < _th_num; j++ )
    _tfunc[j]->stop();
  for( int j = 0; j < _th_num; j++ )
    _tfunc[j]->wait();
}

void thread_bundle::calc( referer<i_tbtask> task, int tnum )
{
  int p = tnum / _th_num;
  int q = tnum % _th_num;

  for( int j = 0; j < _th_num; j++ ){
    int num = p + ((j < q) ? 1 : 0);
    int first = (j < q) ? j * (p+1) : q * (p+1) + p * (j - q);
    _tfunc[j]->start(task, first, num);
  }

  for( int j = 0; j < _th_num; j++ )
    _tfunc[j]->wait();
}

}; // namespace lwml
