// Поддержка CHR-шрифта

#ifndef _FONT_
#define _FONT_

#include "defs.h"
#include "canvas.h"
#include "refcount.h"

/*#lake:stop*/

// Класс осуществляет поддержку отрисовки символов
// из CHR-шрифта в абстрактной графической канве
// Целевая канва указывается в момент рисования.
// Метод width() позволяет вычислить ширину строки символов
// по заданной высоте шрифта
// Методы puts() выводит строку в заданой позиции
// шрифтом заданной высоты
// Указывается также необязательное выравнивание (см. canvas.h)
// и угол поворота строки (поворот осуществляется относительно
// точки привязки, задаваемой выравниванием)

namespace lwml {

class font : public value {
public:
  font();

  real width( const char *str, real height ) const;

  void puts(
    referer<i_canvas> pcv,
    real x, real y, const char *str, real height,
    const align& ta = align(), real alpha = 0.0
  );

private:
  int  _top, _bot;   // размер шрифта в единицах шрифта
  real _x0, _y0;     // точка привязки строки
  real _cx, _cy;     // текущая точка отрисовки

  real mult( real height ) const;

  void moveto( real x, real y );
  void lineto( referer<i_canvas> pcv, real x, real y );

  real width( char, real height ) const;

  void put_char( referer<i_canvas> pcv, char ch, real height, real alpha );
};

}; // namespace lwml

#endif // _FONT_
