#include "lwml/random/frand.h"

/*#lake:stop*/

namespace lwml {

frand::frand( uint seed )
{
  srand(seed);
  for( int j = 0; j < SH0; j++ ){
    _buf[j] = 0.0;
    real val = 0.5;
    for( int k = 0; k < REAL_BITS; k++, val /= 2.0 )
      _buf[j] += (rand() > RAND_MAX/2) ? 0.0 : val;
  }
  _index = 0;

  for( int j = 0; j < SHUFFLE_BOX_SIZE; j++ )
    _sht[j] = get();
  _prev = _sht[0];
}

real frand::get_norm()
{
  real v1, v2, s, m, x1; // x2

  do{
    v1 = 2 * get_real() - 1;
    v2 = 2 * get_real() - 1;
    s = v1 * v1 + v2 * v2;
  }while( s > 1.0 );

  m = sqrt(-2.0 * log(s) / s);
  x1 = v1 * m;
  // x2 = v2 * m;
  return x1;
}

}; // namespace lwml
