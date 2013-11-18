#include "cmplx.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

cmplx cmplx::div( const cmplx& c, const cmplx& d )
{
  real r, den;

  if( fabs( d._re ) >= fabs( d._im ) ){
    r = d._im / d._re;
    den = d._re + r * d._im;
    return cmplx( (c._re + r * c._im) / den, (c._im - r * c._re) / den );
  }else{
    r = d._re / d._im;
    den = d._im + r * d._re;
    return cmplx( (c._re * r + c._im) / den, (c._im * r - c._re) / den );
  }
}

cmplx cmplx::rot( const cmplx& pnt, real angle, const cmplx& cnt )
{
  real sn, cs, dx, dy;

  sn = sin(angle);   cs = cos(angle);
  dx = pnt._re - cnt._re;
  dy = pnt._im - cnt._im;

  return cmplx(cnt._re + dx * cs - dy * sn,
               cnt._im + dx * sn + dy * cs);
}

cmplx cmplx::sqrt( const cmplx& z )
{
  cmplx  c;
  real x, y, w, r;

  if( (z._re == 0.0) && (z._im == 0.0) ){
    c._re = 0.0;
    c._im = 0.0;
    return c;
  }else{
    x = fabs( z._re );
    y = fabs( z._im );
    if( x >= y ){
      r = y / x;
      w = ::sqrt(x) * ::sqrt( 0.5 * (1.0 + ::sqrt(1.0 + r*r)) );
    }else{
      r = x / y;
      w = ::sqrt(y) * ::sqrt( 0.5 * (r + ::sqrt(1.0 + r*r)) );
    }
    if( z._re >= 0.0 ){
      c._re = w;
      c._im = z._im / (2.0 * w);
    }else{
      c._im = (z._im >= 0.0) ? w : -w;
      c._re = z._im / (2.0 * c._im);
    }
    return c;
  }
} 

}; // namespace lwml
