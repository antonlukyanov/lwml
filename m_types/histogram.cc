#include "lwml/m_types/histogram.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/m_base/statfn.h"
#include "lwml/base/basex.h"

/*#lake:stop*/

namespace lwml {

histogram::histogram( int len, real ax, real bx )
  : _ax(ax), _bx(bx), _data(len, 0), _total(0)
{
}

void histogram::clear()
{
  _data.set_val(0);
  _total = 0;
}

int histogram::val2idx( real val ) const
{
  int idx = intervals::val2idx(val, _ax, _bx, _data.len());
  test_index(idx, len());
  return idx;
}

real histogram::idx2val( int idx ) const
{
  test_index(idx, len());
  return intervals::idx2val( idx, _ax, _bx, _data.len() );
}

real histogram::idx2cnt( int idx ) const
{
  test_index(idx, len());
  return intervals::idx2cnt( idx, _ax, _bx, _data.len() );
}

histogram& histogram::put( real x )
{
  _data[val2idx(x)]++;
  _total++;
  return *this;
}

int histogram::max_idx() const
{
  return _data.max_idx();
}

int histogram::max_val() const
{
  return _data.max();
}

void histogram::set_integral( int_vector& idx )
{
  test_size(idx.len(), len());
  idx[0] = _data[0];
  for( int j = 1; j < len(); j++ )
    idx[j] = idx[j-1] + _data[j];
}

// chisq: вычисление статистики хи-квадрат дл равномерного распределени
// n - общее число шаров
// m - число луз
// p_j - веротность i-й лузы
// n_j - число шаров в i-й лузе
// r_j - частота i-й лузы (r_i=n_i/n)
// S = \sum_{j=1}^m \frac{(n_j - n p_j)^2}{n p_j} =
//     n \sum_{j=1}^m \frac{(n_j/n - p_j)^2}{p_j} =
//     n \sum_{j=1}^m \frac{(r_j - p_j)^2}{p_j}

real histogram::chisq_val()
{
  real pi = 1.0 / real_cast(len());
  real sum = 0.0;
  for( int j = 0; j < len(); j++ ){
    real ri = real_cast(_data[j]) / real_cast(_total);
    sum += fsqr(ri - pi) / pi;
  }
  return _total * sum;
}

real histogram::chisq_prob()
{
  return statfn::chi2(chisq_val(), len() - 1);
}

int histogram::chisq_susp()
{
  real cp = chisq_prob();
  if( cp < 0.01 || cp > 0.99 )
    return 3;
  else if( cp < 0.05 || cp > 0.95 )
    return 2;
  else if( cp < 0.1 || cp > 0.9 )
    return 1;
  else
    return 0;
}

}; // namespace lwml
