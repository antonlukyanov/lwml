#include "mcontr.h"

/*#lake:stop*/

namespace lwml {

void mcontr::transform( 
  matrix& m, real a, real b, const line_func& lf, const line_func& cf, const line_func& rf 
)
{
  int ns = m.str();
  int nc = m.col();
  for( int s = 0; s < ns; s++ ){
    for( int c = 0; c < nc; c++ ){
      real x = m(s, c);
      if( x < a )
        m(s, c) = lf(x);
      else if( x > b )
        m(s, c) = rf(x);
      else
        m(s, c) = cf(x);
    }
  }
}

void mcontr::upcontr( matrix& m, real par, real sh )
{
  if( sh == 0.0 )
    sh = par;

  real ax = m.min();
  real bx = m.max();
  real d = par * (bx-ax);                  // сдвиг от углов квадрата
  real e = sh * (bx - ax);

  line_func cf(ax+d, ax, bx-d, bx);
  line_func lf(ax, ax, cf.inv(ax+e), ax+e);
  line_func rf(cf.inv(bx-e), bx-e, bx, bx);

  transform(m, ax+d, bx-d, lf, cf, rf);
}

void mcontr::decontr( matrix& m, real par, real sh )
{
  if( sh == 0.0 )
    sh = par;

  real ax = m.min();
  real bx = m.max();
  real d = par * (bx-ax);                  // сдвиг от углов квадрата
  real e = sh * (bx - ax);

  line_func cf(ax, ax+d, bx, bx-d);
  line_func lf(ax, ax, ax+e, cf(ax+e));
  line_func rf(bx-e, cf(bx-e), bx, bx);

  transform(m, ax+d, bx-d, lf, cf, rf);
}

}; // namespace lwml
