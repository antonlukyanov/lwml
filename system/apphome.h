// ������ �������� ���������� ����������
// lwml, (c) ltwood

#ifndef _APPHOME_
#define _APPHOME_

#include "defs.h"
#include "mdefs.h"
#include "cstrng.h"
#include "platforms.h"

/*#lake:stop*/

// ������ �������� ���������� ���������� ������������ �����������
// ��������� �����, ��������������� � �����������, � �� �������������.
// � ������ ��� win32 ����� ����� ����������� � ��� �� ����������,
// ��� � ���������� ������ ����������.
// ��� *nix ����� ����������� �� �������������� ����, ������� �����������
// �� ���������� ����� LWML_HOME (�� �����������).
// ������� mk_fname() �� �������������� ����� ����� (�.�. ����� � ���������� ��� ����)
// �������� ������ ��� ����� �����, ������������ � ������������� dump
// �������� ���������� ����������.

namespace lwml {

class apphome : public scope {
public:
  static strng mk_fname( const char* fnm );

  static strng mk_dump( const char* fmt, ... ) PRINTF(1,2);
  static strng mk_dump_v( const char* fmt, va_list va );
};

}; // namespace lwml

#endif // _APPHOME_
