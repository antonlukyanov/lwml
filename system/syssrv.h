// ��������� �������� ������ ��������� �� �������
// lwml, (c) ltwood

#ifndef _SYSMSG_
#define _SYSMSG_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

class syssrv : public scope {
public:
  // ������� ��������� � ������� ���������� ������� ������ ���������.
  static void message( const char *title, const char *msg );

  // ������� ������ � ������� ���������� ������� ������ ��������.
  static int ask( const char *title, const char *msg );

  // ��������� ����������� �������������� ��������.
  static uint32 get_pid();

  // ��������� ����������� �������������� ������.
  static uint32 get_tid();

  // �������������� JustInTime (JIT) �������� �.�. �������� ������ ����������.
  // ��� ���������� ���������� ������� Mingw ������������ JIT-�������� DrMingw.
  static void call_jit();

  // ����� ��������� � ��������� ����� �����������.
  static void debug_message( const char* );

private:
  static void exit( const char* msg );
};

}; // namespace lwml

#endif // _SYSMSG_
