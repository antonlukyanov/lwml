// ��������� ���������������
// �������� ������, ��������������� ��������� ������� ������� �
// �� ������������� ��������-����������� �������
// lwml, (c) ltwood

#ifndef _MTHREAD_
#define _MTHREAD_

#include "defs.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// locker ��� win32 ����������� ��� lightweight mutex (aka critical section).
// ��������� ������������� ����������� ������ ����������.

class locker : public value {
public:
  // ������� ���� ��������� ������� � �������� ��������� ����������
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
  void* _hnd;
};

// ����� event ��������� �������.
// ���������� ������� ������, ����� ������� ����� ������
// ���� ��������� ��� ����� � ����� �� ������������� ������������.
// ��� ���� ���� �� ������ ����������� ������� �� ���� ����� �� �������
// ��� �����������, �� ������� �������� �� ���������� ��������� ��
// ��������� ������ ������.
// ����� ������ wait() �������� ����� �� ����������� �������.
// ����� set() ������� ���� �������.

class event : public value {
public:
  enum state { ON, OFF };

  event( state st = ON );
  ~event();

  void set();
  void wait();

private:
  void* _hnd;
};

// ����� intex ��������� ���������� �������������� ������� �����
// ����������� ��� ������������� ����������.
// ��� ����������� ���������� ����� ���������
// ��������� �������� ���������� inc() � dec().
// ������� �������� ���������� ����� �������� ������ ����� get().
// ����� set() ������������� ����� �������� ����������
// ������ inc() � dec() ���������� �����, ���� �������� ���������
// �� ������ ���������� ����� ���������� ��������.
// ����� set() ���������� ���������� �������� ����������.

class intex : public value {
public:
  intex( int32 val = 0 ) { _data = val; }

  int32 inc();
  int32 dec();
  int32 set( int32 val );

  int32 get() const { return _data; }

private:
  int32 _data;
};

// ������� ��� ���������� ������������ ������.
// ���������� ��� ������������� ���������� ���������� i_thread_function,
// ����������� ����� func(), ��� �������� ����������� ����������� �������.
// ����� ��������� � ���������������� ��������� � ������ ���� �����������.

class i_thread_function : public interface, public refcount {
public:
  virtual void func() = 0;
};

class thread : public value {
public:
  thread( referer<i_thread_function> );
  ~thread();

  void start();     // ������ ������
  void kill();      // ������������� ��������� �����
  bool is_active(); // ���������, ����������� �� ����� � ������ ������
  void wait();

private:
  referer<i_thread_function>    _thr_func;
  void*                         _hnd;
  unsigned                      _th_id;
  bool                          _not_yet_started;

  static unsigned __stdcall code( void* ptr );
  // ���������, ��� �� ����������� �������� ������������ __stdcall
  // ��� ������� � MinGW-GCC � VC-7 ��� ��������...
};

}; // namespace lwml

#endif // _MTHREAD_
