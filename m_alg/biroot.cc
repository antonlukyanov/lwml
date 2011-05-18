#include "biroot.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

real biroot::calc( const i_function& func, real a, real b, real eps )
{
  // ��������� ���������� ����������� ������� �� �������������
  // ������������� �� ������� ����� �������� eps � ������
  // ����� max(fabs(a), fabs(b)) ����������� ������ ����
  real abseps = (eps + 2.0*REAL_EPS) * (1.0 + t_max<real>(fabs(a), fabs(b)));

  real fa = func(a);
  // �������������� �� ������������ � ������������ �������
  if( fsgn(fa) * fsgn(func(b)) > 0.0 )
    fail_assert("no sign change in bisection");

  while( fabs(a - b) > abseps ){
    // �� ���� ����� m ����� �� ������� ������� ��� ����������
    real m = a + (b - a)/2.0;
    real fm = func(m);
    // �������������� �� ������������ � ������������ �������
    if( fsgn(fa) * fsgn(fm) <= 0.0 )
      b = m;
    else{
      a = m;
      fa = fm;
    }
  }
  return a + (b - a)/2.0;
}

}; // namespace lwml
