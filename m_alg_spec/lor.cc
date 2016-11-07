#include "lwml/m_alg_spec/lor.h"

/*#lake:stop*/

namespace lwml {

// Уравнение Лоренца

void lorenz_eq::func( real, const vector& x, vector& dx )
{
  dx[0] = _s * ( x[1] - x[0] );
  dx[1] = - x[0] * x[2] + _r * x[0] - x[1];
  dx[2] = x[0] * x[1] - _b * x[2];
}

/*
// Уравнение в вариацих

static real fv( int i, real t, real *y )
{
    case 0 : return S*(y[1]-y[0]);
    case 1 : return (R-x[2])*y[0]-y[1]-x[0]*y[2];
    case 2 : return x[1]*y[0]+x[0]*y[1]-B*y[2];
}

static rk4de *dev;
real **e;

void lds_init()
{
  int i;
  real *e0;

  dev=(rk4de*)malloc(n*sizeof(rk4de));
  e0=(real*)malloc(n*sizeof(real));
  e=(real**)malloc(n*sizeof(real*));

  for( i=0;i<n;i++ ){
    int j;

    for( j=0;j<n;j++ )
      e0[j]=0.0;
    e0[i]=1.0;
    dev[i]=rk4_init(n, 0.0, e0, fv, h);
    e[i]=(dev[i]->x);
  }

  mem::free(e0);
}

void lds_step()
{
  int i;
  for( i=0;i<n;i++ )
    rk4_step(dev[i]);
}

void lds_done()
{
  int i;
  for( i=0;i<n;i++ )
    rk4_done(dev[i]);
  mem::free(dev);
  mem::free(e);
}
*/

}; // namespace lwml
