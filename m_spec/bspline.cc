#include "bspline.h"

/*#lake:stop*/

namespace lwml {

void bspline::set( real p1, real p2, real p3, real p4 )
{
  _data[3] = (-p1 + 3.0 * p2 - 3.0 * p3 + p4) / 6.0;
  _data[2] = (p1 - 2.0 * p2 + p3) / 2.0;
  _data[1] = (-p1 + p3) / 2.0;
  _data[0] = (p1 + 4.0 * p2 + p3) / 6.0;
}

real bspline::calc( real x )
{
  return ((_data[3]*x + _data[2])*x + _data[1])*x + _data[0];
}

}; // namespace lwml
