#include "marker3d.h"

/*#lake:stop*/

namespace lwml {

vp3d_marker::vp3d_marker( referer<viewport3d> vp, real gsize )
  : _vp(vp), _gridsize(gsize)
{
}

void vp3d_marker::show_viewbox()
{
  fpoint3d a = _vp->scene().ax();
  fpoint3d b = _vp->scene().bx();
  _vp->reset();
  _vp->point(fpoint3d(a.x(), a.y(), b.z()));
  _vp->point(fpoint3d(a.x(), a.y(), a.z()));
  _vp->point(fpoint3d(b.x(), a.y(), a.z()));
  _vp->point(fpoint3d(b.x(), b.y(), a.z()));
  _vp->reset();
}

void vp3d_marker::gridline( fpoint3d pos, fpoint3d dir, real sz )
{
  fpoint p1 = _vp->prj(pos);
  fpoint3d pos_sh = pos; pos_sh += dir;
  fpoint p2 = _vp->prj(pos_sh);
  fpoint p12(p2.x() - p1.x(), p2.y() - p1.y());
  p12.x() /= _vp->cv()->xunit();
  p12.y() /= _vp->cv()->yunit();
  p12 *= sz / p12.norm();
  p12.x() *= _vp->cv()->xunit();
  p12.y() *= _vp->cv()->yunit();
  fpoint p2c(p1.x() + p12.x(), p1.y() + p12.y());
  _vp->cv()->line(p1.x(), p1.y(), p2c.x(), p2c.y());
}

void vp3d_marker::xgrid( real x, const char *subs )
{
  fpoint3d ax = _vp->scene().ax();

  gridline(fpoint3d(x, ax.y(), ax.z()), fpoint3d(0, 1, 0), _gridsize);
  if( subs ){
    fpoint3d pos(x, ax.y(), ax.z());
    _vp->puts(pos, subs, align().hcenter().top(), fpoint3d(1, 0, 0));
  }
}

void vp3d_marker::ygrid( real y, const char *subs )
{
  fpoint3d ax = _vp->scene().ax();
  fpoint3d bx = _vp->scene().bx();

  gridline(fpoint3d(bx.x(), y, ax.z()), fpoint3d(-1, 0, 0), _gridsize);
  if( subs ){
    fpoint3d pos(bx.x(), y, ax.z());
    _vp->puts(pos, subs, align().hcenter().top(), fpoint3d(0, 1, 0));
  }
}

void vp3d_marker::zgrid( real z, const char *subs )
{
  fpoint3d ax = _vp->scene().ax();

  gridline(fpoint3d(ax.x(), ax.y(), z), fpoint3d(1, 0, 0), _gridsize);
  if( subs ){
    fpoint3d pos(ax.x(), ax.y(), z);
    _vp->puts(pos, subs, align().hcenter().bot(), fpoint3d(0, 0, 1));
  }
}

void vp3d_marker::xsubscript(
  const char *bl, const char *br, const align& lal, const align& ral
)
{
  fpoint3d ax = _vp->scene().ax();
  fpoint3d bx = _vp->scene().bx();

  fpoint3d v3l(ax.x(), ax.y(), ax.z());
  fpoint3d v3r(bx.x(), ax.y(), ax.z());
  fpoint3d dir(1, 0, 0);
  if( bl )
    _vp->puts(v3l, bl, lal, dir);
  if( br )
    _vp->puts(v3r, br, ral, dir);
}

void vp3d_marker::ysubscript(
  const char *bl, const char *br, const align& lal, const align& ral
)
{
  fpoint3d ax = _vp->scene().ax();
  fpoint3d bx = _vp->scene().bx();

  fpoint3d v3l(bx.x(), ax.y(), ax.z());
  fpoint3d v3r(bx.x(), bx.y(), ax.z());
  fpoint3d dir(0, 1, 0);
  if( bl )
    _vp->puts(v3l, bl, lal, dir);
  if( br )
    _vp->puts(v3r, br, ral, dir);
}

void vp3d_marker::zsubscript(
  const char *bl, const char *br, const align& lal, const align& ral
)
{
  fpoint3d ax = _vp->scene().ax();
  fpoint3d bx = _vp->scene().bx();

  fpoint3d v3l(ax.x(), ax.y(), ax.z());
  fpoint3d v3r(ax.x(), ax.y(), bx.z());
  fpoint3d dir(0, 0, 1);
  if( bl )
    _vp->puts(v3l, bl, lal, dir);
  if( br )
    _vp->puts(v3r, br, ral, dir);
}

}; // namespace lwml
