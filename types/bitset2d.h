// Двумерный массив битов.
// lwml, (c) ltwood

#ifndef _BITSET2D_
#define _BITSET2D_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/types/bitset.h"

/*#lake:stop*/

namespace lwml {

class bitset2d : public value {
public:
  // Создать двумерное битовое множество размером xsz на ysz бит, установив каждый элемент в v.
  bitset2d( int xsz, int ysz, bool v = false )
    : _xsize(xsz), _ysize(ysz), _bits(xsz * ysz, v)
  {
    double sz = real_cast(xsz) * real_cast(ysz);
    if( sz > INT_MAX )
      fail_assert("too large bitset2d");
  }

  // Импортировать значение множества из строки байтов.
  void char_read( const char* buf ) { _bits.char_read(buf); }

  // Экспортировать значение множества в строку байтов.
  void char_write( char* buf ) const { _bits.char_write(buf); }

  // Возвращает число байтов в представляющей множестве строке байтов.
  int char_num() const { return _bits.len(); }

  // Получение геометрических размеров множества.
  int lx() const { return _xsize; }
  int ly() const { return _ysize; }

  // Сбросить все биты множества в ноль.
  void set_zero() { _bits.set_zero(); }

  // Установить все биты множества в единицу.
  void set_one() { _bits.set_one(); }

  // Установить бит в позиции (x, y).
  void set( int ix, int iy ) {
    TEST_INDEX2(ix, _xsize, iy, _ysize);
    _bits.set(idx2lidx(ix, iy));
  }

  // Установить биты в прямоугольнике.
  void set( int ax, int bx, int ay, int by );

  // Сбросить бит в позиции (x, y).
  void clear( int ix, int iy ) {
    TEST_INDEX2(ix, _xsize, iy, _ysize);
    _bits.clear(idx2lidx(ix, iy));
  }

  // Сбросить биты в прямоугольнике.
  void clear( int ax, int bx, int ay, int by );

  // Инвертировать бит в позиции (x, y).
  void flip( int ix, int iy ) {
    TEST_INDEX2(ix, _xsize, iy, _ysize);
    _bits.flip(idx2lidx(ix, iy));
  }

  // Инвертировать биты в прямоугольнике.
  void flip( int ax, int bx, int ay, int by );

  // Получить значение бита в позиции (x, y).
  bool test( int ix, int iy ) const {
    TEST_INDEX2(ix, _xsize, iy, _ysize);
    return _bits.test(idx2lidx(ix, iy));
  }

  // Проверить множества на пересечение.
  // Возвращает true если множество пересекается с множеством ls.
  bool is_meet( const bitset2d& ls ) const {
    TEST_INDEX2(ls.lx(), _xsize, ls.ly(), _ysize);
    return _bits.is_meet(ls._bits);
  }

  // Вычисление объединения множеств.
  // Добавляет множество-аргумент к текущему множеству т.е.
  // устанавливает в множестве все биты, установленные в множестве ls.
  // Возвращает ссылку на измененное исходное множество.
  bitset2d& join( const bitset2d& ls ) {
    TEST_INDEX2(ls.lx(), _xsize, ls.ly(), _ysize);
    _bits.join(ls._bits);
    return *this;
  }

  // Вычисление пересечения множеств.
  // Пересекает текущее множество с множеством-аргументом т.е.
  // оставляет установленными только биты, установленные в множестве ls.
  // Возвращает ссылку на измененное исходное множество.
  bitset2d& meet( const bitset2d& ls ) {
    TEST_INDEX2(ls.lx(), _xsize, ls.ly(), _ysize);
    _bits.meet(ls._bits);
    return *this;
  }

  // Возвращает true, если все биты находятся в состоянии false.
  bool is_empty() const {
     return _bits.is_empty();
  }

private:
  int idx2lidx( int32 x, int32 y ) const { return x * _ysize + y; }

  int _xsize, _ysize;
  bitset _bits;
};

}; // namespace lwml

#endif // _BITSET2D_
