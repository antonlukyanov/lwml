// ����� ��� ���������� ��������� ��������
// lwml, (c) ltwood

#ifndef _CONSOLE_
#define _CONSOLE_

#include "defs.h"

#include "basex.h"
#include "cstrng.h"
#include "progress.h"

/*#lake:stop*/

namespace lwml {

// ���������� ������� ��� ���������� ���������� ��� �����

DEF_EX_TYPE_MSG(ex_base, ex_console, "console error");

// ����� console ��������� ����� ���������� ���������� �������������� ��������.
// ��� ��������������� �������� ��������� ������� main().
// �������� ����������������:
//   - ����������� ��������� ��������� �� ������
//   - ��������� ���������� � ����� ��������� �� ������
//   - ��������� ���������� ��������� ������ � �����
// ������������� ������� ������������ ���������� �������� ��������-����������
// � ������ �������� ���������� ����������.
//
// ����� ��������� ������ ������� �� ������ �������, ����� �������
// ����� ���� '-' � ����� ������������� � ������������ ����� ��������� ������.
// �������� ����� ����� ��������� ��������������� �� �������� �����
// ��� ���������� �� ���� �������� ':' ��� �������� '='.
// ��������� ��������� � ���������� ��� ����� �����.

class console : public scope {
public:
  enum errmsg_mode { USE_SYSMSG, USE_CONSOLE };

  static void init( int argc, char *argv[], errmsg_mode errmsg = USE_CONSOLE );

  // ���������� ����������� ������

  // ���������������� ��������� ��������� � ������� ��������� �� ������.
  static void abort( const char *msg );

  // ��������� �� �������� ���������� ��������� ������.
  // ��������� - �������� ��������� � ������� ����������� ������.
  static void usage( const char *name, const char *usage );

  // ���������� �������������� ����������.
  // �������� ��������� � ������������� ���������
  static void handlex( const ex_base& ex );

  // ��������� ������� � ��������� ������

  // ��������� ���������� ����������
  static strng app_name();          // ��� ����������� ������ ���������
  static strng app_path();          // ���� � ����������� ������ ���������

  // ��������� ����������
  static int   argc();              // ����� ���������� ��������� ������
  static strng argv( int k = 0 );   // k-� �������� ��������� ������

  // ��������� �����
  static bool  is_opt( char ch );   // ��������� ������� �����
  static strng get_opt( char ch );  // �������� �������� ����� ��� ������

private:
  static void print_msg( const char* title, const char* msg );
  static void print_msg2( const char* title, const char* msg );
};

}; // namespace lwml

#endif // _CONSOLE_
