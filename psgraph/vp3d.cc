#include "lwml/psgraph/vp3d.h"

/*#lake:stop*/

namespace lwml {

frect viewport3d::calcbound( scene3d sc, viewpoint3d vp )
{
  fpoint3d vb = sc.viewbox();

  fpoint3d p3[8];
  p3[0] = fpoint3d(0.0,    0.0,    0.0);
  p3[1] = fpoint3d(0.0,    0.0,    vb.z());
  p3[2] = fpoint3d(0.0,    vb.y(), 0.0);
  p3[3] = fpoint3d(0.0,    vb.y(), vb.z());
  p3[4] = fpoint3d(vb.x(), 0.0,    0.0);
  p3[5] = fpoint3d(vb.x(), 0.0,    vb.z());
  p3[6] = fpoint3d(vb.x(), vb.y(), 0.0);
  p3[7] = fpoint3d(vb.x(), vb.y(), vb.z());

  fpoint p2[8];
  for( int j = 0; j < 8; j++ )
    p2[j] = vp(p3[j]);

  frect bnd(p2[0].x(), p2[0].x(), p2[0].y(), p2[0].y());
  for( int j = 1; j < 8; j++ ){
    bnd.ax() = t_min<real>(bnd.ax(), p2[j].x());
    bnd.bx() = t_max<real>(bnd.bx(), p2[j].x());
    bnd.ay() = t_min<real>(bnd.ay(), p2[j].y());
    bnd.by() = t_max<real>(bnd.by(), p2[j].y());
  }

  return bnd;
}

frect viewport3d::calcrect( referer<i_canvas> cv, real border )
{
  frect res(cv->boundrect());
  real dx = cv->xunit() * border;
  real dy = cv->yunit() * border;
  res.ax() += dx;
  res.bx() -= dx;
  res.ay() += dy;
  res.by() -= dy;
  return res;
}

viewport3d::viewport3d( referer<i_canvas> cv, scene3d sc, viewpoint3d vp, real border )
  : _sc(sc), _vp(vp), _cv(viewport::create(cv, calcbound(sc, vp), calcrect(cv, border)))
{
}

void viewport3d::pixel( fpoint3d p )
{
  fpoint v2 = prj(p);
  _cv->pixel(v2.x(), v2.y());
}

void viewport3d::marker( fpoint3d p )
{
  fpoint v2 = prj(p);
  _cv->marker(v2.x(), v2.y());
}

void viewport3d::line( fpoint3d p1, fpoint3d p2 )
{
  fpoint v21 = prj(p1);
  fpoint v22 = prj(p2);
  _cv->line(v21.x(), v21.y(), v22.x(), v22.y());
}

void viewport3d::box( fpoint3d p1, fpoint3d p2 )
{
  fpoint v21 = prj(p1);
  fpoint v22 = prj(p2);
  _cv->box(v21.x(), v21.y(), v22.x(), v22.y());
}

void viewport3d::rectangle( fpoint3d p1, fpoint3d p2 )
{
  fpoint v21 = prj(p1);
  fpoint v22 = prj(p2);
  _cv->rectangle(v21.x(), v21.y(), v22.x(), v22.y());
}

void viewport3d::triangle( fpoint3d p1, fpoint3d p2, fpoint3d p3 )
{
  fpoint v21 = prj(p1);
  fpoint v22 = prj(p2);
  fpoint v23 = prj(p3);
  _cv->triangle(v21.x(), v21.y(), v22.x(), v22.y(), v23.x(), v23.y());
}

void viewport3d::facet( fpoint3d p1, fpoint3d p2, fpoint3d p3 )
{
  fpoint v21 = prj(p1);
  fpoint v22 = prj(p2);
  fpoint v23 = prj(p3);
  _cv->facet(v21.x(), v21.y(), v22.x(), v22.y(), v23.x(), v23.y());
}

void viewport3d::puts( fpoint3d p, const char *str, const align& ta, fpoint3d dir )
{
  fpoint v2 = prj(p);
  fpoint d2 = _vp(dir);
  _cv->puts(v2.x(), v2.y(), str, ta, d2.arg());
}

void viewport3d::reset()
{
  _cv->reset();
}

void viewport3d::point( fpoint3d p )
{
  fpoint v2 = prj(p);
  _cv->point(v2.x(), v2.y());
}

}; // namespace lwml
