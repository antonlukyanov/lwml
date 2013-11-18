// ������-����������
// lwml, (c) ltwood

#ifndef _BASEX_
#define _BASEX_

#include "defs.h"
#include "t_strbuf.h"
#include "platforms.h"
#include "lwmlconf.h"

/*#lake:stop*/

/*
 * ��� ����� -- ����� ����� �������������� �� ���� lwml.
 * ���� �������� ������� ��������� �������� ������� lwml.
 *
 * ������������ ���� ����������� ���������� -- �� ����������� ���������
 * �.�. ����������� ������������ � ����� catch ����� ������ ����������,
 * �� ���� ����� ��� ����������� �� ������������, � ����������� ��������
 * ������ �� ��������� ������������ "�������������� �����������".
 * ������������ ��������� ���������� � ��������� ����� �������������
 * ������ ��� ����, ����� ����� ���� ������ ���������� ����������.
 * ��� ���� ����� ������������, � ������ �������� ���������� ��� ����������
 * ��������� ��������� �� �������.
 * ����� ��������� ��� ����������� ��� �������� �������-����������
 * � lwml � ����������� ��� ���������� ������������ �� �������-����������,
 * ������� ����� ���� ����������� ������, ��� �������-����������.
 * ������������ ��� �������� ���������� � �������������� ����������� ����������
 * ��������� � ����� ����� �������, � ��� ����� �������� �� �������� ���������
 * �� ������� �������� � ����� �������, �������� ������-����������
 * ������ ��� ������� �����, ��������������� �� ��������������� ��������������.
 */

namespace lwml {

// ������� ����� ��� ����������.
// ��� ������� ����������� ���� ������ ����� ������������ ������� ����� ������,
// �������������� ����������� ����������� ��������� ����������.

class i_exception : public interface {
public:
  // ��������� ��������� �� ������.
  virtual const char* msg() const = 0;

  static void raise_jit( const char* msg );
};

typedef i_exception ex_base;
typedef ex_base error;

// ����� ������ ��� ��������� �� ����������.
// ����� ����������� � ������ ���� �� ����� ������.

const int EX_BUFLEN = 512;

// ������� ��� ����������� ����� ����������

#define DEF_EX_CLASS( base_name, class_name )        \
  class class_name : public base_name { }            //

#define DEF_EX_TYPE( base_name, type_name, mesg )    \
  class type_name : public base_name, public value { \
  public:                                            \
    type_name() { raise_jit(mesg); }                 \
                                                     \
    virtual const char* msg() const { return mesg; } \
  }                                                  //

#define DEF_EX_TYPE_MSG( base_name, type_name, mesg )           \
  class type_name : public base_name, public value {            \
  public:                                                       \
    PRINTF(2,3) type_name( const char* fmt, ... ) {             \
      va_list va;                                               \
      va_start(va, fmt);                                        \
      _msg.put(mesg).put(": ");                                 \
      _msg.vprintf(fmt, va);                                    \
      va_end(va);                                               \
                                                                \
      raise_jit(_msg.get());                                    \
    }                                                           \
                                                                \
    virtual const char* msg() const { return _msg.get(); }      \
                                                                \
  private:                                                      \
    strbuf<EX_BUFLEN> _msg;                                     \
  }                                                             //

// �������� ��� ����������, ��������� � �������� ����������� ������

DEF_EX_CLASS(ex_base, ex_integrity);

// ����� ������������������� (�� ������� ������������ ������-����������)
// ������ ������������ � runtime'�.
// ���� ��������� ��� ���������� ������ �.�. ������ ������� ����������,
// ������� �� ������ ����������� ��� ���������� ���������.
// ��� ������ ������ ����� ������������� � ������� ��������������� ��������.
// �� ������������� �������������� ��� ���������� ��������������.
// ��� ������������� ������������������� ���������� ������� ������������
// ���� �� �������� fail_xxx() ��� runtime.

DEF_EX_TYPE_MSG(ex_base, ex_runtime, "runtime error");

// �����-�������, ���������� ���������� � ����� ������������� ����������

class runtime_location : public value {
public:
  runtime_location( const char* fname, const char* func, int line );

  void operator()( const char* fmt, ... ) PRINTF(2,3);

private:
  strbuf<64> _fname;
  strbuf<64> _func;
  int _line;
};

// ������, ������������ ���������� ex_runtime.

#define runtime runtime_location(__FILE__, __FUNCTION__, __LINE__)

// �������, ������������ ���������� ������������������� ����������

#define fail_assert( msg ) runtime("assertion failed: %s", msg)
#define fail_syscall( fn ) runtime("system call <%s> failure", fn)
#define fail_unexpected()  runtime("unexpected behavior")
#define fail_index( idx )  runtime("incorrect index value <idx=%d>", idx)
#define fail_size( sz )    runtime("incorrect sizes value <sz=%d>", sz)

// �������, ����������� ������� � ������������ ����������

#define test_index( idx, sz )                                                      \
  if( idx < 0 || idx > sz-1 )                                                      \
    runtime("index out of bounds <idx=%d, sz=%d>", idx, sz)                        //

#define test_index2( idx1, sz1, idx2, sz2 )                                        \
  if( idx1 < 0 || idx1 > sz1-1 || idx2 < 0 || idx2 > sz2-1 )                       \
    runtime("index out of bounds <idx=(%d,%d), sz=(%d,%d)>", idx1, idx2, sz1, sz2) //

#define test_size( sz, exp )                                                       \
  if( sz != exp )                                                                  \
    runtime("incorrect sizes <sz=%d, exp=%d>", sz, exp)                            //

#define test_size2( sz1, exp1, sz2, exp2 )                                         \
  if( sz1 != exp1 || sz2 != exp2 )                                                 \
    runtime("incorrect sizes <sz=(%d,%d), exp=(%d,%d)>", sz1, sz2, exp1, exp2)     //

#ifdef _CHECK_IDX_

#define TEST_INDEX(idx, sz) test_index( idx, sz )
#define TEST_INDEX2(idx1, sz1, idx2, sz2) test_index2( idx1, sz1, idx2, sz2 )
#define TEST_SIZE(sz, exp) test_size( sz, exp )
#define TEST_SIZE2(sz1, exp1, sz2, exp2) test_size2(sz1, exp1, sz2, exp2)

#else

#define TEST_INDEX(idx, sz)
#define TEST_INDEX2(idx1, sz1, idx2, sz2)
#define TEST_SIZE(sz, exp)
#define TEST_SIZE2(sz1, exp1, sz2, exp2)

#endif

}; // namespace lwml

#endif // _BASEX_
