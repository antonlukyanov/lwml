#include "lwml/fourier/convolv.h"
#include "lwml/fourier/fft.h"

/*#lake:stop*/

namespace lwml {

convolver::convolver( int len, const vector& hp, const vector& hn )
  : _uplen(t_max<int>(hp.len()-1, hn.len())),
    _dlen(len),
    _splen(up2pow2(len + _uplen)),
    _hr(_splen), _hi(_splen), _xr(_splen), _xi(_splen)
{
  _hr.set_zero();
  _hi.set_zero();
  for( int j = 0; j < hp.len(); j++ )
    _hr[j] = hp[j];
  for( int j = 0; j < hn.len(); j++ )
    _hr[_hr.len()-1-j] = hn[j];
  fft::cfft(_hr, _hi);
}

convolver::convolver( int len, const vector& hp )
  : _uplen(hp.len()-1),
    _dlen(len),
    _splen(up2pow2(len + _uplen)),
    _hr(_splen), _hi(_splen), _xr(_splen), _xi(_splen)
{
  _hr.set_zero();
  _hi.set_zero();
  _hr[0] = hp[0];
  for( int j = 1; j < hp.len(); j++ )
    _hr[_hr.len()-j] = _hr[j] = hp[j];
  fft::cfft(_hr, _hi);
}

void convolver::operator()( vector& dst, const vector& src )
{
  test_size(dst.len(), src.len());
  if( src.len() > _dlen )
    fail_size(src.len());

  _xr.set_zero();
  _xi.set_zero();
  for( int j = 0; j < src.len(); j++ )
    _xr[j] = src[j] * _splen; // убираем делитель N в циклической свертке
  fft::cfft(_xr, _xi);
  for( int j = 0; j < _splen; j++ ){
    cmplx y = cmplx(_xr[j], _xi[j]) * cmplx(_hr[j], _hi[j]);
    _xr[j] = y.re();
    _xi[j] = y.im();
  }
  fft::cifft(_xr, _xi);
  for( int j = 0; j < dst.len(); j++ )
    dst[j] = _xr[j];
}

// crosscorr

crosscorr::crosscorr( int len, int clen )
  : _dlen(len), _clen(clen), _splen(up2pow2(len+clen)),
    _xr(_splen), _xi(_splen), _xxr(_splen), _xxi(_splen)
{
  _xr.set_zero();
}

void crosscorr::operator()( const vector& src1, const vector& src2 )
{
  if( src1.len() > _dlen )
    fail_size(src1.len());
  if( src2.len() > _dlen )
    fail_size(src2.len());

  _xr.set_zero();
  real mid1 = src1.mid();
  for( int j = 0; j < src1.len(); j++ )
    _xr[j] = (src1[j] - mid1) * fsqrt(_splen); // убираем делитель N в ККФ
  _xi.set_zero();
  fft::cfft(_xr, _xi);

  _xxr.set_zero();
  real mid2 = src2.mid();
  for( int j = 0; j < src2.len(); j++ )
    _xxr[j] = (src2[j] - mid2) * fsqrt(_splen); // убираем делитель N в ККФ
  _xxi.set_zero();
  fft::cfft(_xxr, _xxi);

  for( int j = 0; j < _splen; j++ ){
    cmplx y = cmplx(_xr[j], -_xi[j]) * cmplx(_xxr[j], _xxi[j]);
    _xr[j] = y.re();
    _xi[j] = y.im();
  }
  fft::cifft(_xr, _xi);
}

// autocorr

autocorr::autocorr( int len, int clen )
  : _dlen(len), _clen(clen), _splen(up2pow2(len+clen)),
    _xr(_splen), _xi(_splen)
{
  _xr.set_zero();
}

void autocorr::operator()( const vector& src )
{
  if( src.len() > _dlen )
    fail_size(src.len());

  _xr.set_zero();
  real mid1 = src.mid();
  for( int j = 0; j < src.len(); j++ ) // убираем делитель N в ККФ
    _xr[j] = (src[j] - mid1) * fsqrt(_splen) / fsqrt(src.len());
  _xi.set_zero();
  fft::cfft(_xr, _xi);

  for( int j = 0; j < _splen; j++ ){
    cmplx y = cmplx(_xr[j], _xi[j]);
    _xr[j] = y.mag();
  }
  _xi.set_zero();
  fft::cifft(_xr, _xi);
}

}; // namespace lwml
