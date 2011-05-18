// resampling
// lwml, (c) ltwood

#ifndef _RESAMPLE_
#define _RESAMPLE_

#include "defs.h"
#include "mdefs.h"

#include "vector.h"
#include "matrix.h"

/*#lake:stop*/

namespace lwml {

class lanczos : public scope {
public:
  // ���������� �������/������� src � ������/������� dst.
  // �������� r ������ ������ ���� �������, ������������� ��� �����������.
  // ������� �������� ��� ������� ���� -- 2, 3, 4.
  static void warp( vector &dst, const vector &src, real r = 3 );
  static void warp( matrix &dst, const matrix &src, real r = 3 );

  // ���������� �������� ������������ ������� � ������������ �����.
  // ���������� ����� �������� � ������������ ����������� �.�.
  // �� 0 <= x <= len-1 �� ������ ����������.
  static real calc( const vector &src, real x, real r = 3 );
  static real calc( const matrix &src, real y, real x, real r = 3 );

private:
  static real sinc( real x ) { return lwml::sinc(M_PI * x); }

  static real lanczos_kernel( real x, real r ){
    return fabs(x) <= r ? sinc(x) * sinc(x/r) : 0.0;
  }

  static real calc_in_line( const matrix &src, int y, real x, real r );
};

}; // namespace lwml

#endif // _RESAMPLE_
