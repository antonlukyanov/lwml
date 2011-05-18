#include "meigen.h"

/*#lake:stop*/

namespace lwml {

// eigen

void eigenvalues::rrot( matrix& a, int k, int l, real al, real be )
{
  for( int j = 0; j < a.str(); j++ ){
    real bjk =   al * a(j, k) + be * a(j, l);
    real bjl = - be * a(j, k) + al * a(j, l);
    a(j, k) = bjk;
    a(j, l) = bjl;
  }
}

void eigenvalues::lrot( matrix& a, int k, int l, real al, real be )
{
  for( int j = 0; j < a.str(); j++ ){
    real ckj =   al * a(k, j) + be * a(l, j);
    real clj = - be * a(k, j) + al * a(l, j);
    a(k, j) = ckj;
    a(l, j) = clj;
  }
}

void eigenvalues::rot( matrix& a, matrix& v, int k, int l, real al, real be )
{
  rrot(a, k, l, al, be);
  lrot(a, k, l, al, be);
  rrot(v, k, l, al, be);
}

void eigenvalues::findoptel( const matrix& a, const vector& r, int& k, int& l )
{
  real max = 0.0;
  k = 0;
  for( int j = 0; j < a.str(); j++ )
    if( r[j] > max ){
      max = r[j];
      k = j;
    }
  max = 0.0;
  l = 0;
  for( int j = 0; j < a.col(); j++ )
    if( j != k && fabs(a(k, j)) > max ){
      max = fabs(a(k, j));
      l = j;
    }
}

void eigenvalues::calcr( const matrix& a, vector& r )
{
  for( int k = 0; k < a.str(); k++ ){
    r[k] = 0;
    for( int j = 0; j < a.col(); j++ )
      if( k != j )
        r[k] += fsqr(a(k, j));
  }
}

void eigenvalues::recalcr( const matrix& a, vector& r, int k, int l )
{
  r[k] = 0;
  for( int i = 0; i < a.col(); i++ )
    if( k != i )
      r[k] += fsqr(a(k, i));
  r[l] = 0;
  for( int i = 0; i < a.col(); i++ )
    if( l != i )
      r[l] += fsqr(a(l, i));
}

void eigenvalues::step( matrix& a, matrix& v, vector& r )
{
  int k, l;
  findoptel(a, r, k, l);
  real mu = (a(k, k) - a(l, l)) / (2.0 * a(k, l));
  real smu = fabs(mu) / sqrt(1.0 + fsqr(mu));
  real al = sqrt(0.5 * (1.0 + smu) );
  real be = sqrt(0.5 * (1.0 - smu) );
  if( mu < 0.0 ) be = -be; // sic!
  rot(a, v, k, l, al, be);
  recalcr(a, r, k, l);
}

void eigenvalues::calc( matrix& a, matrix& v, real eps, int maxit )
{
  vector r(a.str());
  v.set_diag(1.0);
  calcr(a, r);
  for( int it = 0; it < a.str() * a.col() * maxit; it++ ){
    step(a, v, r);
    if( r.max() < eps ) goto normend;
  }
  throw ex_noconv();
normend: (void)0;
}

}; // namespace lwml
