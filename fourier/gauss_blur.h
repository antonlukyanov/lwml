// Гауссово размытие изображения
// lwml, (c) ltwood

#ifndef _GAUSS_BLUR_
#define _GAUSS_BLUR_

#include "defs.h"
#include "matrix.h"
#include "progress.h"

/*#lake:stop*/

namespace lwml {

// При каждом вызове proc( sigma_x, sigma_y, phi ),
// результат сворачивается с
// нормированной гауссианой шириной sigma_x, sigma_y по осям и повернутой
// на phi радиан протоив часовой стрелки.
// Предположение: нулевая строка матрицы - низ изображения, то есть росту
// номера строки соответствует рост координаты y изображения.

class gauss_blur : public value {
public:
  gauss_blur( int ly, int lx, int hly = 0, int hlx = 0 );

  void proc(
    const matrix& src, real sigma_x, real sigma_y, real phi = 0.0, tick_mode tick = tmOFF
  );

  int lx() const { return _lx; }
  int ly() const { return _ly; }

  real operator()( int y, int x ) const { return _rx(y, x); }

  void get( matrix& dst ) const;

private:
  int _ly, _lx, _hly, _hlx;
  int _ny2, _nx2;
  matrix _rx, _ix;               // для преобразования фурье и обработки
  matrix _rg, _ig;               // для преобразования гауссианы

  real gauss2d( real sx, real sy, real phi, real x, real y );
};

}; // namespace lwml

#endif // _GAUSS_BLUR_
