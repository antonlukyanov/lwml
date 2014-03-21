// ��������� ���������������.
// �������� ������, ��������������� ������ ������� � �� �������������.
// lwml, (c) ltwood

#ifndef _MTHREAD_
#define _MTHREAD_

#include "defs.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// ����������� ������ �������.

class locker : public value {
public:
  locker();
  ~locker();

  // ��������� ������
  void lock();

  // ���������� ��������� ������
  // ���������� ���� �����.
  bool test();

  // ���������� ������
  void unlock();

private:
  void* _mutex;
};

// ����� rw_locker ��������� ������ ������ � ������
// "���� ��������, ����� ���������".
// �������� ����� ���������� ������ �������� ����� write_wait(),
// ������� ������� ����������� ������.
// ����� ��������� ������ �������� �������� ����� write_done().
// �������� � ����������� �������� ��� ������ ���������� ������
// read_wait() � read_done().

class rwlocker : public value {
public:
  rwlocker();
  ~rwlocker();

  void write_wait();
  void write_done();

  void read_wait();
  void read_done();

private:
  void* _rwlock;
};

// ����� event ��������� ������� ������ condition variable.
// �� ������� ������������ ����� ���� ������ ��� �����.
// ���� �� ���� ����� �� ���� �������, �� ������� ��������� ����������.

class event : public value {
public:
  event();
  ~event();

  void signal();
  void wait();

private:
  bool _event;
  void* _mutex;
  void* _cond;
};

// ������� ��� ���������� ������������ ������.
// ���������� ��� ������������� ���������� ���������� i_thread_function,
// ����������� ����� func(), ��� �������� ����������� ����������� �������.

class i_thread_function : public interface, public refcount {
public:
  virtual void func() = 0;
};

class thread : public value {
public:
  thread( referer<i_thread_function> thr_func );
  ~thread();

  void wait();

private:
  referer<i_thread_function> _thr_func;
  void* _thread_ptr;

  static void* code( void* arg );
};

}; // namespace lwml

#endif // _MTHREAD_
