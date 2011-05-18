#include "randcomb.h"

/*#lake:stop*/

namespace lwml {

// randperm

randperm::randperm( int len, uint seed )
  : _len(len), _data(len), _rnd(seed)
{
  for( int k = 0; k < _len; k++ )
    _data[k] = k;
  next();
}

void randperm::swap( int i, int j )
{
  _data.swap( i, j );
}

void randperm::next()
{
  for( int j = _len-1; j > 0; j-- ){
    real u = _rnd.get_real();
    if( !(0.0 <= u && u < 1.0) )
      fail_unexpected();
    int k = (int)( (j+1) * u );
    swap( k, j );
  }
}

// randselect

randselect::randselect( int len, int range, uint seed )
  : _len(len), _range(range), _data(len), _rnd(seed)
{
  next();
}

void randselect::next()
{
  int m = 0;
  for( int t = 0; m < _len && t < _range; t++ ){
    real u = _rnd.get_real();
    if( !( 0.0 <= u && u < 1.0 ) )
      fail_unexpected();
    if( (_range - t) * u < _len - m )
      _data[m++] = t;
  }
  if( m < _len )
    fail_unexpected();
}

}; // namespace lwml
