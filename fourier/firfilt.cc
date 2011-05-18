#include "firfilt.h"
#include "fft.h"
#include "mdefs.h"
#include "basex.h"
#include "stdmem.h"
#include "spwin.h"
#include "fft.h"
#include "progress.h"

/*#lake:stop*/

namespace lwml {

class filtgen : public scope {
public:
  static real lp( real j, real f ){
    if( j == 0 )
      return 2.0 * f;
    else
      return sin(2.0 * M_PI * f * j) / (M_PI * j);
  }

  static real hp( real j, real f ){
    if( j == 0 )
      return 1.0 - 2.0 * f;
    else
      return -sin(2.0 * M_PI * f * j) / (M_PI * j);
  }

  static real bp( real j, real f1, real f2 ){
    if( j == 0 )
      return 2.0 * (f2-f1);
    else
      return (sin(2.0 * M_PI * f2 * j)-sin(2.0 * M_PI * f1 * j)) / (M_PI * j);
  }

  static real nt( real j, real f1, real f2 ){
    if( j == 0 )
      return 1.0 - 2.0 * (f2-f1);
    else
      return -(sin(2.0 * M_PI * f2 * j)-sin(2.0 * M_PI * f1 * j)) / (M_PI * j);
  }

  static real get( fir_filter::filter_type ft, real j, real f1, real f2 );
};

real filtgen::get( fir_filter::filter_type ft, real j, real f1, real f2 )
{
  real val = 0.0; // has no mean!
  switch( ft ){
    case fir_filter::LP_FILTER:
      val = lp(j, f1);
      break;
    case fir_filter::HP_FILTER:
      val = hp(j, f1);
      break;
    case fir_filter::BP_FILTER:
      val = bp(j, f1, f2);
      break;
    case fir_filter::NT_FILTER:
      val = nt(j, f1, f2);
      break;
    default:
      fail_assert("unknown filter type");
  }
  return val;
}

// fir_filter

fir_filter::fir_filter( const vector& filt )
  : _coeff(filt.len())
{
  _coeff = filt;
}

fir_filter::fir_filter( const i_function& sp, int len, int lenmul )
  : _coeff(len)
{
  if( lenmul < 4 )
    fail_assert("too small multiplier for fir_filter");
  int suplen = up2pow2(lenmul * len);
  vector hr(suplen), hi(suplen);
  real df = 1.0 / suplen;
  for( int n = 0; n < suplen; n++ )
    hr[n] = sp(fabs(n * df - 0.5)) / suplen;
  hi.set_zero();
  fft::cifft(hr, hi);
  fft::disturb(hr);
  for( int j = 0; j < len; j++ )
    _coeff[j] = hr[j] * spwin::symgannw(j, len);
}

fir_filter::fir_filter( filter_type ft, int len, real f1, real f2 )
  : _coeff(len)
{
  for( int j = 0; j < len; j++ )
    _coeff[j] = filtgen::get(ft, j, f1, f2) * spwin::symgannw(j, len);
}

real fir_filter::sp( real f )
{
  double sum = _coeff[0];
  for( int k = 1; k < _coeff.len(); k++ )
    sum += 2.0 * _coeff[k] * cos(2.0 * M_PI * f * k);
  return sum;
}

real fir_filter::mag()
{
  double sum = fsqr(_coeff[0]);
  for( int k = 1; k < _coeff.len(); k++ )
    sum += 2.0 * fsqr(_coeff[k]);
  return sum;
}

void fir_filter::apply( vector& dst, const vector& src, tick_mode tick )
{
  test_size(dst.len(), src.len());

  if( tick == tmON )
    progress::start("fir_filter", dst.len());
  for( int k = 0; k < dst.len(); k++ ){
    real res = src[k] * _coeff[0];
    for( int j = 1; j < _coeff.len(); j++ ){
      if( k - j >= 0 )
        res += src[k-j] * _coeff[j];
      if( k + j < src.len() )
        res += src[k+j] * _coeff[j];
    }
    dst[k] = res;
    if( tick == tmON )
      progress::up(k);
  }
  if( tick == tmON )
    progress::finish();
}

}; // namespace lwml
