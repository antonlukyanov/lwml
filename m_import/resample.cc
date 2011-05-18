#include "resample.h"

/*#lake:stop*/

namespace lwml {

void lanczos::warp( vector &dst, const vector &src, real r )
{
  if( r < 1 )
    runtime("bad radius (r=%lf) in lanczos::warp()", r);

  int n1 = src.len(), n2 = dst.len();

  for( int px = 0; px < n2; ++px ){
    real pos = scale::coord(px, 0, n2-1, 0, n1-1); // получить положение в исходном векторе
    int start = t_max<int>(0, int_cast(ceil(pos-r)));
    int stop = t_min<int>(n1-1, int_cast(floor(pos+r)));
    real v = 0, w_sum = 0; // значение, сумма весов
    for( int i = start; i <= stop; ++i ){
      real w = lanczos_kernel(i-pos, r);
      w_sum += w;
      v += w * src[i];
    }
    dst[px] = (w_sum != 0) ? v/w_sum : 0.0;
  }
}

real lanczos::calc( const vector &src, real x, real r )
{
  if( r < 1 )
    runtime("bad radius (r=%lf) in lanczos::calc()", r);

  int len = src.len();
  if( x < 0 || x > len-1 )
    runtime("argument out of bounds in lanczos::calc(), x=%lf", x);

  int start = t_max<int>(0, int_cast(ceil(x-r)));
  int stop = t_min<int>(len-1, int_cast(floor(x+r)));
  real v = 0, w_sum = 0; // значение, сумма весов
  for( int i = start; i <= stop; ++i ){
    real w = lanczos_kernel(i-x, r);
    w_sum += w;
    v += w * src[i];
  }
  return (w_sum != 0) ? v/w_sum : 0.0;
}

// для ресемплинга используем промежуточную матрицу и вспомогательные векторы
// неоптимально, но позволяет написать сразу работающий вариант
void lanczos::warp( matrix &dst, const matrix &src, real r )
{
  matrix buf(src.str(), dst.col(), 0.0);

  { // проход по строкам
    vector str_src(src.col(), 0.0); // строка источник и назначение
    vector str_dst(dst.col(), 0.0);
    for( int s = 0; s < buf.str(); ++s ){
      src.get_str(str_src, s);
      warp(str_dst, str_src, r);
      buf.put_str(str_dst, s);
    }
  }

  { // проход по столбцам
    vector col_src(src.str(), 0.0); // столбец источник и назначение
    vector col_dst(dst.str(), 0.0);
    for( int c = 0; c < buf.col(); ++c ){
      buf.get_col(col_src, c);
      warp(col_dst, col_src, r);
      dst.put_col(col_dst, c);
    }
  }
}

// Вычисление значения в точке x для одномерного интерполянта в строке y.
// Никакие проверки корректности аргументов не выполняются
real lanczos::calc_in_line( const matrix &src, int y, real x, real r )
{
  int len = src.col();

  int start = t_max<int>(0, int_cast(ceil(x-r)));
  int stop = t_min<int>(len-1, int_cast(floor(x+r)));
  real v = 0, w_sum = 0; // значение, сумма весов
  for( int i = start; i <= stop; ++i ){
    real w = lanczos_kernel(i-x, r);
    w_sum += w;
    v += w * src(y, i);
  }
  return (w_sum != 0) ? v/w_sum : 0.0;
}

real lanczos::calc( const matrix &src, real y, real x, real r )
{
  int len = src.str();

  int start = t_max<int>(0, int_cast(ceil(y-r)));
  int stop = t_min<int>(len-1, int_cast(floor(y+r)));
  real v = 0, w_sum = 0; // значение, сумма весов
  for( int i = start; i <= stop; ++i ){
    real w = lanczos_kernel(i-y, r);
    w_sum += w;
    v += w * calc_in_line(src, i, x, r);
  }
  return (w_sum != 0) ? v/w_sum : 0.0;
}

}; // namespace resample
