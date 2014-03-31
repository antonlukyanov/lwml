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

// ������ ������������� �������.
//
// ����� ������ ����� ����������� ��������� � �������������,
// ������� � ����� ������ ���������� ����������� ��������,
// ��������� ����� �� ��� ������� � ������� ����� �� ���� ������.

class mtimer : public value {
public:
  mtimer() : _num(0), _sum(0.0) {}

  void start() { _tm.restart(); }

  // ���������� �����, ��������� � ������� ������
  double stop() {
    double left = _tm.left();
    _sum += left;
    _num++;
    return left;
  }

  double sum() const { return _sum; }
    // ������ ����� �� ��� ����������� �������
  int num() const { return _num; }
    // ���������� ����������� ��������
  double mid() const { return _sum / _num; }
    // ������� ����� ������ ������� �������
  double res() const { return _tm.res(); }
    // ���������� �������� �������

private:
  timer _tm;
  int _num;
  double _sum;
};

}; // namespace lwml

#endif // _TIMERS_
