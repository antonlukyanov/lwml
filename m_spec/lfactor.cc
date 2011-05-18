#include "lfactor.h"

/*#lake:stop*/

namespace lwml {

lfactor::lfactor( double x, int maxlen )
  : _maxlen(maxlen), _mult(maxlen), _powr(maxlen)
{
  if( !ieeereal::is_exactint(x) || fmod(x, 1.0) != 0.0 || x < 2.0 )
    runtime("incorrect argument (%f)", x);

  _len = 0;
  factorize(x);
}

void lfactor::put( double d, int k )
{
  if( _len == _maxlen )
    fail_assert("too many divisors");
  _mult[_len] = d;
  _powr[_len] = k;
  _len++;
}

void lfactor::factorize( double x )
{
  double xx = x;
  for( double d = 2; d * d <= x; d += ((d==2)? 1 : 2) ){
    int k;
    for( k = 0; fmod(xx, d) == 0.0; k++ ) xx /= d;
    if( k != 0 ) put(d, k);
  }
  if( xx > 1 ) put(xx);
}

}; // namespace lwml
