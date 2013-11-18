// External mini-canvas for stupid GUI applications
// lwml, (c) ltwood

#ifndef _MINICV_
#define _MINICV_

//!! NB: особенность:
// Этот заголовок не включает заголовки defs.h, mdefs.h только по соображениям совместимости 
// для обеспечения возможности использования этого заголовка отдельно от окружения lwml.

/*#lake:stop*/

//!! NB: особенность:
// Здесь отсутствует пространство имен lwml только по соображениям совместимости для
// обеспечения возможности использования этого заголовка отдельно от окружения lwml.

#define HALIGN(al) ((al) & 0x00ff)
#define VALIGN(al) (((al) & 0xff00)>>8)

//!! NB: особенность:
// Этот класс не унаследован от класса interface только по соображениям совместимости для
// обеспечения возможности использования этого заголовка отдельно от окружения lwml.

class minicv {
public:
  enum {
    haLEFT = 0, haCENTER = 1, haRIGHT = 2,
    vaBOT = 0, vaBASE = 1, vaCENTER = 2, vaTOP = 3
  };

  virtual ~minicv() {}

  // Получить прямоугольник клиентской сцены,
  // в которой происходит рисование
  virtual void boundrect( double& ax, double& bx, double& ay, double& by ) const = 0;

  // Получить размеры RLU (see canvas.h) в координатах сцены
  virtual double xunit() const = 0;
  virtual double yunit() const = 0;

  // Нарисовать пиксел
  // Для векторных устройст, не поддерживающих понятие пиксела,
  // функция может быть реализована путем рисования "маленькой"
  // окружности или круга.
  virtual void pixel( double x, double y ) = 0;

  // Нарисовать "маркер" - альтернативную пометку точки
  // Вид и размер маркера зависит от устройста.
  virtual void marker( double x, double y ) = 0;

  // Нарисовать линию
  virtual void line( double x1, double y1, double x2, double y2 ) = 0;

  // Нарисовать блок
  // Блок рисуется как залитый прямоугольник.
  virtual void box( double x1, double y1, double x2, double y2 ) = 0;

  // Нарисовать прямоугольник
  // Прямоугольник рисуется как контур
  virtual void rectangle( double x1, double y1, double x2, double y2 ) = 0;

  // Нарисовать треугольник
  // Треугольник рисуется с заливкой текущим цветом
  virtual void triangle(
    double x1, double y1, double x2, double y2, double x3, double y3
  ) = 0;

  // Нарисовать "грань" (facet)
  // "Грань" рисуется как треугольник с заливкой цветом фона (белым)
  virtual void facet(
    double x1, double y1, double x2, double y2, double x3, double y3
  ) = 0;

  // В следующих функциях интенсивности цветовых компонент представлены
  // вещественными числами из интервала [0,1]

  // Установка текущего цвета в цветовой схеме grayscale
  virtual void set_gray( double graycol ) = 0;
  // Установка текущего цвета в цветовой схеме RGB
  virtual void set_rgb( double r, double g, double b ) = 0;

  // Вывод строки в заданной позиции с заданным выравниванием
  // algn - флаги выравнивания
  // alpha - угол, определяющий направление вывода строки
  virtual void puts(
    double x, double y, const char* str, unsigned int algn, double alpha
  ) = 0;

  // Передать устройству специфическую для него команду
  // id - целочисленный идентификатор команды
  // cmd - текстовая команда
  virtual void special( double x, double y, int id, const char* cmd ) = 0;
};

#endif // _MINICV_
