#include "vpobject.h"

#include "mdefs.h"
#include "basex.h"
#include "progress.h"

/*#lake:stop*/

namespace lwml {

// viewgrid

viewgrid::viewgrid( referer<i_canvas> vp, int nx, int ny, direction dir )
  : _vp(viewport::create(vp, frect(0.0, nx, 0.0, ny)))
{
  _revers = (dir == REVERSE );
  _nx = nx;
  _ny = ny;
}

void viewgrid::box( int x, int y )
{
  if( _revers )
    _vp->box(x, y, x+1, y+1);
  else
    _vp->box(x, _ny-(y+1), x+1, _ny-y);
}

void viewgrid::rect( int x, int y )
{
  if( _revers )
    _vp->rectangle(x, y, x+1, y+1);
  else
    _vp->rectangle(x, _ny-(y+1), x+1, _ny-y);
}

void viewgrid::show_grid( real dens )
{
  _vp->set_gray(dens);
  for( int x = 1; x < _nx; x++ )
    _vp->line(x, 0.0, x, _ny);
  for( int y = 1; y < _ny; y++ )
    _vp->line(0.0, y, _nx, y);
}

// vector as line

real vector_as_line::correct_min( const vector& x )
{
  real min = x.min();
  real max = x.max();
  if( min < max )
    return min;
  else
    return min - fabs(min/2.0);
}

real vector_as_line::correct_max( const vector& x )
{
  real min = x.min();
  real max = x.max();
  if( min < max )
    return max;
  else
    return max + fabs(max/2.0);
}

vector_as_line::vector_as_line( referer<i_canvas> cv, const vector& x )
  : _vp(viewport::create(cv, frect(0.0, x.len() - 1, correct_min(x), correct_max(x))))
{
  draw(x);
}

void vector_as_line::draw( const vector& x )
{
  for( int j = 0; j < x.len(); j++ )
    _vp->point(j, x[j]);
}

// matrix as grayscale

matrix_as_grayscale::matrix_as_grayscale(
  referer<i_canvas> cv, const matrix& m,
  direction dir,
  const i_colorer& col,
  tick_mode tick
)
  : _vp(viewport::create(cv, frect(0, m.col(), 0, m.str())))
{
  draw(m, (dir == REVERSE), col, tick);
}

void matrix_as_grayscale::draw( const matrix& m, bool revers, const i_colorer& col, tick_mode tick )
{
  real min = m.min();
  real max = m.max();
  if( max == min )
    max += 1.0;
  if( min == min + 1.0 || max == max + 1.0 )
    throw ex_sing();

  if( tick == tmON )
    progress::start("grayscale", m.str());
  for( int iy = 0; iy < m.str(); iy++ ){
    for( int ix = 0; ix < m.col(); ix++ ){
      real nval = scale::coord(m(iy, ix), min, max, 0.0, 1.0);
      _vp->set_hsb(col(nval));
      if( revers )
        _vp->box(ix, iy, ix + 1, iy + 1);
      else
        _vp->box(ix, m.str()-(iy + 1), ix + 1, m.str()-iy);
    }
    if( tick == tmON )
      progress::up(iy);
  }
  if( tick == tmON )
    progress::finish();
  _vp->set_gray(0.0);
}

// matrix as constline

matrix_as_constline::matrix_as_constline(
  referer<i_canvas> cv,
  int str, int col,
  direction dir,
  real eps
)
  : _vp(viewport::create(cv, frect(0, col-1, 0, str-1)))
{
  _str = str;
  _col = col;
  _revers = (dir == REVERSE);
  _eps = eps;
}

// обработать отрезок
void matrix_as_constline::cline( fpoint3d p1, fpoint3d p2, real c )
{
  if( fabs(p1.z() - p2.z()) < _eps * (1.0 + hypot(p1.z(), p2.z())) )
    return;
  if( c < t_min<real>(p1.z(), p2.z()) || t_max<real>(p1.z(), p2.z()) < c )
    return;

  real al = (c - p1.z()) / (p2.z() - p1.z());
  real x = p1.x() + al * (p2.x() - p1.x());
  real y = p1.y() + al * (p2.y() - p1.y());
  if( _revers )
    _vp->point(x, y);
  else
    _vp->point(x, (_col-1) - y);
}

// обработать треугольник
void matrix_as_constline::trcline( fpoint3d p1, fpoint3d p2, fpoint3d p3, real c )
{
  _vp->reset();
  cline(p1, p2, c);
  cline(p2, p3, c);
  cline(p3, p1, c);
}

void matrix_as_constline::draw( const matrix& m, real c, tick_mode tick )
{
  test_size2(m.str(), _str, m.col(), _col);

  if( tick == tmON )
    progress::start("constline", m.str());
  for( int y = 0; y < m.str() - 1; y++ ){
    for( int x = 0; x < m.col() - 1; x++ ){
      int x1 = x+1;
      int y1 = y+1;
      trcline(fpoint3d(x, y, m(y, x)), fpoint3d(x, y1, m(y1, x)), fpoint3d(x1, y, m(y, x1)), c);
      trcline(fpoint3d(x1, y1, m(y1, x1)), fpoint3d(x, y1, m(y1, x)), fpoint3d(x1, y, m(y, x1)), c);
    }
    if( tick == tmON )
      progress::up(y);
  }
  if( tick == tmON )
    progress::finish();
}

void matrix_as_constline::show_grid( real dens )
{
  _vp->set_gray(dens);
  for( int x = 1; x < _col-1; x++ )
    _vp->line(x, 0.0, x, _str-1);
  for( int y = 1; y < _str-1; y++ )
    _vp->line(0.0, y, _col-1, y);
}

}; // namespace lwml
