// Colored map
// lwml, (c) ltwood

#ifndef _CMAP_
#define _CMAP_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"

#include "lwml/types/rgb.h"
#include "lwml/m_types/matrix.h"
#include "lwml/m_types/imatrix.h"

/*#lake:stop*/

namespace lwml {

// Интерфейс преобразователя значений в цвет.

class i_mapcolorer : public interface {
public:
  virtual hsbcolor operator()( real x ) const = 0;
};

// colorer, рисующий изображение в градациях серого,
// но с масштабированием из фиксированного диапазона.

class fixed_gray_colorer : public i_mapcolorer, public value {
public:
  fixed_gray_colorer( real ax, real bx ) : _ax(ax), _bx(bx) {}

  virtual hsbcolor operator()( real x ) const {
    return hsbcolor(0.0, 0.0, scale::coord(x, _ax, _bx, 0.0, 1.0));
  }

private:
  real _ax, _bx;
};

// colorer, рисующий простые карты (содержит магические константы).
// Отрицательные значения рисуются синим цветом,
// положительные -- красным.

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

// Функции для вывода матриц в виде цветовых карт.

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
