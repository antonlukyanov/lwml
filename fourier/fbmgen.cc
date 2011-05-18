#include "fbmgen.h"
#include "fft.h"

/*#lake:stop*/

namespace lwml {

fbmgen::fbmgen( int len, uint seed )
  : _len(up2pow2(len)),
    _xr(up2pow2(len)), _xi(up2pow2(len)),
    _rnd(seed)
{
  generate();
}

void fbmgen::generate( real h )
{
  _xr[0] = _rnd.get_norm();
  _xi[0] = 0.0;
  for( int j = 1; j < _len/2; j++ ){
    real am = _rnd.get_norm() / pow(real_cast(j), h+0.5);
    real ph = 2.0 * M_PI * _rnd.get_real();
    _xr[_len-j] = _xr[j] = am * cos(ph);
    _xi[_len-j] = -(_xi[j] = am * sin(ph));
  }
  _xr[_len/2] = _rnd.get_norm() / pow(real_cast(_len/2), h+0.5);
  _xi[_len/2] = 0.0;
  fft::cifft(_xr, _xi);
}

void fbmgen::get( vector& dst )
{
  if( dst.len() > _len )
    fail_size(dst.len());
  for( int j = 0; j < dst.len(); j++ )
    dst[j] = _xr[j];
}

}; // namespace lwml
