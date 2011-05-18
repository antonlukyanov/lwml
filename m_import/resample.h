// resampling
// lwml, (c) ltwood

#ifndef _RESAMPLE_
#define _RESAMPLE_

#include "defs.h"
#include "mdefs.h"

#include "vector.h"
#include "matrix.h"

/*#lake:stop*/

namespace lwml {

class lanczos : public scope {
public:
  // Ресемплинг вектора/матрицы src в вектор/матрицу dst.
  // Параметр r задает радиус ядра Ланцоша, используемого при ресамплинге.
  // Обычные значения для радиуса ядра -- 2, 3, 4.
  static void warp( vector &dst, const vector &src, real r = 3 );
  static void warp( matrix &dst, const matrix &src, real r = 3 );

  // Вычисление значения интерполянта Ланцоша в произвольной точке.
  // Координаты точки задаются в естественных координатах т.е.
  // от 0 <= x <= len-1 по каждой координате.
  static real calc( const vector &src, real x, real r = 3 );
  static real calc( const matrix &src, real y, real x, real r = 3 );

private:
  static real sinc( real x ) { return lwml::sinc(M_PI * x); }

  static real lanczos_kernel( real x, real r ){
    return fabs(x) <= r ? sinc(x) * sinc(x/r) : 0.0;
  }

  static real calc_in_line( const matrix &src, int y, real x, real r );
};

}; // namespace lwml

#endif // _RESAMPLE_
