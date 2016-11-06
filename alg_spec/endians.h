// Межсистемное endian-конвертирование чисел.
// lwml, (c) ltwood

#ifndef _ENDIANS_
#define _ENDIANS_

#include "defs.h"

/*#lake:stop*/

namespace lwml {

// Реализует переносимые функции для работы с двоичными данными,
// записанными на системах с различными соглашениями о порядке байтов.
// Используется при межсистемном переносе двоичных данных.

// Широко распространены 2 основных способа упорядочения байтов при
// записи в память: Big Endian (BE) и Little Endian (LE) и еще один
// промежуточный используемый исключительно на машинах серии DEC VAX
// формат Middle Endian (ME).
// Процессоры intel используют порядок Little Endian,
// а процессоры motorola - Big Endian.
// При Big Endian упорядочении по младшим адресам памяти записывается
// наиболее значимые байты, а при Little Endian - наоборот.
// Функции Xe2Ye_nn конвертируют число из упорядочения X в упорядочение Y.
// Функции to_Xe_nn конвертируют число из текущего упорядочения в упорядочение X.
// Функции from_Xe_nn конвертируют число из упорядочения X в текущее упорядочение.
// Суффикс nn определяет число битов в конвертируемом числе.

class endian : public scope {
public:
  // конвертирование 32-битных чисел
  static uint32 le2be_32( uint32 );
  static uint32 be2le_32( uint32 );
  static uint32 le2me_32( uint32 );
  static uint32 me2le_32( uint32 );
  static uint32 me2be_32( uint32 );
  static uint32 be2me_32( uint32 );

  static uint32 to_le_32( uint32 );
  static uint32 to_be_32( uint32 );
  static uint32 to_me_32( uint32 );

  static uint32 from_le_32( uint32 );
  static uint32 from_be_32( uint32 );
  static uint32 from_me_32( uint32 );

  // конвертирование 16-битных чисел
  static uint16 le2be_16( uint16 x ) { return swab(x); }
  static uint16 be2le_16( uint16 x ) { return swab(x); }
  static uint16 le2me_16( uint16 x ) { return x; }
  static uint16 me2le_16( uint16 x ) { return x; }
  static uint16 me2be_16( uint16 x ) { return swab(x); }
  static uint16 be2me_16( uint16 x ) { return swab(x); }

  static uint16 to_le_16( uint16 );
  static uint16 to_be_16( uint16 );
  static uint16 to_me_16( uint16 );

  static uint16 from_le_16( uint16 );
  static uint16 from_be_16( uint16 );
  static uint16 from_me_16( uint16 );

private:
  static uint16 swab( uint16 );
};

}; // namespace lwml

#endif // _ENDIANS_
