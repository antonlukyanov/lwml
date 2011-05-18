#include "gj3d.h"

/*#lake:stop*/

namespace lwml {

gj3d::gj3d( int sz )
  : _size(sz), _aa(sz-1), _bb(sz-1)
{
}

void gj3d::calc(
  vector& x, const vector& ld, const vector& dg, const vector& ud, const vector& dd
)
{
  test_size(dd.len(), _size);
  test_size2(ld.len(), _size-1, ud.len(), _size-1);

  if( dg[0] == 0 )
    throw ex_sing();
  _aa[0] = -ud[0] / dg[0];
  _bb[0] =  dd[0] / dg[0];
  for( int j = 1; j < _size - 1; j++ ){
    real e = ld[j-1] * _aa[j-1] + dg[j];
    if( e == 0 )
      throw ex_sing();
    _aa[j] = -ud[j] / e;
    _bb[j] = (dd[j] - ld[j-1] * _bb[j-1]) / e;
   }

  real e = dg[_size-1] + ld[_size-2] * _aa[_size-2];
  if( e == 0 )
    throw ex_sing();
  x[_size-1] = (dd[_size-1] - ld[_size-2] * _bb[_size-2]) / e;
  for( int j = _size - 2; j >= 0; j-- )
    x[j] = _aa[j] * x[j+1] + _bb[j];
}

}; // namespace lwml
