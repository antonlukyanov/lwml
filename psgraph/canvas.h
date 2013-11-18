// Абстрактный класс для устройств вывода графики
// lwml, (c) ltwood

#ifndef _CANVAS_
#define _CANVAS_

#include "defs.h"
#include "geom.h"
#include "rgb.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// Тип используемой цветовой схемы

class color_scheme : public value {
  enum { csGRAY = 0, csRGB = 1, csHSB = 2 };
public:
  // Установка умалчиваемой цветовой схемы
  // Умалчиваемой схемой считается схема grayscale.
  color_scheme() : _type(csGRAY) {}

  // Установка цветовой схемы grayscale (градации серого)
  void set_gray() { _type = csGRAY; }

  // Установка цветовой схемы RGB
  void set_rgb()  { _type = csRGB; }

  // Установка цветовой схемы HSB/HSV
  void set_hsb()  { _type = csHSB; }

  // Проверить, является ли grayscale текущей цветовой схемой
  bool is_gray() const { return _type == csGRAY; }

  // Проверить, является ли RGB текущей цветовой схемой
  bool is_rgb()  const { return _type == csRGB; }

  // Проверить, является ли HSB/HSV текущей цветовой схемой
  bool is_hsb()  const { return _type == csHSB; }

private:
  char _type;
};

// Типы выравнивания текста
// Выравнивание определяет расположение точки привязки по отношению к строке.
// При задании расположения текста задаются координаты точки привязки.
// Например выравнивание align().left().top() указывает, что точкой привязки будет
// считаться левый верхний угол прямоугольника, содержащего строку,
// а сама строка будет расположена _справа_ _снизу_ от точки привязки.

class align : public value {
  enum horz_align { haLEFT = 0, haCENTER = 1, haRIGHT = 2 };
  enum vert_align { vaBOT = 0, vaBASE = 1, vaCENTER = 2, vaTOP = 3 };
public:
  // Установить умалчиваемое выравнивание
  // Умалчиваемым считается выравнивание, соответствующее
  // расположению точки привязки слева внизу.
  align() : _horz(haLEFT), _vert(vaBOT) {}

  // Переместить точку привязки к левому краю
  align& left()    { _horz = haLEFT; return *this; }
  // Переместить точку привязки к центру по горизонтали
  align& hcenter() { _horz = haCENTER; return *this; }
  // Переместить точку привязки к правому краю
  align& right()   { _horz = haRIGHT; return *this; }
  // Переместить точку привязки к нижнему краю
  align& bot()     { _vert = vaBOT; return *this; }
  // Переместить точку привязки к базовой линии
  align& base()    { _vert = vaBASE; return *this; }
  // Переместить точку привязки к центру по вертикали
  align& vcenter() { _vert = vaCENTER; return *this; }
  // Переместить точку привязки к верхнему краю
  align& top()     { _vert = vaTOP; return *this; }

  // Проверить, расположена ли точка привязки слева
  bool is_left()    const { return _horz == haLEFT; }
  // Проверить, расположена ли точка привязки в центре по горизонтали
  bool is_hcenter() const { return _horz == haCENTER; }
  // Проверить, расположена ли точка привязки справа
  bool is_right()   const { return _horz == haRIGHT; }
  // Проверить, расположена ли точка привязки внизу
  bool is_bot()     const { return _vert == vaBOT; }
  // Проверить, расположена ли точка привязки на базовой линии
  bool is_base()    const { return _vert == vaBASE; }
  // Проверить, расположена ли точка привязки в центре по вертикали
  bool is_vcenter() const { return _vert == vaCENTER; }
  // Проверить, расположена ли точка привязки вверху
  bool is_top()     const { return _vert == vaTOP; }

private:
  char _horz, _vert;
};

// Абстрактный класс для устройств вывода графики.
// Наследники переопределяют все чистые виртуальные функции рисования.
//
// Рисование происходит в виртуальных вещественных координатах.
// Границы прямоугольника сцены определяются устройством.
// Поведение при выходе за границы сцены определяется устройством.
//
// Единственная собственная функциональность этого класса -
// реализация на базе функции line() простой "квазичерепашьей" графики
// с действиями reset() и point().
// point() позволяет рисовать ломаную поточечно,
// причем при рисовании первой точки перо просто премещается в эту точку,
// а при рисовании последующих точек происходит рисование звеньев.
// reset() сбрасывает флаг продолжения ломаной и приводит к рисованию
// новой линии.
//
// - Высота шрифта определяется устройством и используется как
// "относительная единица длины" - "relative length unit" - RLU.
// - Треугольник и грань используются при рисовании поверхностей.
// - Функции установки текущего цвета влияют на все операции рисования,
// за исключением операции заливки внутренности грани (facet).
//
// Функция special() позволяет передать устройству специфичную для него
// команду, состоящую из координат, целого числа (идентификатор команды)
// и строки (char*) с параметрами команды.

class i_canvas : public interface, public refcount {
public:
  // Инициализация механизма "черепашьей" графики.
  i_canvas() { reset(); }

  // Получить прямоугольник клиентской сцены,
  // в которой происходит рисование
  virtual frect boundrect() const = 0;

  // Получить x-размер RLU в координатах сцены
  virtual real xunit() const = 0;

  // Получить y-размер RLU в координатах сцены
  virtual real yunit() const = 0;

  // Нарисовать пиксел
  // Для векторных устройст, не поддерживающих понятие пиксела,
  // функция может быть реализована путем рисования "маленькой"
  // окружности или круга.
  virtual void pixel( real x, real y ) = 0;

  // Нарисовать "маркер" - альтернативную пометку точки
  // Вид и размер маркера зависит от устройста.
  virtual void marker( real x, real y ) = 0;

  // Нарисовать линию
  virtual void line( real x1, real y1, real x2, real y2 ) = 0;

  // Нарисовать блок
  // Блок рисуется как залитый прямоугольник.
  virtual void box( real x1, real y1, real x2, real y2 ) = 0;

  // Нарисовать прямоугольник
  // Прямоугольник рисуется как контур
  virtual void rectangle( real x1, real y1, real x2, real y2 ) = 0;

  // Нарисовать треугольник
  // Треугольник рисуется с заливкой текущим цветом
  virtual void triangle(
    real x1, real y1, real x2, real y2, real x3, real y3
  ) = 0;

  // Нарисовать "грань" (facet)
  // "Грань" рисуется как треугольник с заливкой цветом фона (белым)
  virtual void facet(
    real x1, real y1, real x2, real y2, real x3, real y3
  ) = 0;

  // Установка текущего цвета в цветовой схеме grayscale
  virtual void set_gray( real graycol ) = 0;
  // Установка текущего цвета в цветовой схеме RGB
  virtual void set_rgb( rgbcolor rgbcol ) = 0;
  // Установка текущего цвета в цветовой схеме HSB
  virtual void set_hsb( hsbcolor hsbcol ) = 0;

  // Вывод строки в заданной позиции с заданным выравниванием
  // algn - флаги выравнивания
  // alpha - угол, определяющий направление вывода строки
  virtual void puts(
    real x, real y, const char* str, const align& algn, real alpha
  ) = 0;

  // Передать устройству специфическую для него команду
  // id - целочисленный идентификатор команды
  // cmd - текстовая команда
  virtual void special( real x, real y, int id, const char* cmd ) = 0;

  // Начать рисование новой ломаной в "квазичерепашьей" графике
  void reset();

  // Отрисовать очередную точку ломаной в "квазичерепашьей" графике
  void point( real x, real y );

private:
  bool _newflag;             // moveto/lineto flag for point()
  real _cx, _cy;             // current point for point()
};

}; // namespace lwml

#endif // _CANVAS_
