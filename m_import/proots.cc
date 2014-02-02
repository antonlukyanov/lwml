#include "proots.h"
#include "mdefs.h"
#include "basex.h"

namespace lwml {

const real proots::_frac[PROOTS_MR+1] = {
  0.0, 0.5, 0.25, 0.75, 0.13, 0.38, 0.62, 0.88, 1.0
}; // fractions used to break a limit cycle

// Laguerre's method

void proots::laguer( const polyn& ar, const polyn& ai, int m, cmplx& x, real poly_eps )
{
  int  iter, j;
  real abx, abp, abm, err;
  cmplx dx, x1, b, d, f, g, h, sq, gp, gm, g2;

  for( iter = 1; iter <= PROOTS_MAXIT; iter++ ){ // loop over iterations
    b = cmplx( ar[m], ai[m] ); // up to allowed maximum
    err = cmplx::norm(b);
    d = f = cmplx(0.0, 0.0);
    abx = cmplx::norm(x);
    for( j = m-1; j >= 0; j-- ){ // effcient computation of the polynomial
      f = x * f + d; // and its first two derivatives
      d = x * d + b;
      b = x * b + cmplx(ar[j], ai[j]);
      err = cmplx::norm(b) + abx * err;
    }
    err *= poly_eps;
    // estimate of roundoff error in evaluating polynomial
    if( cmplx::norm(b) <= err ) return; // we are on the root
    g = d / b; // the generic case: use Laguerre's formula
    g2 = g * g;
    h = g2 - 2.0 * f / b;
    sq = cmplx::sqrt((m-1) * (m * h - g2));
    gp = g + sq;
    gm = g - sq;
    abp = cmplx::norm(gp);
    abm = cmplx::norm(gm);
    if( abp < abm ) gp = gm;
    dx = t_max<real>(abp, abm) > 0.0 ?
      cmplx(m, 0.0) / gp : cmplx(cos(real_cast(iter)), sin(real_cast(iter)) * (1.0+abx));
    x1 = x - dx;
    //!! SIC: converged
    if( x.re() == x1.re() && x.im() == x1.im() ) return;
    if( iter % PROOTS_MT )
      x = x1;
    else
      x -= dx * _frac[iter/PROOTS_MT];
    // Every so often we take a fractional step, to break any limit cycle
    // (itself a rare occurrence)
  }
  throw ex_noconv();
  // Very unusual - can occur only for complex roots
  // Try a different starting guess for the root
}

void proots::calc(
  const polyn& a, vector& rroots, vector& iroots, real poly_eps, real root_eps
)
{
  int j, jj, dg;
  cmplx x, b, c;

  dg = a.deg();
  test_size2(rroots.len(), dg, iroots.len(), dg);
  polyn adr(a); // copy of coeffcients for successive deflation
  polyn adi(a.deg());
  adi.set_zero();

  for( j = dg; j >= 1; j-- ){ // loop over each root to be found
    x = cmplx(0.0, 0.0); // start at zero to favor convergence to
    laguer(adr, adi, j, x, poly_eps); // smallest remaining root, and find the root
    if( fabs(x.im()) <= 2.0 * root_eps * fabs(x.re()) )
      x.im() = 0.0;
    rroots[j-1] = x.re();
    iroots[j-1] = x.im();
    b = cmplx(adr[j], adi[j]); // forward deflation
    for( jj = j-1; jj >= 0; jj-- ){
      c = cmplx(adr[jj], adi[jj]);
      adr[jj] = b.re();
      adi[jj] = b.im();
      b = x * b + c;
    }
  }
  adi.set_zero();
  for( j = 0; j < dg; j++ ){                 // polish the roots using
    x = cmplx(rroots[j], iroots[j]);
    laguer(a, adi, dg, x, poly_eps);         // the undeflated coefficients
    rroots[j] = x.re();
    iroots[j] = x.im();
  }
}

}; // namespace lwml
