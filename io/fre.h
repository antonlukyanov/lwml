// �������� ���������� ���������.
// lwml, (c) ltwood

#ifndef _FRE_
#define _FRE_

#include "defs.h"

/*#lake:stop*/

namespace lwml {

// ������������� ����� ����� fn � �������� ������ re.
// �������� ����� ������������ ����� ������������
// �������� ����� Posix (��������� ��������� ��������).
// �� ��������� � ������������ DOS/Win ����� �����������
// ����������� � ���������� ��������� ���������� �������� '*'.
// ��������� ������������ ��� ����� �������� ��������.

class fre : public scope {
public:
  static bool is_match( const char* re, const char* str );

private:
  static bool matchstar( const char* re, const char* str );
};

}; // namespace lwml

#endif // _FRE_
