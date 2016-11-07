#include "lwml/m_alg/biroot.h"
#include "lwml/base/basex.h"

/*#lake:stop*/

namespace lwml {

real biroot::calc( const i_function& func, real a, real b, real eps )
{
  // вычисляем абсолютную погрешность решения по относительной
  // предохраняясь от слишком малых значений eps и случая
  // когда max(fabs(a), fabs(b)) расположено вблизи нуля
  real abseps = (eps + 2.0*REAL_EPS) * (1.0 + t_max<real>(fabs(a), fabs(b)));

  real fa = func(a);
  // предохраняемся от переполнения и исчезновения порядка
  if( fsgn(fa) * fsgn(func(b)) > 0.0 )
    fail_assert("no sign change in bisection");

  while( fabs(a - b) > abseps ){
    // не даем точке m выйти за границы отрезка при округлении
    real m = a + (b - a)/2.0;
    real fm = func(m);
    // предохраняемся от переполнения и исчезновения порядка
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
