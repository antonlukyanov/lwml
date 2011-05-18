// Продвинутая целочисленная геометрия
// lwml, (c) ltwood

#ifndef _IGEOMX_
#define _IGEOMX_

#include "defs.h"
#include "mdefs.h"
#include "igeom.h"

/*#lake:stop*/

namespace lwml {

// Класс реализует построение аппроксимации прямой линии на целочисленной плоскости.
// Используется несимметричный вариант т.н. "цифрового дифференциального анализатора"
// (Digital Differential Analyzer, DDA).
// На каждом шаге алгоритм гарантирует единичное смещение вдоль
// наиболее быстро меняющейся координаты.
//
// Клиентский код определяет наследника класса i_pixel_map,
// в котором функция pixel(x, y, idx, w) вызывается для каждой точки отрезка.
// Аргумент idx определяет номер точки на линии.
// Аргумент w всегда принимает значение 1.0 при вызове из функции line().
// При вызове из функции line_sp() аргумент w принимает значения от 0.0 до 1.0
// в зависимости от веса ("яркости") пиксела в субпиксельной аппроксимации прямой.
//
// Функция line() отрисовывает "обычную" линию с заданными координатами концов.
// Функция line_sp() реализует субпиксельную модификацию алгоритма DDA (автор Xiaolin Wu).
// Параметр close определяет надо ли включать в линию конечную точку.
//
// Функция line_sp() обладает рядом особенностей:
//  -- она не гарантирует, что функция pixel() будет вызываться только
//     для точек из прямоугольника, ограничивающего отрезок.
//  -- она не гарантирует точное соблюдение режима close
//     (этот флаг трактуется как рекомендация).

class i_pixel_map : public interface {
public:
  virtual void pixel( int x, int y, int idx, real w = 1.0 ) = 0;
};

class linegen : public scope {
public:
  enum closure { OPEN_END, CLOSED_END };

  static int  calc_len( real x1, real y1, real x2, real y2, closure cl = OPEN_END );
  static void line( i_pixel_map&, real x1, real y1, real x2, real y2, closure cl = OPEN_END );
  static void line_sp( i_pixel_map& pm, real x1, real y1, real x2, real y2, closure cl = OPEN_END );
};

// Класс реализует операции с аппроксимацией круга
// с центром (x0, y0) и радиусом r на целочисленной решетке
// Класс содержит только статические функции
// Функция is_in() проверяет пиксел на принадлежность кругу
// с оптимальной аппроксимацией на границе
// Функция is_in_in() реализует ту же проверку, но не позволяет
// пикселам пересекать границу круга
// Функция is_in_out() реализует ту же проверку, но полностью
// покрывает границу круга пикселами
// Функция weight() дает долю площади пиксела, попавшей в круг

class circle : public scope {
public:
  static bool is_in( int x, int y, int x0, int y0, real r );
  static bool is_in_in( int x, int y, int x0, int y0, real r );
  static bool is_in_out( int x, int y, int x0, int y0, real r );
  static real weight( int x, int y, int x0, int y0, real r );

private:
  static void conv( int& x, int& y, int x0, int y0 );
};

}; // namespace lwml

#endif // _IGEOMX_
