#include "cwt_drv.h"

#include "fft.h"

/*#build_stop*/

namespace lwml {

int cwt_drv::calc_scales_num( const i_wavelet& wav, real min_t, real max_t, int oct )
{
  real _wconst = wav.get_const();
  return cwt::calc_scales_num(_wconst * min_t, _wconst * max_t, oct);
}

cwt_drv::cwt_drv( const i_wavelet& wav, real min_t, real max_t, int oct, int len )
  : _wtr(calc_scales_num(wav, min_t, max_t, oct), len),
    _wti(calc_scales_num(wav, min_t, max_t, oct), len)
{
  _wconst = wav.get_const();
  _min_t = min_t;
  _max_t = max_t;
  _octave = oct;
  _len = len;

  _min_k = _wconst * _min_t;
  _max_k = _wconst * _max_t;
  _scales = cwt::calc_scales_num(_min_k, _max_k, _octave);

//  _cg = 1.797; //!!
//  _cg = 3.142;
//  _cg = M_PI; // last value
}

real cwt_drv::get_power( int k, int j ) const
{
  real sc = idx2scale(k);
  return fsqr(hypot(_wtr(k, j), _wti(k, j)) / sc);
}

void cwt_drv::get_power( matrix& ps ) const
{
  TEST_SIZE2(ps.str(), _scales, ps.col(), _len);

  for( int k = 0; k < _scales; k++ ){
    real sc = idx2scale(k);
    for( int j = 0; j < _len; j++ )
      ps(k, j) = fsqr(hypot(_wtr(k, j), _wti(k, j)) / sc);
  }
}

void cwt_drv::get_power( vector& ps ) const
{
  TEST_SIZE(ps.len(), _scales);

  ps.set_zero();
  for( int k = 0; k < _scales; k++ ){
    real sc = idx2scale(k);
    for( int j = 0; j < _len; j++ )
      ps[k] += fsqr(hypot(_wtr(k, j), _wti(k, j)) / sc);
  }
}

void cwt_drv::tran( const i_wavelet& wav, const vector& data )
{
  TEST_SIZE(data.len(), _len);
  if( _scales != calc_scales_num(wav, _min_t, _max_t, _octave) )
    fail_assert("inconsistent wavelet type");

  int len2 = 2 * up2pow2(_len);

  vector dataftr(len2);
  vector datafti(len2);
  dataftr.set_zero();
  for( int j = 0; j < data.len(); j++ )
    dataftr[j] = data[j];
  datafti.set_zero();
  fft::cfft(dataftr, datafti);
  vector vwtr(len2);
  vector vwti(len2);

  //progress ind( "wavelet transform", _scales );
  for( int k = 0; k < _scales; k++ ){
    real sc = cwt::idx2scale(_min_k, _octave, k);
    //ind( k );
    cwt::fcwt(vwtr, vwti, dataftr, datafti, wav, sc);
    for( int j = 0; j < data.len(); j++ ){
      _wtr(k,j) = vwtr[j];
      _wti(k,j) = vwti[j];
    }
  }
}

/*
void cwt_drv::invtran( vector& data )
{
  int n = _length2;
  vector wtftr(n);
  vector wtfti(n);
  vector dataftr(n);
  vector datafti(n);

  dataftr.set_zero();
  datafti.set_zero();
  for( int k = 0; k < _scales; k++ ){
    real rk = idx2scale( k );

    wtftr.set_zero();
    wtfti.set_zero();
    int j;
    for( j = 0; j < _len; j++ ){
      wtftr[j] = _wtr(k, j);
      wtfti[j] = _wti(k, j);
    }
    fft::cfft(wtftr, wtfti);
    for( j = 0; j < n; j++ ){
      real om = wavelets::idx2om(j, rk, n);
      cmplx c = _wft(om, _par) * cmplx(wtftr[j], wtfti[j]);
      dataftr[j] += c.re()/ sqrt(rk);
      datafti[j] += c.im() / sqrt(rk);
    }
  }
  real dj2 = 1.0 / _scales / 2.0;
  real dpow = pow(2.0, dj2) - pow(0.5, dj2);
  dataftr *= 2.0 * M_PI * dpow / _cg;
  datafti *= 2.0 * M_PI * dpow / _cg;
  fft::cifft(dataftr, datafti);

  for( int j = 0; j < _len; j++ )
    data[j] = dataftr[j];
}
*/

}; // namespace lwml
