#include "spden.h"
#include "fft.h"
#include "pwspest.h"
#include "sliding.h"

/*#lake:stop*/

namespace lwml {

spdenest::spdenest( int winlen, int uplen, int winshift )
  : _winlen(winlen), _uplen(uplen),
    _splen(up2pow2(winlen + uplen)),
    _winshift(winshift ? winshift : winlen/2),
    _len(_splen/2 + 1),
    _p(_len)
{
  _p.set_zero();
  _win_num = 0;
}

void spdenest::put( const vector& src )
{
  sliding sl(src, _winlen, _winshift);
  _win_num = sl.len();
  pwspest pw(_winlen + _uplen);
  for( int j = 0; j < _win_num; j++ ){
    pw.put(sl, j);
    for( int n = 0; n < _len; n++ )
      _p[n] += pw[n];
  }
  for( int n = 0; n < _len; n++ )
    _p[n] /= _win_num;
}

void spdenest::get( vector& dst ) const
{
  if( dst.len() < len() )
    fail_size(dst.len());
  for( int j = 0; j < len(); j++ )
    dst[j] = _p[j];
  for( int j = len(); j < dst.len(); j++ )
    dst[j] = 0.0;
}

// spacorr

spacorr::spacorr( const spdenest& src )
  : _splen(src._splen), _clen(src._uplen),
    _xr(_splen), _xi(_splen)
{
  _xr.set_zero();
}

void spacorr::operator()( const spdenest& sd )
{
  test_size2(sd._splen, _splen, sd._uplen, _clen);

  _xr[0] = sd[0];
  for( int j = 1; j < sd.len(); j++ )
    _xr[_splen-j] = _xr[j] = sd[j];
  _xi.set_zero();
  fft::cifft(_xr, _xi);
}

}; // namespace lwml
