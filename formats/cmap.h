// Colored map
// lwml, (c) ltwood

#ifndef _CMAP_
#define _CMAP_

#include "defs.h"
#include "mdefs.h"

#include "rgb.h"
#include "matrix.h"
#include "imatrix.h"

/*#lake:stop*/

namespace lwml {

// ��������� ��������������� �������� � ����.

class i_mapcolorer : public interface {
public:
  virtual hsbcolor operator()( real x ) const = 0;
};

// colorer, �������� ����������� � ��������� ������,
// �� � ���������������� �� �������������� ���������.

class fixed_gray_colorer : public i_mapcolorer, public value {
public:
  fixed_gray_colorer( real ax, real bx ) : _ax(ax), _bx(bx) {}

  virtual hsbcolor operator()( real x ) const {
    return hsbcolor(0.0, 0.0, scale::coord(x, _ax, _bx, 0.0, 1.0));
  }

private:
  real _ax, _bx;
};

// colorer, �������� ������� ����� (�������� ���������� ���������).
// ������������� �������� �������� ����� ������,
// ������������� -- �������.

class map_colorer : public i_mapcolorer, public value {
public:
  map_colorer( real ax, real bx ) : _ax(ax), _bx(bx) {
    _ax = t_min<real>(_ax, -_bx);
    _bx = t_max<real>(_bx, -_ax);
  }

  virtual hsbcolor operator()( real x ) const {
    real hue = (x < 0) ? 0.68 : 0.07;
    real sat = (x < 0) ? scale::coord(x, _ax, 0.0, 0.9, 0.0) : scale::coord(x, 0.0, _bx, 0.0, 0.9);
    return hsbcolor(hue, sat, 0.4);
  }

private:
  real _ax, _bx;
};

// ������� ��� ������ ������ � ���� �������� ����.

class cmap : public scope {
public:
  enum direction { NORMAL, REVERSE };

  static void save( 
    const char* fn, const matrix& src, const i_mapcolorer& map, direction dir = NORMAL
  );
  static void save( 
    const char* fn, const int_matrix& src, const i_mapcolorer& map, direction dir = NORMAL
  );
};

}; // namespace lwml

#endif // _CMAP_
