// Загрузчик библиотеки limlib
// lwml, (c) ltwood

#ifndef _LIMLIB_
#define _LIMLIB_

#include "defs.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

// Обертка над библиотекой limlib.dll, поддерживающей
// загрузку полутоновых изображений в форматах tiff и jpeg
// и сохранение полутонового изображения в формате jpeg.
// Поддерживает автоматическую загрузку библиотеки при первом вызове.
// Успешность загрузки можно проверить функцией is_active(),
// которая при необходимости сама пытается загрузить библиотеку.
// Если библиотека не загрузилась, то вызов любой функции генерирует assert.
// Класс не обрабатывает ошибки, возвращаемые библиотекой.

class limlib : public scope {
public:
  static bool is_active() { return init(); }

  static bool getsize( const char* fn, int& lx, int& ly );
  static bool load( const char* fn, uchar* data );
  static bool load_rgb( const char* fn, uchar* data );
  static bool save( const char* fn, int lx, int ly, const uchar* data );

  static const char* errmsg();

private:
  static bool init();
  static void test();
};

}; // namespace lwml

#endif // _LIMLIB_
