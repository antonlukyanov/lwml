// Шрифт, импортированный из формата CHR (BGI) конвертером CHR2C.
// Сгенерированный C-файл модифицирован вручную.
// lwml, (c) ltwood

#ifndef _CHRFONT_
#define _CHRFONT_

#include "defs.h"

/*#lake:stop*/

namespace lwml {

// Класс содержит данные CHR-шрифта
// Интерпретацией данных и отрисовкой шрифта занимается класс font.

class chrfont : public scope {
public:
  // Структура, представляющая точку в начертании шрифта.
  // Начертание каждого символа представляет собой
  // последовательность точек.
  typedef struct {
    signed char t; // тип точки
    signed char x; // x-координата
    signed char y; // y-координата
  } stroke_type;

  static const char name[];        // имя шрифта
  static const char uppermargin;   // высота шрифта
  static const char baseline;      // положение базовой линии
  static const char descender;     // глубина шрифта

  static const unsigned char width[];   // ширины символов
  static const signed short  offset[];  // смещение начертания символа в общей таблице
  static const stroke_type   stroke[];  // общая таблица начертаний символов
};

}; // namespace lwml

#endif // _CHRFONT_
