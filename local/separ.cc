#include "separ.h"
#include "matrix.h"
#include "t_sorter.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

// delta-критерий (В.В.Майоров)

// формирует усредненный вектор по векторам из строк матрицы
void delta_separ::setmid( vector& mid, const i_vector_set& matr )
{
  for( int j = 0; j < matr.len(); j++ )
    for( int i = 0; i < matr.dim(); i++ )
      mid[i] += matr(j, i);
}

// находит скалярное произведение вектора и j-й строки матрицы
real delta_separ::inner( const vector& a, const i_vector_set& m, int j )
{
  double res = 0.0;
  for( int i = 0; i < m.dim(); i++ )
    res += a[i] * m(j, i);
  return res;
}

// вычисляет значение дельта-критерия В.В.Майорова для двух наборов векторов
real delta_separ::calc( const i_vector_set& m1, const i_vector_set& m2 )
{
  int j;

  test_size(m1.dim(), m2.dim());

  vector mid1(m1.dim());
  vector mid2(m2.dim());
  vector a(m2.dim());
  vector aX(m1.len());
  vector aY(m2.len());

  setmid(mid1, m1);
  setmid(mid2, m2);
  // a = (n1/n)M[X]+(n2/n)M[Y] = (M[X]+M[Y])/n
  for( j = 0; j < m1.dim(); j++ )
    a[j] = (mid1[j] - mid2[j]) / (m1.len() + m2.len());

  for( j = 0; j < m1.len(); j++ )
    aX[j] = inner(a, m1, j);
  for( j = 0; j < m2.len(); j++ )
    aY[j] = inner(a, m2, j);

  t_sorter<vector> aXidx(aX);

  t_sorter<vector> aYidx(aY);

  for( j = 0; j < m1.len() && j < m2.len(); j++ )
    if( aX[aXidx[j]] > aY[aYidx[m2.len() - 1 - j]] )
      break;
  if( j == m1.len() || j == m2.len() )
    fail_unexpected(); //!! SIC: никогда не должно произойти

////  Qx = m1->str - j; Qy = m2->str - j; //!! SIC: !!

  real b0 = (aX[aXidx[j]] + aY[aYidx[j]]) / 2.0;
  real Qx = 0.0;
  real Qy = 0.0;
  for( j = 0; j < m1.len(); j++ )
    if( aX[aXidx[j]] > b0 ) Qx++;
  for( j = 0; j < m2.len(); j++ )
    if( aY[aYidx[j]] < b0 ) Qy++;

  return fsqrt(m1.len() + m2.len()) * (Qx/m1.len() + Qy/m2.len() - 1.0);
}

void delta_separ::weights( vector& a, const i_vector_set& m1, const i_vector_set& m2 )
{
  test_size2(m1.dim(), m2.dim(), a.len(), m1.dim());

  vector mid1(m1.dim());
  vector mid2(m2.dim());

  setmid(mid1, m1);
  setmid(mid2, m2);

  for( int j = 0; j < m1.dim(); j++ )
    a[j] = (mid1[j] - mid2[j]) / (m1.len() + m2.len());
}

// gamma-критерий (А.Ю.Левин)

real gamma_separ::dist( const i_vector_set& m1, int j, const i_vector_set& m2, int i )
{
  double s = 0.0;
  for( int l = 0; l < m1.dim(); l++ )
    s += fabs(m1(j, l) - m2(i, l));
  return s;
}

// Определяет число векторов из m1, имеющих
// ближайшим вектором вектор из своего класса
int gamma_separ::num( const i_vector_set& m1, const i_vector_set& m2 )
{
  int num = 0;
  for( int j = 0; j < m1.len(); j++ ){
    real smin = dist(m1, j, m1, (j + 1) % m1.len());
    for( int i = 0; i < m1.len(); i++ ){
      if( i == j ) continue;
      real s = dist(m1, j, m1, i);
      if( s < smin )
        smin = s;
    }
    bool d = true;
    for( int i = 0; i < m2.len(); i++ ){
      if( dist(m1, j, m2, i) < smin ){
        d = false;
        break;
      }
    }
    if( d ) num++;
  }
  return num;
}

gamma_separ::gamma_separ( const i_vector_set& m1, const i_vector_set& m2 )
{
  test_size(m1.dim(), m2.dim());

  _num1 = m1.len();
  _num2 = m2.len();

  _self1 = num(m1, m2);
  _self2 = num(m2, m1);
}

real gamma_separ::val() const
{
  return fsqrt(_num1 + _num2) * (real_cast(_self1)/_num1 + real_cast(_self2)/_num2 - 1.0);
}

real gamma_separ::mid( int n1, int n2 )
{
  real n = n1 + n2;
  return -sqrt(n)/(n-1);
}

real gamma_separ::sigma2( int n1, int n2 )
{
  real n = n1 + n2;
  return (n-2)/fsqr(n-1) + (n*(1 - 2.0/n1 - 2/n2) + 6.0)/(n-1)/(n-3);
}

real gamma_separ::max_val( real p ) const
{
  return mid(_num1, _num2) + sqrt(sigma2(_num1, _num2) / (1.0-p));
}

}; // namespace lwml
