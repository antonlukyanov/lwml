#include "lwml/wavelets/cwt.h"
#include "lwml/fourier/fft.h"

/*#build_stop*/

namespace lwml {

// dog_gauss

cmplx dog_gaussian::td_func( real t ) const
{
  return cmplx(g0(t));
}

cmplx dog_gaussian::ft_func( real f ) const
{
  return cmplx(sqrt(2.0*M_PI) * g0(2.0*M_PI*f));
}

real dog_gaussian::get_const() const
{
  return 1.0;
}

// dog_wave

cmplx dog_wave::td_func( real t ) const
{
  return cmplx(t * g0(t));
}

cmplx dog_wave::ft_func( real f ) const
{
  return cmplx(0.0, -(2.0*M_PI*f) * sqrt(2.0*M_PI) * g0(2.0*M_PI*f));
}

real dog_wave::get_const() const
{
  return sqrt(1.0 / 2.0) / (2.0*M_PI);
}

// dog_mhat

cmplx dog_mhat::td_func( real t ) const
{
  return cmplx((fsqr(t) - 1.0) * g0(t));
}

cmplx dog_mhat::ft_func( real f ) const
{
  return cmplx(-fsqr(2.0*M_PI*f) * sqrt(2*M_PI) * g0(2.0*M_PI*f));
}

real dog_mhat::get_const() const
{
  return sqrt(3.0 / 2.0) / (2.0*M_PI);
}

// morlet

cmplx morlet::td_func( real t ) const
{
  return cmplx::iexp(2.0 * M_PI * _f0 * t) * g0(t);
}

cmplx morlet::ft_func( real f ) const
{
  return cmplx(sqrt(2*M_PI) * g0(2.0*M_PI*(f - _f0)));
}

real morlet::get_const() const
{
  real om0 = 2.0 * M_PI * _f0;
  return (om0 + sqrt(fsqr(om0)-2.0)) / 2.0 / (2.0*M_PI);
}

// cwt

int cwt::calc_scales_num( real min_sc, real max_sc, int octave )
{
  return int_cast(octave * log2(max_sc / min_sc) + 0.5);
}

real cwt::idx2scale( real min_sc, int octave, int idx )
{
  return min_sc * pow(2.0, real_cast(idx) / real_cast(octave));
}

namespace {
  inline real idx2freq( int idx, int len2 )
  {
    return (idx <= len2/2) ? real_cast(idx)/len2 : real_cast(idx-len2)/len2;
  }
};

void cwt::simple_cwt( vector& hr, vector& hi, const vector& x, const i_wavelet& wav, real sc )
{
  TEST_SIZE(hr.len(), hi.len());
  TEST_SIZE(hr.len(), x.len());
  for( int k = 0; k < hr.len(); k++ ){
    cmplx res(0.0, 0.0);
    for( int j = 0; j < x.len(); j++ )
      res += wav.td_func((j-k)/sc).conj() * x[j];
    hr[k] = res.re() / sqrt(sc);
    hi[k] = res.im() / sqrt(sc);
  }
}

void cwt::fcwt( vector& hr, vector& hi, vector& xftr, vector& xfti, const i_wavelet& wav, real sc )
{
  TEST_SIZE(xftr.len(), xfti.len());
  TEST_SIZE(hr.len(), hi.len());
  TEST_SIZE(hr.len(), xftr.len());
  int nn = hr.len();

  for( int n = 0; n < nn; n++ ){
    real f = idx2freq(n, nn);
    cmplx c = wav.ft_func(sc * f).conj() * cmplx(xftr[n], xfti[n]);
    hr[n] = sqrt(sc) * c.re();
    hi[n] = sqrt(sc) * c.im();
  }
  fft::cifft(hr, hi);
}

}; // namespace lwml
