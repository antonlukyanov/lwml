#include "rfft.h"

#include "fft.h"
#include "trig_gen.h"

/*#lake:stop*/

namespace lwml {

void rfft::rcfft2( vector& a, vector& b )
{
  test_size(a.len(), b.len());
  int n = a.len();

  fft::cfft(a, b);
  for( int j = 1; j < n/2; j++ ){
    real xrj = a[j];
    real xij = b[j];
    real xrnj = a[n-j];
    real xinj = b[n-j];
    a[j] = (xrj + xrnj)/2.0;
    b[j] = (xij - xinj)/2.0;
    a[n-j] = (xij + xinj)/2.0;
    b[n-j] = (xrnj - xrj)/2.0;
  }
  real xrn2 = a[n/2];
  real xin2 = b[n/2];
  for( int j = n/2+1; j < n; j++ ){
    a[j-1] = a[j];
    b[j-1] = b[j];
  }
  a[n-1] = b[0];
  b[n-1] = xin2;
  b[0] = xrn2;
}

void rfft::crifft2( vector& a, vector& b )
{
  test_size(a.len(), b.len());
  int n = a.len();

  real zr0 = a[n-1];
  real zrn2 = b[n-1];
  for( int j = n-1; j >= n/2+1; j-- ){
    a[j] = a[j-1];
    b[j] = b[j-1];
  }
  a[n/2] = b[0];
  b[n/2] = zrn2;
  b[0] = zr0;
  for( int j = 1; j < n/2; j++ ){
    real yrj = a[j];
    real yij = b[j];
    real zrj = a[n-j];
    real zij = b[n-j];
    a[j] = yrj - zij;
    b[j] = yij + zrj;
    a[n-j] = yrj + zij;
    b[n-j] = zrj - yij;
  }
  fft::cifft(a, b);
}

void rfft::rcfft( vector& a, vector& b )
{
  test_size(a.len(), b.len());
  int n = a.len();

  rcfft2(a, b);
  real yr0 = a[0] / 2.0;
  real yrn2 = b[0] / 2.0;
  real zr0 = a[n-1] / 2.0;
  real zrn2 = b[n-1] / 2.0;
  trig_gen tg(M_PI / n);
  tg.next();
  for( int j = 1; j < n/2; j++ ){
    real yrj = a[j] / 2.0;
    real yij = b[j] / 2.0;
    real zrj = a[n-1-j] / 2.0;
    real zij = b[n-1-j] / 2.0;
    real cs = tg.get_cos();
    real sn = tg.get_sin();
    tg.next();
    a[j] = yrj + cs * zrj + sn * zij;
    b[j] = yij + cs * zij - sn * zrj;
    a[n-1-j] = yrj - cs * zrj - sn * zij;
    b[n-1-j] = -yij + cs * zij - sn * zrj;
  }
  for( int j = n-1; j > n/2; j-- ){
    a[j] = a[j-1];
    b[j] = b[j-1];
  }
  a[n/2] = yrn2;
  b[n/2] = -zrn2;
  a[0] = yr0 + zr0;
  b[0] = yr0 - zr0;
}

void rfft::crifft( vector& a, vector& b )
{
  test_size(a.len(), b.len());
  int n = a.len();

  trig_gen tg(M_PI / n);
  tg.next();
  for( int j = 1; j < n/2; j++ ){
    real xrj = a[j];
    real xij = b[j];
    real xrnj = a[n-j];
    real xinj = b[n-j];
    a[j] = xrj + xrnj;
    b[j] = xij - xinj;
    real cs = tg.get_cos();
    real sn = tg.get_sin();
    tg.next();
    a[n-j] = (xrj - xrnj) * cs - (xij + xinj) * sn;
    b[n-j] = (xrj - xrnj) * sn + (xij + xinj) * cs;
  }
  real xrn4 = a[n/2];
  real xin4 = b[n/2];
  for( int j = n/2; j < n-1; j++ ){
    a[j] = a[j+1];
    b[j] = b[j+1];
  }
  real xr0 = a[0];
  real xrn2 = b[0];
  a[0] = xr0 + xrn2;
  a[n-1] = xr0 - xrn2;
  b[0] = 2.0 * xrn4;
  b[n-1] = -2.0 * xin4;
  crifft2(a, b);
}

}; // namespace lwml
