// Точечное изображение как матрица.
// lwml, (c) ltwood

#ifndef _VBMP_
#define _VBMP_

#include "defs.h"
#include "mdefs.h"
#include "bmp.h"
#include "matrix.h"
#include "imatrix.h"
#include "t_membuf.h"
#include "t_membuf2d.h"
#include "rgb.h"

/*#lake:stop*/

namespace lwml {

// Внутренняя ошибка библиотеки limlib

DEF_EX_TYPE_MSG(ex_integrity, ex_limlib, "limlib error");

// Класс vbmp содержит статические функции, реализующие представление
// точечного изображения в виде матрицы яркостей точек.
// Функция get() копирует изображение в передаваемый ей контейнер.
// Функции put() позволяют загрузить изображение из контейнера.
// По умолчанию минимальным значениям в матрице соответствует черный цвет.
// Обратный порядок цветов можно установить, вызвав функцию col_rev().
// По умолчанию нулевая строка матрицы представляет верхнюю линию на изображении.
// Обратный порядок строк можно установить, вызвав функцию revers().
// При загрузке изображения в буфер происходит автоматическая подстройка размера буфера
// под размер изображения и возвращается ширина изображения.
// При загрузке изображения в буфер не производится масштабирования,
// т.е. диапазон значений соответствует диапазону яркостей изображения.
// При загрузке изображения из контейнера по умолчанию производится
// масштабирование в полный диапазон яркостей изображения.
// Методы с суффиксом noscale не производят масштабирования,
// но проверяют диапазон значений перед загрузкой изображения.

class vbmp : public value {
public:
  vbmp() { _lx = _ly = 0; }
  vbmp( int ly, int lx );
  vbmp( const char* fn );

  void resize( int ly, int lx );

  void read( const char* fn );
  void save( const char* fn ) const;

  int len() const { return _img.len(); }
  int lx() const { return _lx; }
  int ly() const { return _ly; }

        uchar& operator[]( int idx )       { return _img[idx]; }
  const uchar& operator[]( int idx ) const { return _img[idx]; }

        uchar& operator()( int y, int x )       { return _img[_lx * y + x]; }
  const uchar& operator()( int y, int x ) const { return _img[_lx * y + x]; }

        uchar* access_raw_data()       { return _img.access_raw_data(); }
  const uchar* access_raw_data() const { return _img.access_raw_data(); }

  void fill( int v ) { _img.set_val(v); }

  // скопировать изображение в контейнер
  int get( t_membuf<uchar>& dst ) const;
  void get( t_membuf2d<uchar>& dst ) const;
  void get( matrix& dst ) const;
  void get( int_matrix& dst ) const;

  // загрузить изображение из контейнера
  void put( const t_membuf<uchar>& src, int lx );
  void put( const t_membuf2d<uchar>& src );
  void put( const matrix& );
  void put_noscale( const matrix& src );
  void put( const int_matrix& );
  void put_noscale( const int_matrix& src );

  void revers();
  void col_rev();

  enum color_mode { POSITIVE, NEGATIVE };
  enum direction { NORMAL, REVERSE };

  static void save( 
    const char* fn, const t_membuf<uchar>& src, int lx, 
    color_mode cm = POSITIVE, direction dir = NORMAL
  );
  static void save( 
    const char* fn, const t_membuf2d<uchar>& src,
    color_mode cm = POSITIVE, direction dir = NORMAL
  );
  static void save( 
    const char* fn, const matrix& src, 
    color_mode cm = POSITIVE, direction dir = NORMAL
  );
  static void save( 
    const char* fn, const int_matrix& src, 
    color_mode cm = POSITIVE, direction dir = NORMAL
  );

private:
  int _lx, _ly;
  t_membuf<uchar> _img;
};

}; // namespace lwml

#endif // _VBMP_
