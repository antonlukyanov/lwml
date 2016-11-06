// Операции с битовым представлением двоичных данных
// lwml, (c) ltwood

#ifndef _BITS_
#define _BITS_

#include "defs.h"
#include "lwmlconf.h"

/*#lake:stop*/

namespace lwml {

// В классе в виде статических функций реализованы основные нетривиальные
// операции, производимые над бинарным представлением чисел.
// Если макрос _SIGN_SHR_IS_ARITHM_ определен как 1, то функции,
// использующие сдвиг для знаковых чисел, полагаются
// на то, что реализация использует арифметический сдвиг.
// В противном случае арифметический сдвиг моделируется через логический.
// Бит четности принимает ненулевое значение при нечетном количестве
// ненулевых битов.
// При вычислении бита четности для больших блоков данных вычисления
// проволятся над каждым словом в отдельности и накапливаемый бит
// четности обновляется операцией '^='.
// Ссылки:
//   [1] http://graphics.stanford.edu/~seander/bithacks.html
//   [2] Henry S. Warren, Hacker's Delight
//       (Генри Уоррен мл., Алгоритмические трюки для программистов)
// В комментарии к каждой функции указана соответствующая ссылка
// (на самом деле все приемы описаны в [2]).

const int SIGN_BIT_POS = sizeof(int) * 8 - 1;

class bits : public scope {
public:
  // [2] Циклический сдвиг влево.
  static uint32 lrot( uint32 x, uint32 n )
  {
    return (x << n) | (x >> (32-n));
  }

  // [2] Циклический сдвиг вправо.
  static uint32 rrot( uint32 x, uint32 n )
  {
    return (x >> n) | (x << (32-n));
  }

  // [2] Моделирование арифметического сдвига вправо.
  static int32 shar( int32 x, uint32 n )
  {
#if _SIGN_SHR_IS_ARITHM_
    return x >> n;
#else
    int32 t = -(static_cast<uint32>(x) >> 31);
    return (static_cast<uint32>(x ^ t) >> n) ^ t;
#endif
  }

  // [2] Округление вниз до целого кратного p-й степени двойки.
  static uint32 dn2pow2mult( uint x, uint32 p )
  {
    return (x >> p) << p;
  }

  // [2] Округление вверх до целого кратного p-й степени двойки.
  static uint32 up2pow2mult( uint32 x, uint32 p )
  {
    uint32 t = (1 << p) - 1;
    return (x + t) & ~t;
  }

  // [1] (x < 0) ? -1 : 0
  static int32 is_neg( int32 x )
  {
    return shar(x, SIGN_BIT_POS);
  }

  // [1] (x < 0) ? -1 : +1
  static int32 sign( int32 x )
  {
    return +1 | shar(x, SIGN_BIT_POS);
  }

  // [1] (x < 0) ? -1 : ((x==0) ? 0 : +1)
  static int32 sign_zero( int32 x )
  {
    return (x != 0) | shar(x, SIGN_BIT_POS);
  }

  // [1] (x < 0) ? -x : x
  static int32 abs( int32 x )
  {
    return (x ^ shar(x, SIGN_BIT_POS)) - shar(x, SIGN_BIT_POS);
  }

  // [1] (x < y) ? x : y
  static int32 min( int32 x, int32 y )
  {
    return y + ((x - y) & shar(x - y, SIGN_BIT_POS));
  }

  // [1] (x > y) ? x : y
  static int32 max( int32 x, int32 y )
  {
    return x - ((x - y) & shar(x - y, SIGN_BIT_POS));
  }

  // [1] (x==2**k) || (x==0)
  static bool is_pow2_zero( uint32 x )
  {
    return (x & (x - 1)) == 0;
  }

  // [1] x==2**k
  static bool is_pow2( uint32 x )
  {
    return !(x & (x - 1)) && x;
  }

  // [2] Округление до ближайшей снизу степени двойки
  static uint32 dn2pow2( uint32 x )
  {
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >>16);
    return x - (x >> 1);
  }

  // [2] Округление до ближайшей сверху степени двойки
  static uint32 up2pow2( uint32 x )
  {
    x--;
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >>16);
    return x + 1;
  }

  // [2] Число ведущих нулей
  static uint32 nlz( uint32 x )
  {
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >>16);
    return count(~x);
  }

  // [2] Целочисленный логарифм с округлением вниз
  static uint32 log2dn( uint32 x )
  {
    return 31 - nlz(x);
  }

  // [2] Целочисленный логарифм с округлением вверх
  static uint32 log2up( uint32 x )
  {
    return 32 - nlz(x - 1);
  }

  // [1] if(cond) w |= m; else w &= ~m;
  static uint32 cond_bitset( bool cond, uint32 w, uint32 m )
  {
    return w ^ ((-cond ^ w) & m);
  }

  // [2] Подсчет числа взведенных битов
  static uint32 count( uint32 x )
  {
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x0000003F;
  }

  // [2] Вычисление значения бита четности
  static uint32 parity( uint32 x )
  {
    x = x ^ (x >> 1);
    x = (x ^ (x >> 2)) & 0x11111111;
    x = x * 0x11111111;
    return (x >> 28) & 1;
  }

  // [2] Переворачивание битовой маски
  static uint reverse( uint32 x )
  {
     x = ((x & 0x55555555) <<  1) | ((x >> 1) & 0x55555555);
     x = ((x & 0x33333333) <<  2) | ((x >> 2) & 0x33333333);
     x = ((x & 0x0F0F0F0F) <<  4) | ((x >> 4) & 0x0F0F0F0F);
     x = (x << 24) | ((x & 0xFF00) << 8) | ((x >> 8) & 0xFF00) | (x >> 24);
     return x;
  }
};

}; // namespace lwml

#endif // _BITS_
