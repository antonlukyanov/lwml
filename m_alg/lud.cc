#include "lwml/m_alg/lud.h"

/*#lake:stop*/

namespace lwml {

void lud::set_hilbert( matrix& m )
{
  test_size(m.str(), m.col());
  for( int i = 0; i < m.str(); i++ )
    for( int j = 0; j < m.col(); j++ )
      m(i, j) = 1.0 / (i + j + 1.0);
}

void lud::set_test()
{
  for( int k = 0; k < _data.str(); k++ ){
    _tst[k] = 0.0;
    for( int j = 0; j < _data.col(); j++ )
      _tst[k] += _data(k, j);
  }
}

void lud::set_scale( bool do_scale )
{
  if( !do_scale ){
    _scale.set_val(1.0);
    return;
  }
  for( int k = 0; k < _data.str(); k++ ){
    real max = 0.0;
    for( int j = 0; j < _data.col(); j++ ){
      real cur = fabs(_data(k, j));
      if( cur > max ) max = cur;
    }
    _scale[k] = max;
    if( max == 0.0 )
      throw ex_sing();
    for( int j = 0; j < _data.col(); j++ )
      _data(k, j) /= max;
  }
}

lud::lud( int size )
  : _data(size, size), _scale(size), _tst(size), perm(size-1)
{
  _data.set_diag(1.0);
  _scale.set_val(1.0);
  permsgn = 1;
  _tst.set_val(1.0);
  for( int j = 0; j < size-1; j++ )
    perm[j] = j;
}

void lud::put( const matrix& m, scaling sc )
{
  test_size2(m.str(), m.col(), m.str(), _data.str());

  _data = m;         // копируем матрицу
  set_test();        // устанавливаем тестовый вектор
  set_scale(sc == SCALE);  // посчитать масштабы
  permsgn = 1;       // пока что перестановок нет
  gjdc();            // факторизуем матрицу
  solve(_tst);       // решаем систему с тестовым вектором
  _src_norm = m.norm_manh(); // сохраняем значение нормы исходной матрицы
}

void lud::gjdc()
{
  for( int k = 0; k < _data.str()-1; k++ ){
    int m = k;
    for( int i = k+1; i < _data.str(); i++ )
      if( fabs(_data(i, k)) > fabs(_data(m, k)) ) m = i;
    if( _data(m, k) == 0.0 )
      throw ex_sing();
    perm[k] = m;
    if( m != k ) permsgn = -permsgn;
    _data.swap(k, m);
    for( int i = k+1; i < _data.str(); i++ ){
      real v = _data(i, k) / _data(k, k);
      _data(i, k) = v;
      for( int j = k+1; j < _data.col(); j++ )
        _data(i, j) -= v * _data(k, j);
    }
  }
}

void lud::solve( vector& b ) const
{
  test_size(b.len(), _data.str());

  for( int k = 0; k < b.len(); k++ )
    b[k] /= _scale[k];

  for( int k = 0; k < _data.str()-1; k++ )
    b.swap(k, perm[k]);

  for( int k = 0; k < _data.str()-1; k++ )
    for( int j = k+1; j < _data.str(); j++ )
      b[j] -= _data(j, k) * b[k];

  for( int k = b.len()-1; k > 0; k-- ){
    real diag = _data(k, k);
    if( diag == 0.0 )
      throw ex_sing();
    b[k] = b[k] / diag;
    for( int j = 0; j < k; j++ )
      b[j] -= _data(j, k) * b[k];
  }

  if( _data(0, 0) == 0.0 )
    throw ex_sing();
  b[0] = b[0] / _data(0, 0);
}

void lud::solve( matrix& b ) const
{
  test_size(b.str(), _data.str());

  for( int k = 0; k < b.str(); k++ )
    for( int j = 0; j < b.col(); j++ )
      b(k, j) /= _scale[k];

  for( int s = 0; s < b.col(); s++ ){
    for( int k = 0; k < _data.str()-1; k++ )
      b.swap(k, perm[k]);

    for( int k = 0; k < _data.str()-1; k++ )
      for( int j = k+1; j < _data.str(); j++ )
        b(j, s) -= _data(j, k) * b(k, s);

    for( int k = b.str()-1; k > 0; k-- ){
      real diag = _data(k, k);
      if( diag == 0.0 )
        throw ex_sing();
      b(k, s) = b(k, s) / diag;
      for( int j = 0; j < k; j++ )
        b(j, s) -= _data(j, k) * b(k, s);
    }

    if( _data(0, 0) == 0.0 )
      throw ex_sing();
    b(0, s) = b(0, s) / _data(0, 0);
  }
}

real lud::cond_est() const
{
  real max = 0.0;
  for( int k = 0; k < _data.str(); k++ ){
    real d = fabs(_tst[k] - 1.0);
    if( d > max ) max = d;
  }
  return 10.0 * (max / REAL_EPS) + 1.0; // эмпирические коэффициенты
}

real lud::cond() const
{
  matrix b(_data.str(), _data.col());
  b.set_diag(1.0);
  solve(b);
  return _src_norm * b.norm_manh();
}

real lud::det() const
{
  double res = 1.0;
  for( int k = 0; k < _data.str(); k++ )
    res *= _data(k, k);
  return res * permsgn;
}

real lud::logdet() const
{
  real res = 0.0;
  for( int k = 0; k < _data.str(); k++ )
    res += log(fabs(_data(k, k)));
  return res;
}

real lud::detsgn() const
{
  real res = 1.0;
  for( int k = 0; k < _data.str(); k++ )
    res *= fsgn(_data(k, k));
  return res * permsgn;
}

}; // namespace lwml
