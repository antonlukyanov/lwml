// ��������� ���
// lwml, (c) ltwood

#ifndef _LMS_
#define _LMS_

//!! TODO: �������� �����������

#include "mdefs.h"
#include "polyn.h"
#include "vector.h"
#include "matrix.h"
#include "lud.h"

/*#lake:stop*/

namespace lwml {

// �������������� ������������� �� ���
// ����� �������� ������ ���� ����������� ������� calc(),
// ����������� ���������������� �������
// x, y - �������, ���������� ������� �����
// c    - �������, � ������� ���������� ����������� ������������
// ���������� ������������� ������ �������������.

class lmspoly : public scope {
public:
  static real calc( const vector& x, const vector& y, polyn& c );
};

// ����������� ���������������� ���� �� ���.
// �������� ���������������� ������� Ax=b, � ������� ������� ��������� ������������� ��� p[j].
// (�������, �� ����������� �������� p ������ �� �� ������ ��� ����������� ���������.)
// ���� ������� A ����� ������ [mxn], �� ������ x ����� ������ [n], � ������� b � p ����� ������ [m].
// ���������� ������������� ������ �������������.

class lmsleq : public value {
public:
  lmsleq( int n );

  real calc( vector& x, const matrix& a, const vector& b, const vector& p );
  real calc( vector& x, const matrix& a, const vector& b );

private:
  int _n;
  matrix _a;
  vector _b;
  lud _lu;
};

}; // namespace lwml

#endif // _LMS_
