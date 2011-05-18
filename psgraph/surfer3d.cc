#include "surfer3d.h"
#include "t_sorter.h"

/*#lake:stop*/

namespace lwml {

scene3d matrix_as_surface::calcscene( const matrix& m, fpoint3d viewbox )
{
  real az = m.min();
  real bz = m.max();
  fpoint3d ax(0, 0, az);
  fpoint3d bx(m.col() - 1, m.str() - 1, bz);
  return scene3d(ax, bx, viewbox);
}

matrix_as_surface::matrix_as_surface(
  referer<i_canvas> cv,
  const matrix& m,
  fpoint3d viewbox,
  viewpoint3d& viewpoint,
  coloring_type ct,
  const i_colorer3d& col
)
  : _vp(viewport3d::create(cv, calcscene(m, viewbox), viewpoint)),
    _viewpoint(viewpoint)
{
  _showcolor = (ct == COLOR);
  draw(m, col);
}

void matrix_as_surface::draw( const matrix& m, const i_colorer3d& col )
{
  int nt = trnum(m);

  // упордочение в z-пордке
  triangler triang(*this, m, _vp->scene());
  t_sorter<triangler> idx(triang);

  for( int j = 0; j < nt; j++ ){
    int num = idx[j];
    int k = idx2it(num);
    int x = idx2ix(num, m);
    int y = idx2iy(num, m);
    plottr(m, x, y, k, col);
  }

  _vp->set_gray(0.0);
}

void matrix_as_surface::plottr( const matrix& m, int x, int y, int k, const i_colorer3d& col )
{
  fpoint3d p1(x, y+1, m(y+1, x));
  fpoint3d p3(x+1, y, m(y, x+1));
  fpoint3d p2(x+k, y+k, m(y+k, x+k));
  fpoint3d p2c(x+(1-k), y+(1-k), m(y+(1-k), x+(1-k)));

  fpoint3d p1s = _vp->scene()(p1);
  fpoint3d p3s = _vp->scene()(p3);
  fpoint3d p2s = _vp->scene()(p2);
  fpoint3d p2cs = _vp->scene()(p2c);

  if( _showcolor ){
    real br = 0.5 * (calccol(p1s, p2s, p3s) + calccol(p1s, p2cs, p3s));
    real val = (2*p1s.z() + 2*p3s.z() + p2s.z() + p2cs.z()) / 6.0 / _vp->scene().viewbox().z();
    _vp->set_hsb(col(br, val));
    _vp->triangle(p1, p2, p3);
  }else
    _vp->facet(p1, p2, p3);
}

real matrix_as_surface::calccol( fpoint3d p1, fpoint3d p2, fpoint3d p3 )
{
  fpoint3d l1 = p2; l1 -= p1;
  fpoint3d l2 = p3; l2 -= p1;
  fpoint3d nm = fpoint3d::vector_mul(l1, l2);
  if( nm.z() < 0 ) // нормаль д.б. наружной
    nm *= -1.0;
  nm /= fpoint3d::norm(nm);
  return fpoint3d::inner_mul(nm, _viewpoint.prj_vect());
}

// matrix_as_surface::triangler

matrix_as_surface::triangler::triangler(
  const matrix_as_surface& src, const matrix& m, const scene3d& sc
)
  : _len(_base->trnum(m)), _val(_len)
{
  _matr= &m;
  _base = &src;
  _scene = &sc;
  for( int i = 0; i < _len; ++i )
    _val[i] = val(i);
}

real matrix_as_surface::triangler::val( int j ) const
{
  int k = _base->idx2it(j);
  int x = _base->idx2ix(j, *_matr);
  int y = _base->idx2iy(j, *_matr);

  real xy3 = (k==0) ? 1 : 2;
  real z3 = (k==0) ? (*_matr)(y, x) : (*_matr)(y+1, x+1);
  real cx = (3.0 * real_cast(x) + xy3) / 3.0;
  real cy = (3.0 * real_cast(y) + xy3) / 3.0;
  real cz = ((*_matr)(y+1, x) + (*_matr)(y, x+1) + z3) / 3.0;

  fpoint3d vc = (*_scene)(fpoint3d(cx, cy, cz));
  return fpoint3d::inner_mul(_base->_viewpoint.prj_vect(), vc);
}

}; // namespace lwml
