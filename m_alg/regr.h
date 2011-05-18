// �������� ���������
// lwml, (c) ltwood

#ifndef _REGR_
#define _REGR_

#include "defs.h"
#include "vector.h"

/*#lake:stop*/

namespace lwml {

// ����� lregr ��������� ���������� �������������
// �������� (�������� ����������) ���������
// ���������� ������� ������������� �������������
// ��������� ������� put(*,*), ������� ���������� ������
// �������� ��������� � ������ �������� �������
// ���������� ������� ������������� �������������
// ��������� ������� put(*,*,*), ������� �������������
// ���������� ������ ����� ��������
// ������� k() � �() ��������� �������� ����������� ���������,
// � �������� operator() ��������� ��������� ��������
// ���������� �������� �������

class lregr : public value {
public:
  lregr() : _k(0.0), _c(0.0) {}

  void put( const vector& x, const vector& y );
  void put( const vector& x, const vector& y, const vector& w );

  real k() const { return _k; } // �������� ������� �����������
  real c() const { return _c; } // �������� ��������

  // ��������� �������� �������� �������
  real operator()( real x ) const { return _k * x + _c; }

private:
  real _k, _c;

  real midw( const vector& x, const vector& w, real ws );
};

}; // namespace lwml

#endif // _REGR_
