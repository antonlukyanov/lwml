// ��������-���������
// lwml, (c) ltwood

#ifndef _PROGRESS_
#define _PROGRESS_

#include "defs.h"
#include "basex.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// ���������� ��������� ��� ��������� ������� ���������

DEF_EX_TYPE(ex_base, ex_userabort, "user abort");

// ��������� ���������� ��������� �� ��������-����������.
// �������� �� ������������ ��������-����������.

class i_progress_renderer : public interface, public refcount {
public:
  // nm - �������� ��������m, sz - ��������� ����� �����.
  // ���� �������� ������������� ����� �����, �� ��� �������� �������������
  // ������� ������� ����� ����� ���������.
  // � ���� ������ ������������� ������������ ������� ���������������
  // ���� ������� ��������� up().
  virtual void start( const char* nm, int sz ) = 0;

  // ���������� �������� ����������.
  virtual void finish() = 0;

  // ��������� �������� � ����� ������� ���������.
  // �������� true, ������������ ���� ��������,
  // ������������� � ������������� �������� ���������.
  // �������� st - ������� ��������� �������� (�� 0 �� size-1).
  virtual bool up( int st ) = 0;
};

// ������ ������ ��������� � ��������� ����������.
// ������������ ���� ������ ����� ����� �������� ������
// � ������������� �������� ���������.

class progress : public scope {
public:

  // ��������� � ��������� ����������, �������� ��. � i_progress_renderer

  static void start( const char* nm, int sz ) { if( _pr.is_ok() ) _pr->start(nm, sz); }

  static void finish() { if( _pr.is_ok() ) _pr->finish(); }

  static void up( int st ) {
    if( _pr.is_ok() ){
      if( _pr->up(st) )
        throw ex_userabort();
    }
  }

  // ���������������� ���������� ��������� � ��������� ����������.
  static void set_renderer( referer<i_progress_renderer> pr ){ _pr = pr; }

private:
  static referer<i_progress_renderer> _pr;
};

}; // namespace lwml

#endif // _PROGRESS_
