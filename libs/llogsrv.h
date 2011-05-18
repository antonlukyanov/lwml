// ������ ������ ��������������� ���������
// lwml, (c) ltwood

#ifndef _LLOGSRV_
#define _LLOGSRV_

#include "defs.h"

/*#lake:stop*/

namespace lwml {

// ������� ��� ����������� llogsrv.dll, ��������������
// ����� ��������������� ��������� � ����� ������ � ����.
// ��� ��������� �������� ���������� llogsrv ������� ������
// ��������������� ��������� �� ���������� ������� ��������.
//
// is_active() - ���������� ������, ���� ���������� ����������� �������.
// out_log(asp, msg) - ������� � ����� ����������� ��������� msg � �������� asp.
// is_dump() - ���������� ������, ���� ��������� ����� �����.
// is_jit() - ���������� ������, ���� ��������� ����������� jit-���������� ��� ������.
// get_id() - ���������� ������������� ��������.
// get_ct() - ���������� ������������� ������� �������.
// ��������� ��� ������� ������������ ��� ������������ ����� ����� �����.

class llogsrv : public scope {
public:
  static bool is_active() { return init(); }

  static void out_log( const char* asp, const char* msg );

  static bool is_dump();
  static bool is_jit();

  static uint get_id();
  static uint get_ct();

private:
  static bool init();
};

}; // namespace lwml

#endif // _LLOGSRV_
