#include "lwml/geometry/igeomx.h"

/*#lake:stop*/

namespace lwml {

// linegen

int linegen::calc_len( real x1, real y1, real x2, real y2, closure cl )
{
  real len = t_max(fabs(x2-x1), fabs(y2-y1));
  int endl = int_cast((cl == CLOSED_END) ? ceil(len) : ceil(len)-1);
  return endl + 1;
}

void linegen::line( i_pixel_map& pm, real x1, real y1, real x2, real y2, closure cl )
{
  real len = t_max(fabs(x2-x1), fabs(y2-y1));
  real dx = (x2-x1) / len;
  real dy = (y2-y1) / len;
  real x = x1;
  real y = y1;
  int endl = int_cast((cl == CLOSED_END) ? ceil(len) : ceil(len)-1);
  for( int j = 0; j <= endl; j++ ){
    pm.pixel(fpr::lround(x), fpr::lround(y), j);
    x += dx;
    y += dy;
  }
}

void linegen::line_sp( i_pixel_map& pm, real x1, real y1, real x2, real y2, closure cl )
{
  real len = t_max(fabs(x2-x1), fabs(y2-y1));
  real dx = (x2-x1) / len;
  real dy = (y2-y1) / len;

  int sdx = 1, sdy = 0;
  if( fabs(x2-x1) > fabs(y2-y1) ){
    sdx = 0;
    sdy = 1;
  }

  real x = x1;
  real y = y1;
  int endl = int_cast((cl == CLOSED_END) ? ceil(len) : ceil(len)-1);
  for( int j = 0; j <= endl; j++ ){
    int ix = fpr::ltrunc(x);
    int iy = fpr::ltrunc(y);
    real v = (x - ix)*sdx + (y - iy)*sdy;
    pm.pixel(ix, iy, j, 1-v);
    pm.pixel(ix + sdx, iy + sdy, j, v);
    x += dx;
    y += dy;
  }
}

// circle

void circle::conv( int& x, int& y, int x0, int y0 )
{
  // переносим начало отсчета в центр окружности
  x -= x0;
  y -= y0;
  // отражаем картинку в первый квадрант
  if( x < 0 ) x = -x - 1;
  if( y < 0 ) y = -y - 1;
}

bool circle::is_in( int x, int y, int x0, int y0, real r )
{
  conv(x, y, x0, y0);
  return hypot(x+0.5, y+0.5) < r;
}

bool circle::is_in_in( int x, int y, int x0, int y0, real r )
{
  conv(x, y, x0, y0);
  return hypot(x+1, y+1) < r;
}

bool circle::is_in_out( int x, int y, int x0, int y0, real r )
{
  conv(x, y, x0, y0);
  return hypot(x, y) < r;
}

real circle::weight( int x, int y, int x0, int y0, real r )
{
  conv(x, y, x0, y0);
  real x1 = x;
  real y1 = y;
  real x2 = x + 1;
  real y2 = y + 1;

  if( hypot(x2, y2) < r ) return 1.0;
  if( hypot(x1, y1) > r ) return 0.0;

  double xp2 = r*r - y2*y2;
  real xa = (x1*x1 < xp2) ? sqrt(xp2) : x1;
  double xp1 = r*r - y1*y1;
  real xb = (xp1 < x2*x2) ? sqrt(xp1) : x2;

  real ta = asin(xa / r);
  real tb = asin(xb / r);
  return (r*r/2.0)*((tb-ta) + (sin(2*tb)-sin(2*ta))/2.0) - (xb-xa)*y1 + (xa-x1);
}

}; // namespace lwml
