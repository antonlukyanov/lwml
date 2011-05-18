#include "integral.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

real integral::fixed( const i_function& func, real a, real b, int sect )
{
  real dx = (b - a) / sect;     // мелкость разбиения
  double res = 0.0;
  real x = a;
  for( int j = 0; j < sect; j++ ){
    res += func(x);
    x += dx;
  }
  return res * dx;
}

real integral::adapt( const i_function& func, real a, real b, int sect, real eps )
{
  int bisect = intlog2(sect);
  real prev;
  real res = fixed(func, a, b, sect);
  do{
    prev = res;
    sect *= 2;
    res = fixed(func, a, b, sect);
    if( ++bisect > 128 ) //!! SIC: !!
      throw ex_noconv();
  }while( fabs(res-prev) > eps * (1.0 + hypot(prev, res)) );
  return res;
}

}; // namespace lwml
