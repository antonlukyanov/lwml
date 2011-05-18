// ��������� ������
// lwml, (c) ltwood

#ifndef _SYSTIMER_
#define _SYSTIMER_

#include "defs.h"

/*#lake:stop*/

namespace lwml {

// ��������� ������ ���������� (������� time()) ������� ����� � ��������.
// ������� ����� ������������ �� ������������� ������� �������.
// ������� res() ���������� ���������� ������� � ��������.
// �� ����������� ������������ ������ �������� ����������.

class systimer : public scope {
public:
  static double res();

  static double time();

  static void sleep( int ms = 0 );
};

}; // namespace lwml

#endif // _SYSTIMER_
