#include "lwml/geometry/geomx.h"

/*#lake:stop*/

namespace lwml {

uchar clipping::get_code( const fpoint& pnt, const frect& cliprect )
{
  return (pnt.x() < cliprect.ax()) << 3 |
         (pnt.x() > cliprect.bx()) << 2 |
         (pnt.y() < cliprect.ay()) << 1 |
         (pnt.y() > cliprect.by());
}

bool clipping::clip_line( fpoint& l1, fpoint& l2, const frect& cliprect )
{
  uchar c1 = get_code(l1, cliprect);
  uchar c2 = get_code(l2, cliprect);

  while( c1 | c2 ){
    if( c1 & c2 ) return false;
    real dx = l2.x() - l1.x();
    real dy = l2.y() - l1.y();
    if( c1 ){
      if( l1.x() < cliprect.ax() ){
        l1.y() = l1.y() + dy * (cliprect.ax()-l1.x())/dx;
        l1.x() = cliprect.ax();
      }else if( l1.x() > cliprect.bx() ){
        l1.y() = l1.y() + dy * (cliprect.bx()-l1.x())/dx;
        l1.x() = cliprect.bx();
      }else if( l1.y() < cliprect.ay() ){
        l1.x() = l1.x() + dx * (cliprect.ay()-l1.y())/dy;
        l1.y() = cliprect.ay();
      }else if( l1.y() > cliprect.by() ){
        l1.x() = l1.x() + dx * (cliprect.by()-l1.y())/dy;
        l1.y() = cliprect.by();
      }
      c1 = get_code(l1, cliprect);
    }else{
      if( l2.x() < cliprect.ax() ){
        l2.y() = l2.y() + dy * (cliprect.ax()-l2.x())/dx;
        l2.x() = cliprect.ax();
      }else if( l2.x() > cliprect.bx() ){
        l2.y() = l2.y() + dy * (cliprect.bx()-l2.x())/dx;
        l2.x() = cliprect.bx();
      }else if( l2.y() < cliprect.ay() ){
        l2.x() = l2.x() + dx * (cliprect.ay()-l2.y())/dy;
        l2.y() = cliprect.ay();
      }else if( l2.y() > cliprect.by() ){
        l2.x() = l2.x() + dx * (cliprect.by()-l2.y())/dy;
        l2.y() = cliprect.by();
      }
      c2 = get_code(l2, cliprect);
    }
  }
  return true;
}

}; // namespace lwml
