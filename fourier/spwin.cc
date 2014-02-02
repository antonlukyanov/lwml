#include "spwin.h"

/*#lake:stop*/

namespace lwml {

#define _SQRT_8_3_   1.632993161855   // sqrt(8/3)=1.632993161855

real spwin::asymgannw( real n, real len, normalization norm )
{
  real val = (1.0 - cos(2.0 * M_PI * n / len)) / 2.0;
  real nc = ((norm == NORMALIZED) ? _SQRT_8_3_ : 1.0);
  return (n < 0 || n > len) ? 0.0 : val * nc;
}

real spwin::symgannw( real n, real len )
{
  real val = (1.0 + cos(M_PI * n / len)) / 2.0;
  return (n < -len || n > len) ? 0.0 : val;
}

}; // namespace lwml
