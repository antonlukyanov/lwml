#include "lwml/geometry/circle.h"

#include "lwml/geometry/igeom.h"
#include "lwml/types/t_list.h"

/*#lake:stop*/

namespace lwml {

void circlegen::circle( i_circle_pixel_map& pm, int cx, int cy, int r )
{
  t_list<int_point> arc;

  // целочисленный алгоритм рисования окружности
  int x = 0, y = r;
  int d = 1 - r;
  int delta1 = 3, delta2 = -2 * r + 5;

  arc.put(int_point(x, y));
  while( y > x ){
    if( d < 0 ){
      d += delta1;
      delta2 += 2;
      ++x;
    }
    else{
      d += delta2;
      delta2 += 4;
      ++x;
      --y;
    }
    delta1 += 2;
    arc.put(int_point(x, y));
  }

  for( int i = 0; i < arc.len(); ++i)
    pm.pixel(cx - arc[i].x(), cy + arc[i].y());
  for( int i = arc.len()-1; i >= 0 ; --i)
    pm.pixel(cx - arc[i].y(), cy + arc[i].x());

  for( int i = 0; i < arc.len(); ++i)
    pm.pixel(cx - arc[i].y(), cy - arc[i].x());
  for( int i = arc.len()-1; i >= 0 ; --i)
    pm.pixel(cx - arc[i].x(), cy - arc[i].y());

  for( int i = 0; i < arc.len(); ++i)
    pm.pixel(cx + arc[i].x(), cy - arc[i].y());
  for( int i = arc.len()-1; i >= 0 ; --i)
    pm.pixel(cx + arc[i].y(), cy - arc[i].x());

  for( int i = 0; i < arc.len(); ++i)
    pm.pixel(cx + arc[i].y(), cy + arc[i].x());
  for( int i = arc.len()-1; i >= 0 ; --i)
    pm.pixel(cx + arc[i].x(), cy + arc[i].y());

}

}; // namespace lwml
