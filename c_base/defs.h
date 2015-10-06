// ����� ����������� ��������, ����� � �������
// lwml, (c) ltwood

#ifndef _DEFS_
#define _DEFS_

#include "platforms.h"

// ����������� ��������� ANSI-C.
// ������� ������������ � ������������ ���� std.

#include <cassert>
#include <cctype>
#include <cfloat>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cerrno>
#include <cstdint>

// ����������� ��������� ANSI-C++.

#include <exception>
#include <new>
#include <typeinfo>

/*#lake:stop*/

// ������������ ���� std ������������� � ������������ ���� lwml.

namespace lwml {
  using namespace std;
};

namespace lwml{

// ����������� ����� �����.

typedef unsigned char  uchar;      // ����������� ������������ ������
typedef   signed char  schar;      // �������� ������������ ������
typedef unsigned int   uint;       // ����������� �����

// ����������� ����� � ������������� ��������.

typedef int16_t  int16;      // �������� 16-������ �����
typedef uint16_t uint16;     // ����������� 16-������ �����
typedef int32_t  int32;      // �������� 32-������ �����
typedef uint32_t uint32;     // ����������� 32-������ �����

// ��������� ������� ������ �������� � ���������

template <typename T>
  inline T t_min( const T& t1, const T& t2 ) { return (t1 < t2) ? t1 : t2; }
template <typename T>
  inline T t_max( const T& t1, const T& t2 ) { return (t1 > t2) ? t1 : t2; }

template <typename T>
  inline T t_minize( T& dst, const T& src ) { return dst = t_min<T>(dst, src); }
template <typename T>
  inline T t_maxize( T& dst, const T& src ) { return dst = t_max<T>(dst, src); }

template <typename T>
  inline T t_min( const T& t1, const T& t2, const T& t3 ) { return t_min<T>(t_min<T>(t1, t2), t3); }
template <typename T>
  inline T t_max( const T& t1, const T& t2, const T& t3 ) { return t_max<T>(t_max<T>(t1, t2), t3); }

// ��������� ������� ������ �������� ���������� �������.

template <typename T> inline void t_swap( T& t1, T& t2 )
{
  T tmp = t1;
  t1 = t2;
  t2 = tmp;
}

// ���� ��������

template <typename T1, typename T2 = T1>
struct t_pair {
  T1 a;
  T2 b;

  // ������� ������������� ����
  t_pair( const T1& aa, const T2& bb ) : a(aa), b(bb) {}

  // ���� ctor ���������� ���������� ��-�� ����������� �����������
  t_pair() : a(T1()), b(T2()) {}

  // ������������ �������������� ����� ��� ��������� ����
  template <typename TT1, typename TT2>
    t_pair( const t_pair<TT1, TT2>& p ) : a(p.a), b(p.b) {}
};

// ������ �������, �� �������������� �������� �����������.
// ����� ������ �������� ������ ����������� ������� � ������
// ���������� �������������� ��������� ���������.
// �������� �������� �����������, ��� ����������� �������������
// �������� ���������� ��� ������-���������� ����� ������.

class scope {
private:
  // no constructor
  scope();
};

// ������ �������-��������, ��������� �������������� �����������
// �.�. ���������� ���������� ����������� � �������� �����������.

class value {
public:
  // silly constructor ;)
  value() {}
};

// ������ ��� ����������� �����������
// �������� ������ ����������� ����������, ��� ����������� �������
// ������������ ����������� � ���� ����������� ����� ������.

class interface {
public:
  // virtual destructor
  virtual ~interface() {}
};

// ������ �������, �������������� ������� ������.
// ����� ����������� ���������� �������� ����� ������� �� ������ �������� ������
// ����������� ������� referer (refcount.h).
// ����� ������ �� ������ ������������� ����������� ����������������� ��������
// ���������� �.�. ������ ����� ��������� �����������.
//
// ����� refcount ����������� �� refcount_imp ����������
// � ����� ���������� ����������� ������������� �� ����
// ����������, ������ �� ������� ������� �������� ������,
// �.�. ����� ����������� �� refcount.

class refcount_imp {
public:
  refcount_imp() : _cnt(0) {}

  virtual ~refcount_imp() {}

  void inc_ref() const {
    ++_cnt;
  }

  void dec_ref() const {
    if( _cnt > 0 ){
      --_cnt;
      if( _cnt == 0 ) delete this;
    }
  }

private:
  mutable int _cnt;

  // no copy operations
  refcount_imp( const refcount_imp& );
  refcount_imp& operator=( const refcount_imp& );
};

class refcount : virtual public refcount_imp {
};

// ������ ������� �� ������������� ����������, �� ��������������
// ����������� (������������� ����������� ��� ���������� �����������).
// ����� ������ ������ ������� ������� �������� ��� �������������.
// �������� ��������� ����������� ����������� � �������� �����������,
// ��� ����������� ������������� ����������� ��� ����������� ����� ������.

class hardcore {
public:
  // silly constructor ;)
  hardcore() {}

private:
  // no copy operations
  hardcore( const hardcore& );
  hardcore& operator=( const hardcore& );
};

// ������� ����������� ����������� ������ � �����.
// � ����� ������ �������������� ������������� ������-������������.
// ���� ������� ������� ��������� src, �� � ������ �����
// ����������� ���������� ������ ������� �����.
char* prot_strcpy( char* dst, const char* src, int buflen );

// ������� ����������� ���������� ������ � �����.
// � ����� ������ �������������� ������������� ������-������������.
// ���������� ����� ���������� �������� ��� -1 ��� ������������ ������.
// �������������, ��� ��� ������� ������� �� ���������� ����������.
int prot_vsprintf( char* buf, int buflen, const char* fmt, va_list va );
int prot_sprintf( char* buf, int buflen, const char* fmt, ... ) PRINTF(3,4);

// ������ ����������
const char* lwml_ver();

// ����� ���������

enum tick_mode {  // ��������� ��������-���������
  tmON, tmOFF
};

enum real_format {  // ������ ������������� �����
  nfSCNT, nfFIXED
};

}; // namespace lwml

#endif // _DEFS_
