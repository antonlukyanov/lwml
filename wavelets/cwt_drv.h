// CWT driver
// lwml, (c) ltwood

#ifndef _CWT_DRV_
#define _CWT_DRV_

#include "defs.h"
#include "mdefs.h"

#include "cwt.h"

/*#build_stop*/

namespace lwml {

class cwt_drv {
public:
  cwt_drv( const i_wavelet& wav, real min_t, real max_t, int oct, int len );

  real idx2scale( int idx ) const { return cwt::idx2scale(_min_k, _octave, idx); }
  real idx2period( int idx ) const { return idx2scale(idx) / _wconst; }
  real idx2size( int idx ) const { return idx2period(idx) / 2.0; }

  real get_power( int k, int j ) const;
  void get_power( matrix& ps ) const;
  void get_power( vector& ps ) const;

  void tran( const i_wavelet& wav, const vector& data );

  int scales() const { return _scales; }
  int len() const { return _len; }
  real min_t() const { return _min_t; }
  real max_t() const { return _max_t; }
  real min_k() const { return _min_k; }
  real max_k() const { return _max_k; }

private:
  matrix _wtr, _wti;

  real _wconst;
  real _min_t, _max_t;
  int _octave;
  int _len;

  real _min_k, _max_k;
  int _scales;

  int calc_scales_num( const i_wavelet& wav, real mint, real maxt, int oct );
};

}; // namespace lwml

#endif // _CWT_DRV_
