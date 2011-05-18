#include "goldsect.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

real gminf::calc( const i_function& func, real a, real b, real eps )
{
  // ��������� ���������� ����������� ������� �� �������������
  // ������������� �� ������� ����� �������� eps � ������
  // ����� max(fabs(a), fabs(b)) ����������� ������ ����
  real abseps = (eps + sqrt(REAL_EPS)) * (1.0 + t_max<real>(fabs(a), fabs(b)));

  real al = (sqrt( 5.0 ) - 1.0) / 2.0;
  real be = 1.0 - al;

  real y = a + be * (b - a);
  real fy = func(y);
  real z = a + al * (b - a);
  real fz = func(z);

  while( fabs(b - a) > abseps ){
    if( fy < fz ){
      b = z;
      z = y;
      fz = fy;
      y = a + be * (b - a);
      fy = func(y);
    }else{
      a = y;
      y = z;
      fy = fz;
      z = a + al * (b - a);
      fz = func(z);
    }
  }

  return a + (b - a)/2.0;
}

}; // namespace lwml
