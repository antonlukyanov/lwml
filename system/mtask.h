// ��������������� ���������, ��������� � ����������������.
// lwml, (c) ltwood

#ifndef _MTASK_
#define _MTASK_

#include "defs.h"
#include "stdmem.h"
#include "refcount.h"
#include "refowner.h"
#include "t_array.h"

#include "mthread.h"

/*#build_stop*/

namespace lwml {

// RAII-������� ��� ������ locker

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

// ������ ������� ��� ����������������� ������ ���������� �����.
//
// ��������������, ��� ���� N ���������� �����, ������� �����
// ��������� ���������� � M �������, � ������� ����� �� ���������.
// � ������ ������ ����������� �������� N/M �����, ������
// ������ ������������ ����������� ���������� i_tbtask, �������
// �������� ��� ������, ����������� ��� ������� ������.
// ���������� ������� j-� ������ ���������� ��� ���������� ������ func(),
// �������� � �������� ��������� ���������� ����� �������� ������.

class i_tbtask : public interface, public refcount {
public:
  virtual void func( int idx ) = 0;
};

class tb_thread_func : public i_thread_function {
  tb_thread_func() : _do_stop(false) {}

public:
  static referer<tb_thread_func> create(){
    return referer<tb_thread_func>(new(lwml_alloc) tb_thread_func());
  }

  virtual void func();

  void start( referer<i_tbtask> task, int first, int num );
  void wait();
  void stop();

private:
  referer<i_tbtask> _task;
  int _first, _num;
  bool _do_stop;
  event _wakeup, _is_ready;
};

class thread_bundle {
public:
  thread_bundle( int th_num );
  ~thread_bundle();

  void calc( referer<i_tbtask> task, int num );

private:
  int _th_num;

  t_array< referer<tb_thread_func> > _tfunc;
  t_array< refowner<thread> > _thr;
};

}; // namespace lwml

#endif // _MTASK_
