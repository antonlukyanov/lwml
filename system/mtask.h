// ��������������� ���������, ��������� � ����������������.
// lwml, (c) ltwood

#ifndef _MTASK_
#define _MTASK_

#include "defs.h"
#include "mdefs.h"

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

// ����� swmr_locker ��������� ������ ������ � ������
// "���� ��������, ����� ���������".
// �������� ����� ���������� ������ �������� ����� write_wait(),
// ������� ������� ����������� ������.
// ����� ��������� ������ �������� �������� ����� write_done().
// �������� � ����������� �������� ��� ������ ���������� ������
// read_wait() � read_done().

class swmr_locker : public value {
public:
  swmr_locker();

  void write_wait();
  void write_done();

  void read_wait();
  void read_done();

private:
  locker _no_writer;
  state _no_readers;
  int _readers_counter;
};

}; // namespace lwml

#endif // _MTASK_
