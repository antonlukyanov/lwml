// ����������� ������������ ���������
// lwml, (c) ltwood

#ifndef _GEOMX_
#define _GEOMX_

#include "defs.h"
#include "mdefs.h"
#include "geom.h"

/*#lake:stop*/

namespace lwml {

// ����� ��������� ���������� � ������� �� ���������
// �������� ������ ����������� �������
// ������� dist() ��������� ���������� �� ����� �� ������
// ������� side() ���������� �� ����� ������� �� ������ ����� ����� (+/-1)
// ������� is_coside() ���������� ����� �� ������ 2 �����
// �� ���� ������� �� ������
// �� ���� ������� ������ �������� ����� ������� (p1, p2)

class lines : public scope {
public:
  static real dist(
    const fpoint& pp,
    const fpoint& p1, const fpoint& p2
  );

  static real side(
    const fpoint& pp,
    const fpoint& p1, const fpoint& p2
  );

  static bool is_coside(
    const fpoint& pp1, const fpoint& pp2, const fpoint& p1, const fpoint& p2
  );
};

// ����� ��������� ���������� � �������������� �� ���������
// �������� ������ ����������� �������
// ������� area() ��������� ������� ������������
// ������� is_in() ��������� ����� �� ����� � ������������

class triangle : public scope {
public:
  static real area( const fpoint& p1, const fpoint& p2, const fpoint& p3 );

  static bool is_in( const fpoint& pp,
    const fpoint& m1, const fpoint& m2, const fpoint& m3
  );
};

// ����� ��������� ��������� ����� �� �������� ������������� �������
// (�������� �����-����������).
// ������� get_code() ��������� ��� ����� �������� 4-������ �������� ���,
// ������������ �� ��������� ������������ ������� ���������:
// clipcode = (b3,b2,b1,b0) = (x<x1,x>x2,y<y1,y>y2) = xxyy
//   1001|0001|0101
//   ----+----+----
//   1000+0000+0100
//   ----+----+----
//   1010|0010|0110
// ������� clip_line() ���������� �������� ����� �.�.
// ������������ ��������� �����, �������� ����� �������
// ������������ �������� - ����, ������������, ��� ���������
// ���� �����������

class clipping : public scope {
public:
  static uchar get_code( const fpoint& pnt, const frect& cliprect );
  static bool  clip_line( fpoint& l1, fpoint& l2, const frect& cliprect );
};

// implementation

// lines

inline real lines::dist( const fpoint& pp, const fpoint& p1, const fpoint& p2 ){
  fpoint d(p2.x() - p1.x(), p2.y() - p1.y());
  real dn = d.norm();
  fpoint e(d.x()/dn, d.y()/dn);
  fpoint mpp(pp.x() - p1.x(), pp.y() - p1.y());
  real p = fpoint::inner_mul(mpp, e);
  return sqrt(mpp.mag() - fsqr(p));
}

inline real lines::side(
  const fpoint& pp,
  const fpoint& p1, const fpoint& p2
){
  return fsgn(
    (pp.x()-p1.x())*(p2.y()-p1.y()) - (p2.x()-p1.x())*(pp.y()-p1.y())
  );
}

inline bool lines::is_coside(
  const fpoint& pp1, const fpoint& pp2,
  const fpoint& p1, const fpoint& p2
){
  return side(pp1, p1, p2) * side(pp2, p1, p2) >= 0;
}

// triangle

inline real triangle::area(
  const fpoint& p1, const fpoint& p2, const fpoint& p3
){
  real a = fpoint::dist(p1, p2);
  real b = fpoint::dist(p2, p3);
  real c = fpoint::dist(p3, p1);
  real p = 0.5 * (a + b + c);
  return sqrt(p * (p-a) * (p-b) * (p-c));
}

inline bool triangle::is_in( const fpoint& pp,
  const fpoint& p1, const fpoint& p2, const fpoint& p3
){
  return lines::is_coside(pp, p1, p2, p3) &&
         lines::is_coside(pp, p2, p1, p3) &&
         lines::is_coside(pp, p3, p1, p2);
}

}; // namespace lwml

#endif // _GEOMX_
