#include "lms.h"
#include "matrix.h"

/*#lake:stop*/

namespace lwml {

real lmspoly::calc( const vector& x, const vector& y, polyn& c )
{
  test_size(x.len(), y.len());
  vector res(c.len());
  // формируем вектор правых частей \sum_j x_j^k y_j
  for( int k = 0; k < c.len(); k++ ){
    double s = 0.0;
    for( int j = 0; j < x.len(); j++ )
      s += intpow(x[j], k) * y[j];
    res[k] = s;
  }
  // формируем вектор степеней \sum_j x_j^k  k \in [0..2m-2]
  vector pwa(2 * c.len() - 1);
  for( int k = 0; k < 2 * c.len() - 1; k++ ){
    double s = 0.0;
    for( int j = 0; j < x.len(); j++ )
      s += intpow(x[j], k);
    pwa[k] = s;
  }
  // расписываем вектор степеней в матрицу
  matrix b(c.len(), c.len());
  for( int k = 0; k < c.len(); k++ )
    for( int j = 0; j < c.len(); j++ )
      b(k, j) = pwa[k+j];

  // собственно решение
  lud lu(b.str());
  lu.put(b, lud::SCALE);
  real relerr = lu.cond();
  lu.solve(res);

  for( int j = 0; j < res.len(); j++ )
    c[j] = res[j];

  return relerr;
}

lmsleq::lmsleq( int n )
  : _n(n), _a(n, n), _b(n), _lu(n)
{
}

real lmsleq::calc( vector& x, const matrix& a, const vector& b, const vector& p )
{
  int m = a.str();

  test_size(a.col(), _n);
  test_size(x.len(), _n);
  test_size(b.len(), m);
  test_size(p.len(), m);

  for( int j = 0; j < _n; ++j ){
    for( int k = 0; k < _n; ++k ){
      real sum = 0.0;
      for( int i = 0; i < m; ++i )
        sum += p[i] * a(i, j) * a(i, k);
      _a(j, k) = sum;
    }
  }

  for( int k = 0; k < _n; ++k ){
    real sum = 0.0;
    for( int i = 0; i < m; ++i )
      sum += p[i] * a(i, k) * b[i];
    _b[k] = sum;
  }

  _lu.put(_a, lud::SCALE);
  real relerr = _lu.cond();
  _lu.solve(_b);

  for( int j = 0; j < _n; ++j )
    x[j] = _b[j];

  return relerr;
}

real lmsleq::calc( vector& x, const matrix& a, const vector& b )
{
  int m = a.str();

  test_size(a.col(), _n);
  test_size(x.len(), _n);
  test_size(b.len(), m);

  for( int j = 0; j < _n; ++j ){
    for( int k = 0; k < _n; ++k ){
      real sum = 0.0;
      for( int i = 0; i < m; ++i )
        sum += a(i, j) * a(i, k);
      _a(j, k) = sum;
    }
  }

  for( int k = 0; k < _n; ++k ){
    real sum = 0.0;
    for( int i = 0; i < m; ++i )
      sum += a(i, k) * b[i];
    _b[k] = sum;
  }

  _lu.put(_a, lud::SCALE);
  real relerr = _lu.cond();
  _lu.solve(_b);

  for( int j = 0; j < _n; ++j )
    x[j] = _b[j];

  return relerr;
}

}; // namespace lwml
