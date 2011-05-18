// Вычисление контрольной суммы CRC32
// lwml, (c) ltwood

#ifndef _CRC32_
#define _CRC32_

#include "defs.h"

/*#lake:stop*/

namespace lwml {

// Класс реализует инкрементное вычисление контрольной суммы.
// Алгоритм можно использовать как для блоков двоичных данных,
// так и для последовательно поступающих байтов.

class crc32 : public value {
public:
  // Инициализация инкрементного счетчик контрольной суммы
  crc32() { _crc = static_cast<uint32>(-1); }

  // Обновление контрольной суммы по очередному байту потока двоичных данных.
  void put( char ch ){
    uchar idx = static_cast<uchar>(_crc) ^ static_cast<uchar>(ch);
    _crc = CRC32table[idx] ^ (_crc >> 8);
  }

  // Обновление контрольной суммы по очередному блоку двоичных данных
  // ptr - указатель на блок данных
  // size - размер блока данных
  void put( const char *ptr, int size );

  // Получить текущее значение контрольной суммы
  // Возвращает текущее значение контрольной суммы
  uint32 get() const { return ~_crc; }

private:
  uint32 _crc;

  static const int32 CRC32table[256];
};

}; // namespace lwml

#endif // _CRC32_
