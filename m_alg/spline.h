// Cubic spline
// lwml, (c) ltwood

#ifndef _SPLINE_
#define _SPLINE_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"
#include "gj3d.h"
#include "matrix.h"

/*#lake:stop*/

namespace lwml {

// ������������ ���������� ��������

class spline : public value {
public:
  spline( int sz );

  // ���������� ������������ ������� ��� x, y
  // der_l, der_r ������ �������� ������ ����������� �� ������ �������
  void put( const vector& x, const vector& y, real der_l, real der_r );

  // ��������� �������� ������� � �. xx. ���� xx �������� ��� ���������
  // ����� ���������� ������� x, ������������ ������.
  real calc( real xx ) const {
    return calc_imp(_x, _y, _coeff, xx);
  }

  // �������� ������ ������������� ��� ������������� �������.
  // ����� ������� ������������� ����� 4*(sz-1) � �������:
  // a_0, b_0, c_0, d_0, a_1, b_1, c_1, d_1, ...
  const vector& coeff() const { return _coeff; }

  // ��������� �������� ������� � �. xx �� ������������� � ������
  static real calc( const vector& x, const vector& y, const vector& coeff, real xx );

private:
  class size_checker : value {
  public:
    size_checker( int sz ) {
      if( sz < 4 )
        fail_size(sz);
    }
  };
  size_checker _szcheck;  // �����, ����� �������� ����������� �� ������
                          // ������������� ��������
  int _size;
  vector _x, _y, _coeff;
  gj3d _gj;

  static real calc_imp( const vector& x, const vector& y, const vector& coeff, real xx );
};

/* ������������ ������������ ��������.
   �������� �� ������ spline.
   �� ������ �������� ������� �������� ������ �����������.
   ������������! � ������� ���� ������������ ������ ��� ����������
   ��������� ������������ ��������, ��� ��� ����������� �����������.
*/

class spline2d : public value {
public:
  spline2d( int str, int col );

  void put( const vector& x, const vector& y, const matrix& z );

  real calc( real xx, real yy ) const;
private:
  int _str, _col;
  vector _x, _y;  // �������� x, y, �������� �����
  matrix _z;      // �������� ������� � ����� �����
  matrix _col_cs; // ����������� ��������, ����������� �� ��������
  mutable vector _buf_str; // ����� ��� ������ ��������
  mutable vector _buf_col;
  mutable vector _buf_col_cs;
  mutable spline _sp_str;  // ������ �� ������
};

}; // namespace lwml

#endif // _SPLINE_
