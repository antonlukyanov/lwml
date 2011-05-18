// Fast integral wavelet transform
// lwml, (c) ltwood

#ifndef _CWT_
#define _CWT_

#include "defs.h"
#include "mdefs.h"

#include "cmplx.h"
#include "vector.h"
#include "matrix.h"

/*#build_stop*/

namespace lwml {

// i_wavelet

class i_wavelet : public interface {
public:
  // time domain wavelet function
  virtual cmplx td_func( real t ) const = 0;

  // fourier transform of wavelet function
  virtual cmplx ft_func( real f ) const = 0;

  // wavelet constant (\al_g)
  // sc = \al_g / f = \al_g T
  virtual real get_const() const = 0;

protected:
  real g0( real t ) const { return exp(-fsqr(t) / 2.0); }
};

// GAUSS DOG function (g0)

class dog_gaussian : public i_wavelet {
public:
  virtual cmplx td_func( real t ) const;
  virtual cmplx ft_func( real f ) const;
  virtual real get_const() const;
};

// WAVE DOG wavelet (g1)

class dog_wave : public i_wavelet {
public:
  virtual cmplx td_func( real t ) const;
  virtual cmplx ft_func( real f ) const;
  virtual real get_const() const;
};

// MHAT DOG wavelet (g2)

class dog_mhat : public i_wavelet {
public:
  virtual cmplx td_func( real t ) const;
  virtual cmplx ft_func( real f ) const;
  virtual real get_const() const;
};

// MORLET wavelet

class morlet : public i_wavelet {
public:
  morlet( real f0 ) : _f0(f0) {}

  virtual cmplx td_func( real t ) const;
  virtual cmplx ft_func( real f ) const;
  virtual real get_const() const;

private:
  real _f0;
};

class cwt {
public:
  // Вычисление количества отсчетов по оси масштабов начиная с масштаба min_sc
  // и заканчивая масштабом max_sc, если на каждое удвоение масштаба (октаву)
  // приходится octave отсчетов.
  static int calc_scales_num( real min_sc, real max_sc, int octave );

  // Преобразование номера отсчета idx по оси масштабов в значение масштаба
  // при заданном минимальном масштабе min_sc и количестве отсчетов в октаве octave.
  static real idx2scale( real min_sc, int octave, int idx );

  // simple complex wavelet transform:
  //   hr, hi     - wavelet transform
  //   x          - source data
  //   wav        - wavelet function
  //   sc         - level
  static void simple_cwt( vector& hr, vector& hi, const vector& x, const i_wavelet& wav, real sc );

  // fast complex wavelet transform
  // hr, hi     - wavelet transform
  // xftr, xfti - fourier transform of source data
  // wft        - fourier transform of wavelet
  // par        - wavelet parameter
  // sc         - level
  static void fcwt( vector& hr, vector& hi, vector& xftr, vector& xfti, const i_wavelet& wav, real sc );
};

}; // namespace lwml

#endif // _CWT_
