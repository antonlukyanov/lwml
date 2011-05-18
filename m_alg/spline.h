// Cubic spline
// lwml, (c) ltwood

#ifndef _SPLINE_
#define _SPLINE_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"
#include "gj3d.h"
#include "matrix.h"

/*#lake:stop*/

namespace lwml {

// Интерполяция кубическим сплайном

class spline : public value {
public:
  spline( int sz );

  // Рассчитать коэффициенты сплайна для x, y
  // der_l, der_r задают значения второй производной на концах отрезка
  void put( const vector& x, const vector& y, real der_l, real der_r );

  // Вычислить значение сплайна в т. xx. Если xx окажется вне диапазона
  // ранее указанного вектора x, генерируется ошибка.
  real calc( real xx ) const {
    return calc_imp(_x, _y, _coeff, xx);
  }

  // Получить вектор коэффициентов для рассчитанного сплайна.
  // Длина массива коэффициентов равна 4*(sz-1) в порядке:
  // a_0, b_0, c_0, d_0, a_1, b_1, c_1, d_1, ...
  const vector& coeff() const { return _coeff; }

  // Вычислить значение сплайна в т. xx по коэффициентам и данным
  static real calc( const vector& x, const vector& y, const vector& coeff, real xx );

private:
  class size_checker : value {
  public:
    size_checker( int sz ) {
      if( sz < 4 )
        fail_size(sz);
    }
  };
  size_checker _szcheck;  // нужно, чтобы проверка выполнилась до вызова
                          // конструкторов векторов
  int _size;
  vector _x, _y, _coeff;
  gj3d _gj;

  static real calc_imp( const vector& x, const vector& y, const vector& coeff, real xx );
};

/* Интерполяция бикубическим сплайном.
   Построен на основе spline.
   На концах задается нулевое значение второй производной.
   Неэффективен! В текущем виде предназначен скорее для вычислений
   отдельных произвольных значений, чем для ресемплинга изображений.
*/

class spline2d : public value {
public:
  spline2d( int str, int col );

  void put( const vector& x, const vector& y, const matrix& z );

  real calc( real xx, real yy ) const;
private:
  int _str, _col;
  vector _x, _y;  // значения x, y, задающие сетку
  matrix _z;      // значения функции в узлах сетки
  matrix _col_cs; // коэффиценты сплайнов, расчитанных по столбцам
  mutable vector _buf_str; // буфер для строки значений
  mutable vector _buf_col;
  mutable vector _buf_col_cs;
  mutable spline _sp_str;  // сплайн по строке
};

}; // namespace lwml

#endif // _SPLINE_
