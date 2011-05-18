#include "herst.h"

/*#lake:stop*/

namespace lwml {

herst::herst( int len )
  : _len(len), _lp(len), _ls(len)
{
}

void herst::put( const vector& x )
{
  for( int k = 0; k < _len; k++ ){
    int sh = k + 1;
    int num = x.len() - sh;
    real mid = 0.0;
    for( int j = 0; j < num; j++ ){
      real dx = x[j+sh] - x[j];
      mid += dx / num;
    }
    real disp = 0.0;
    for( int j = 0; j < num; j++ ){
      real dx = x[j+sh] - x[j];
      disp += fsqr(dx - mid) / num;
    }
    _lp[k] = log2(sh);
    _ls[k] = log2(sqrt(disp));
  }
}

}; // namespace lwml
