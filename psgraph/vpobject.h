// Инструменты рисования объектов
// lwml, (c) ltwood

#ifndef _VPOBJECT_
#define _VPOBJECT_

//!! TODO: убрать выполнение рисования в конструкторах
//!! TODO: проверить работу с абсолютной ошибкой

#include "defs.h"
#include "viewport.h"
#include "vector.h"
#include "matrix.h"
#include "histogram.h"
#include "geom3d.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// Каждый инструмент при конструировании получает канву,
// в которой происходит отрисовка
// По этой канве создается собственная канва, удобная для
// рисования объекта
// Созданную канву можно получить, вызвав метод vp()
// Рисование в канве, полученной таким образом предполагает
// наличие знаний об устройстве инструмента, поэтому оно
// нарушает инкапсуляцию

// Класс viewgrid реализует отрисовку в канве клеточной структуры,
// состоящей из ny строк и nx столбцов цветных прямоугольных ячеек.
// Нумерция по вертикали по умолчанию начинается сверху.
// Обратный порядок можно установить при конструировании флажком dir.
// Каждая ячейка может быть отрисована как контур или залита цветом.
// Текущий цвет отрисовки может быть установлен функциями set_XXX().
// Также можно отрисовать серую сетку по границам ячеек.
// Созданный viewport имеет координаты (0, nx), (0, ny).
// Ячейки занимают клетки с целочисленными границами.

class viewgrid : public value {
public:
  enum direction { NORMAL, REVERSE };

  // клеточная структура из ny строк и nx столбцов ячеек
  viewgrid( referer<i_canvas> vp, int nx, int ny, direction dir = NORMAL );

  // установить текущий цвет
  void set_gray( real col )     { _vp->set_gray(col); }
  void set_rgb( rgbcolor col )  { _vp->set_rgb(col); }
  void set_hsb( hsbcolor col )  { _vp->set_hsb(col); }

  // отрисовать ячейку в строке y и столбце x
  void box( int x, int y );   // с заливкой
  void rect( int x, int y );  // как контур

  // отрисовать серую сетку по границам ячеек
  void show_grid( real dens = 0.75 );

  // получить канву клеточной структуры
  referer<viewport> vp() { return _vp; }

private:
  referer<viewport> _vp;
  bool _revers;
  int _nx, _ny;
};

// класс vector_as_line отрисовывает вектор в виде графика
// Созданный viewport имеет координаты (0, x.len()-1), (x.min(), x.max())

class vector_as_line : public value {
public:
  // отрисовать график в канве
  vector_as_line( referer<i_canvas> cv, const vector& x );

  // получить канву графика
  referer<viewport> vp() { return _vp; }

private:
  referer<viewport> _vp;

  real correct_min( const vector& );
  real correct_max( const vector& );

  void draw( const vector& );
};

// Класс colorer - вспомогательный для класса matrix_as_grayscale.
// Он служит для преобразования значения элемента матрицы в цвет.
// Оператор operator() получает значение элемента матрицы,
// отнормированное на диапазон [0,1] и преобразует его в цвет в модели HSB.

class i_colorer : public interface {
public:
  // преобразование значения в цвет
  virtual hsbcolor operator()( real val ) const = 0;
};

// Простейший colorer, преобразующий значения в цвет.
// При конструировании задается тон и насыщенность получаемого на выходе цвета,
// а для яркости задается диапазон, в который линейно преобразуется входное значение.
// Флаг revers позволяет обратить яркости: если этот флаг включен,
// то большим значениям соответствуют темные цвета.
// Пергрузка функции mk_color() позволяет задать зависимость тона и насыщенности
// от получаемого значения элемента матрицы.

const real std_simple_colorer_blackcolor = 0.1;
const real std_simple_colorer_whitecolor = 0.9;

const real COL_POW = 0.5;

class simple_colorer : public i_colorer, public value {
public:
  enum color_mode { POSITIVE, NEGATIVE };

  // для цвета задается тон, насыщенность и диапазон изменения яркости
  // дополнительно задается флаг реверсирования яркостей
  simple_colorer(
    real hue = 0.0,
    real satur = 0.0,
    real bc = std_simple_colorer_blackcolor,
    real wc = std_simple_colorer_whitecolor,
    color_mode cm = POSITIVE
  ){
    _hue = hue;
    _satur = satur;
    _bc = bc;
    _wc = wc;
    _revers = (cm == NEGATIVE);
  }

  virtual hsbcolor operator()( real val ) const {
    real cb = pow(val, COL_POW);
    hsbcolor res = mk_color(val);
    if( _revers )
      res.set_brightness(scale::coord(cb, 0.0, 1.0, _wc, _bc));
    else
      res.set_brightness(scale::coord(cb, 0.0, 1.0, _bc, _wc));
    return res;
  }

  virtual hsbcolor mk_color( real val ) const {
    return hsbcolor(_hue, _satur, 0.0);
  }

private:
  real _hue, _satur;
  real _bc, _wc;
  bool _revers;
};

// Класс matrix_as_grayscale отрисовывает матрицу в виде
// картинки в градациях серого.
// Строки располагаются горизонтально, столбцы - вертикально.
// Нумерция по вертикали по умолчанию начинается сверху.
// Обратный порядок можно установить при конструировании флажком dir.
// При конструировании можно указать специфическое правило отображения
// значения в цвет, передав наследника класса colorer.
// Отрисовка может занимать много времени, поэтому передав прогресс-индикатор,
// можно получить управление между отрисовкой отдельных строк.
// Созданный viewport имеет координаты (0, m.col()), (0, m.str()).
// Элементам матрицы соответствуют клетки с целочисленными границами.

class matrix_as_grayscale : public value {
public:
  enum direction { NORMAL, REVERSE };

  // отрисовать матрицу m в градациях серого
  matrix_as_grayscale(
    referer<i_canvas> cv, const matrix& m, 
    direction dir = NORMAL,
    const i_colorer& col = simple_colorer(), 
    tick_mode tick = tmOFF
  );

  // получить канву картинки
  referer<viewport> vp() { return _vp; }

private:
  referer<viewport> _vp;

  void draw( const matrix&, bool revers, const i_colorer&, tick_mode tick );
};

// Класс matrix_as_constline рисует линии уровня функции,
// заданной отсчетами, расположенными в матрице.
// Строки располагаются горизонтально, столбцы - вертикально.
// Нумерция по вертикали по умолчанию начинается сверху.
// Обратный порядок можно установить при конструировании флажком dir.
// При конструировании указываются только размеры матрицы.
// Сама матрица передается функции рисования линии уровня.
// При конструировании можно также передать относительную погрешность,
// используемую при идентификации вырожденных (горизонтальных) граней.
// Отрисовка линии уровня может занимать много времени, поэтому
// передав прогресс-индикатор, можно получить управление
// между отрисовкой отдельных строк.
// Можно отрисовать серую сетку по границам ячеек.
// Созданный viewport имеет координаты (0, m.col()-1), (0, m.str()-1).
// Элементам матрицы соответствуют _точки_ с целочисленными границами.

const real std_matrix_as_constline_eps = 1e-6;

class matrix_as_constline : public value {
public:
  enum direction { NORMAL, REVERSE };

  // подготовить рисовалку линий уровня
  matrix_as_constline(
    referer<i_canvas> cv, int str, int col,
    direction dir = NORMAL,
    real eps = std_matrix_as_constline_eps
  );

  // отрисовать линию уровня на высоте c
  void draw( const matrix& m, real c, tick_mode tick = tmOFF );

  // отрисовать серую сетку по границам ячеек
  void show_grid( real dens = 0.75 );

  // получить канву картинки
  referer<viewport> vp() { return _vp; }

private:
  referer<viewport> _vp;
  int _str, _col;
  bool _revers;
  real _eps;

  void cline( fpoint3d p1, fpoint3d p2, real c );
  void trcline( fpoint3d p1, fpoint3d p2, fpoint3d p3, real c );
};

}; // namespace lwml

#endif // _VPOBJECT_
