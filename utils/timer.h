// ������� ������������� ������
// lwml, (c) ltwood

#ifndef _TIMERS_
#define _TIMERS_

#include "defs.h"
#include "cstrng.h"
#include "lwmlconf.h"

#include "systimer.h"

/*#lake:stop*/

namespace lwml {

class timer : public value {
public:
  // ������ ������ �������
  timer() { restart(); }

  // �������� ������ � �������� ��������� � ������ ����� ������ �������
  void restart()  { _start = systimer::time(); }

  // �������� �����, ��������� � ������� ���������� ������ �������
  double left() const { return systimer::time() - _start; }

  // �������� ���������� ������� ��������� �����
  static double time() { return systimer::time(); }

  // �������� ���������� ������������� �������
  static double res() { return systimer::res(); }

  // �������� ������ ��������� �������� �������� ������� �������
  // ���������� ������, ���������� ������ ��������� �����
  // �������� ������� ������� (����-�����).
  static strng timestamp();

private:
  double _start;
};

}; // namespace lwml

#endif // _TIMERS_
