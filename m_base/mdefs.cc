#include "mdefs.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

// common funcs

real real_nan()
{
  static const char nan_repr[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff };

  typedef const char* cpchar_aliased __attribute__((__may_alias__));
  cpchar_aliased nan_repr_cpca = nan_repr;

  return *(double*)nan_repr_cpca;
}

bool is_nan( real xx )
{
  return xx != xx;
}

void nan_filling( real* mem, int sz )
{
  real nan = real_nan();
  for( int j = 0; j < sz; j++ )
    mem[j] = nan;
}

real intpow( real x, int n )
{
  if( n == 0 )
    return 1.0;

  if( n < 0 ){
    n = -n;
    x = 1.0 / x;
  }

  if( x == 0 )
    return 0.0;

  real res = 1.0;
  while( n != 0 ){
    if( n & 1 )
      res *= x;
    n /= 2;
    x *= x;
  }
  return res;
}

int intlog2( int x )
{
//  return log2( n ) + 0.5;
  int res;
  for( res = 0; x != 1; res++ )
    x /= 2;
  return res;
}

int up2pow2( int x )
{
// incorrect old version
//  int lg2 = log2(x-1)+1;
//  return intpow( 2, lg2 );

  int res = 1;
  while( res < x )
    res *= 2;
  return res;
}

// intervals

int intervals::val2idx( real x, real ax, real bx, int len )
{
  int idx = int_cast(len * (x - ax) / (bx - ax));
  if( x < ax || bx < x )
    runtime("argument (%f) out of interval (%f, %f)", x, ax, bx);
  return (idx == len) ? len - 1 : idx;
}

real intervals::idx2val( int idx, real ax, real bx, int len )
{
  if( idx < 0 || idx > len - 1 )
    runtime("argument (%d) out of interval (%d, %d)", idx, 0, len-1);
  return ax + real_cast(idx) * ( bx - ax ) / real_cast(len);
}

real intervals::idx2cnt( int idx, real ax, real bx, int len )
{
  if( idx < 0 || idx > len - 1 )
    runtime("argument (%d) out of interval (%d, %d)", idx, 0, len-1);
  return ax + (real_cast(idx) + 0.5) * (bx - ax) / real_cast(len);
}

// IEEE

void ieeereal::set( real x )
{
  _mant = frexp( x, &_expn );
}

void ieeereal::set( real mnt, int exp )
{
  _mant = mnt;
  _expn = exp;
}

real ieeereal::get()
{
  return ldexp(_mant, _expn);
}

}; // namespace lwml
