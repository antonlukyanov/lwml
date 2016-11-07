#include "lwml/m_alg/regr.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/base/basex.h"

/*#lake:stop*/

namespace lwml {

void lregr::put( const vector& x, const vector& y )
{
  test_size(x.len(), y.len());

  int n = x.len();
  real sx = x.mid();
  real sy = y.mid();
  real sxy = 0.0;
  real sxx = 0.0;
  for( int i = 0; i < n; i++ ){
    sxy += (x[i] - sx) * (y[i] - sy);
    sxx += fsqr(x[i] - sx);
  }
  _k = sxy / sxx;
  _c = sy - (sxy/sxx) * sx;
}

//    s2   - сумма квадратов уклонений
//    err  - коэффициент перед квантилем Стьюдента (n-2 степеней свободы)
//           в оценке доверительного интервала дл параметра k
// *s2 = ( syy - sxy*sxy / sxx );
// ss2 = real_cast(*s2) / real_cast(n-2);
// *err = sqrt( ss2 / sxx );

real lregr::midw( const vector& x, const vector& w, real ws )
{
  double sum = 0.0;
  for( int j = 0; j < x.len(); j++ )
    sum += w[j] * x[j];
  return sum / ws;
}

void lregr::put( const vector& x, const vector& y, const vector& w )
{
  test_size2(y.len(), x.len(), w.len(), x.len());

  int n = x.len();
  real ws = w.inner_sum();
  real sx = midw(x, w, ws);
  real sy = midw(y, w, ws);
  real sxy = 0.0;
  real sxx = 0.0;
  for( int i = 0; i < n; i++ ){
    sxy += w[i] * (x[i] - sx) * (y[i] - sy);
    sxx += w[i] * fsqr(x[i] - sx);
  }
  _k = sxy / sxx;
  _c = sy - (sxy/sxx) * sx;
}

}; // namespace lwml
