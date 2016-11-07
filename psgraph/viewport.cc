#include "lwml/psgraph/viewport.h"
#include "lwml/geometry/geomx.h"

/*#lake:stop*/

namespace lwml {

// viewport

void viewport::setup()
{
  _colortype.set_gray();
  _gray = 0.0;
}

void viewport::setcol()
{
  if( _colortype.is_gray() )
    _cv->set_gray(_gray);
  else if( _colortype.is_rgb() )
    _cv->set_rgb(_rgb);
  else if( _colortype.is_hsb() )
    _cv->set_hsb(_hsb);
}

real viewport::X( real x )
{
  return scale::coord(x, _src.ax(), _src.bx(), _dst.ax(), _dst.bx());
}

real viewport::Y( real y )
{
  return scale::coord(y, _src.ay(), _src.by(), _dst.ay(), _dst.by());
}

real viewport::xunit() const
{
  return scale::length(_cv->xunit(), _dst.ax(), _dst.bx(), _src.ax(), _src.bx());
}

real viewport::yunit() const
{
  return scale::length(_cv->yunit(), _dst.ay(), _dst.by(), _src.ay(), _src.by());
}

viewport::viewport( referer<i_canvas> dcv, frect srcrect )
  : _cv(dcv)
{
  _src = srcrect;
  _dst = _cv->boundrect();
  setup();
  set_clip(true);
}

viewport::viewport( referer<i_canvas> dcv, frect srcrect, frect dstrect )
  : _cv(dcv)
{
  _src = srcrect;
  _dst = dstrect;
  setup();
  set_clip(true);
}

void viewport::pixel( real x, real y )
{
  setcol();
  _cv->pixel(X(x), Y(y));
}

void viewport::marker( real x, real y )
{
  setcol();
  _cv->marker(X(x), Y(y));
}

void viewport::line( real x1, real y1, real x2, real y2 )
{
  setcol();
  if( _clip_state ){
    real_point a1(x1, y1);
    real_point a2(x2, y2);
    clipping::clip_line(a1, a2, _src);
    x1 = a1.x();
    y1 = a1.y();
    x2 = a2.x();
    y2 = a2.y();
  }
  _cv->line(X(x1), Y(y1), X(x2), Y(y2));
}

void viewport::box( real x1, real y1, real x2, real y2 )
{
  setcol();
  _cv->box(X(x1), Y(y1), X(x2), Y(y2));
}

void viewport::rectangle( real x1, real y1, real x2, real y2 )
{
  setcol();
  _cv->rectangle(X(x1), Y(y1), X(x2), Y(y2));
}

void viewport::triangle( real x1, real y1, real x2, real y2, real x3, real y3 )
{
  setcol();
  _cv->triangle(X(x1), Y(y1), X(x2), Y(y2), X(x3), Y(y3));
}

void viewport::facet( real x1, real y1, real x2, real y2, real x3, real y3 )
{
  setcol();
  _cv->facet(X(x1), Y(y1), X(x2), Y(y2), X(x3), Y(y3));
}

void viewport::set_gray( real graycol )
{
  _colortype.set_gray();
  _gray = graycol;
}

void viewport::set_rgb( rgbcolor rgbcol )
{
  _colortype.set_rgb();
  _rgb = rgbcol;
}

void viewport::set_hsb( hsbcolor hsbcol )
{
  _colortype.set_hsb();
  _hsb = hsbcol;
}

void viewport::puts(
  real x, real y, const char *str, const align& algn, real alpha
)
{
  setcol();
  real dx = cos(alpha);
  real dy = sin(alpha);
  real bdx = scale::length(dx, _src.ax(), _src.bx(), _dst.ax(), _dst.bx());
  real bdy = scale::length(dy, _src.ay(), _src.by(), _dst.ay(), _dst.by());
  _cv->puts(X(x), Y(y), str, algn, atan2(bdy, bdx));
}

void viewport::show_viewbox()
{
  setcol();
  _cv->rectangle(_dst.ax(), _dst.ay(), _dst.bx(), _dst.by());
}

void viewport::special( real x, real y, int id, const char* cmd )
{
  setcol();
  _cv->special(X(x), Y(y), id, cmd);
}

// diagram

frect diagram::blow( referer<i_canvas> cv, const diagram_param& par )
{
  frect src = cv->boundrect();
  real dx = cv->xunit() * (1.0 + par.ygridsize() + par.top_shift() + par.bot_shift());
  real dy = cv->yunit() * (1.0 + par.xgridsize() + par.top_shift() + par.bot_shift());
  return frect(src.ax() + dx, src.bx() - dx, src.ay() + dy, src.by() - dy);
}

diagram::diagram( referer<i_canvas> dcv, frect srcrect, const diagram_param& par )
  : viewport(dcv, srcrect, blow(dcv, par))
{
  _tdx = par.top_shift() * dcv->xunit();
  _bdx = par.bot_shift() * dcv->xunit();
  _tdy = par.top_shift() * dcv->yunit();
  _bdy = par.bot_shift() * dcv->yunit();
  _xgsz = par.xgridsize() * dcv->yunit();
  _ygsz = par.ygridsize() * dcv->xunit();
}

void diagram::xgrid( real x, const char *subs )
{
  setcol();
  real xx = X(x);
//  basecv()->line(xx, baserect().ay(), xx, baserect().ay() + _xgsz);
  basecv()->line(xx, baserect().ay() - _xgsz, xx, baserect().ay());
  if( subs )
    basecv()->puts(xx, baserect().ay() - _tdy - _xgsz, subs, align().hcenter().top(), 0.0);
}

void diagram::ygrid( real y, const char *subs )
{
  setcol();
  real yy = Y(y);
//  basecv()->line(baserect().ax(), yy, baserect().ax() + _ygsz, yy);
  basecv()->line(baserect().ax() - _ygsz, yy, baserect().ax(), yy);
  if( subs )
    basecv()->puts(baserect().ax() - _bdx - _ygsz, yy, subs, align().hcenter().bot(), M_PI/2.0);
}

void diagram::title( const char *title )
{
  setcol();
  basecv()->puts(baserect().bx(), baserect().by() + _bdy, title, align().right().bot(), 0.0);
}

void diagram::xsubscript( const char *bl, const char *br )
{
  setcol();
  if( bl )
    basecv()->puts(baserect().ax(), baserect().ay() - _tdy, bl, align().left().top(), 0.0);
  if( br )
    basecv()->puts(baserect().bx(), baserect().ay() - _tdy, br, align().right().top(), 0.0);
}

void diagram::ysubscript( const char *lb, const char *lt )
{
  setcol();
  if( lb )
    basecv()->puts(baserect().ax() - _bdx, baserect().ay(), lb, align().left().bot(), M_PI/2.0);
  if( lt )
    basecv()->puts(baserect().ax() - _bdx, baserect().by(), lt, align().right().bot(), M_PI/2.0);
}

}; // namespace lwml
