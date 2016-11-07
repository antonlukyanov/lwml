#include "lwml/psgraph/vgadgets.h"
#include "lwml/base/basex.h"

/*#lake:stop*/

namespace lwml {

fpoint viewrect::gold_rect( real width )
{
  return fpoint(width, M_GOLDSECT * width);
}

fpoint viewrect::correct( const fpoint& maxviewrect, const fpoint& objrect )
{
  fpoint res;
  res.x() = maxviewrect.x();
  res.y() = res.x() * objrect.y() / objrect.x();
  if( res.y() > maxviewrect.y() ){
    res.y() = maxviewrect.y();
    res.x() = res.y() * objrect.x() / objrect.y();
  }
  return res;
}

fpoint viewrect::correct( real width, const fpoint& objrect )
{
  fpoint maxviewrect = gold_rect(width);
  return correct(maxviewrect, objrect);
}

real viewgrids::round( real x )
{
  if( fabs(x) < REAL_EPS )
    fail_assert("incorrect argument");

  real sign = (x < 0.0) ? -1.0 : 1.0;
  x = fabs(x);
  real mul = (x >= 1.0) ? 0.1 : 10.0;
  int n = 0;
  while( x < 1.0 || x >= 10.0 ){
    x *= mul;
    ++n;
  }
  x = floor(x + 0.5);
  for( int j = 0; j < n; j++ )
    x /= mul;
  return sign * x;
}

real viewgrids::getgridstep(
  real obj_a, real obj_b,
  real view_a, real view_b,
  real view_step
)
{
  if( view_a == view_b )
    throw ex_sing();
  return round(scale::length(view_step, view_a, view_b, obj_a, obj_b));
}

real viewgrids::getfirstgrid( real obj_a, real step )
{
  if( obj_a >= 0.0 )
    return step * (floor(obj_a / step) + 1.0);
  else
    return step * (ceil(obj_a / step) + 1.0);
}

}; // namespace lwml
