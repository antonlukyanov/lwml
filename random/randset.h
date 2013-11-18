// Коллекция датчиков случайных чисел
// Только для тестирования!
// В реальных задачах следует использовать датчик frand (см. frand.h)
// lwml, (c) ltwood

#ifndef _RANDSET_
#define _RANDSET_

#include "defs.h"
#include "mdefs.h"
#include "frand.h"

/*#lake:stop*/

namespace lwml {

// Коллекция содержит следующие датчики:
// i_rand   - общий интерфейс всех датчиков из этой коллекции
// lcg32    - обобщенный 32-битный линейный конгруентный ДСЧ
// randu    - знаменитый датчик RANDU из SSP
// vrand    - датчик В.В.Васильчикова
// brand    - датчик rand() из C/C++ компиляторов фирмы Borland
// wrand    - датчик rand() из C/C++ компиляторов фирмы Watcom
// krand    - датчик А.К.Карлина
// lcg10    - обобщенный модельный 10-битный линейный конгруентный ДСЧ
// drand1   - простой модельный 10-битный ДСЧ
// drand2   - еще один модельный 10-битный ДСЧ
// tfrand   - обертка над рабочим датчиком frand

// Преобразование беззнакового целого 32-битного
// числа в вещественное число из диапазона [0,1]

inline real UINT2FLOAT( uint x )
{
  return static_cast<double>((x >> 1) & 0x7FFFFFFF) / static_cast<double>(0x80000000);
}

// Преобразование целого числа в ближайшее нечетное целое число

inline int ODD( int x )
{
  return (x & 0x01) ? x : x + 1;
}

// Интерфейс ДСЧ из коллекции

class i_rand : public interface {
public:
  // получить очередное случайное число
  virtual real get() = 0;
};

// Обобщенный 32-битный линейный конгруентный ДСЧ
// При конструировании указывается стартовое число
// и значения констант линейного конгруентного алгоритма

class lcg32 : public i_rand, public value {
public:
  lcg32( uint seed, uint a, uint c ){
    _x = seed;
    _a = a;
    _c = c;
  }

  virtual real get(){
    return UINT2FLOAT(next());
  }

private:
  uint _x; // MUST be 32-bit unsigned int
  uint _a, _c;

  uint next(){
    return _x = _a * _x + _c;
  }
};

// ДСЧ RANDU из пакета SSP фирмы IBM.
// X(k+2) = 6 * X(k+1) - 9 * X(k) mod m

class randu : public lcg32 {
public:
  randu( uint seed = 1 ) : lcg32(ODD(seed), 0x10003, 0) {}
};

// ДСЧ В.В.Васильчикова

class vrand : public lcg32 {
public:
  vrand( uint seed = 1 ) : lcg32(ODD(seed), 0xCAB3, 0) {}
};

// Функция rand() из Borland C/C++
// Оригинальная функция возвращает 16-битное число,
// вычисляемое по формуле (X >> 16) & 0x7fff

class brand : public lcg32 {
public:
  brand( uint seed = 1 ) : lcg32(seed, 0x15A4E35, 1) {}
};

// Функция rand() из Watcom C/C++
// Оригинальная функция возвращает 16-битное число,
// вычисляемое по формуле (X >> 16) & 0x7fff

class wrand : public lcg32 {
public:
  wrand( uint seed = 1 ) : lcg32(seed, 0x41C64E6D, 0x3039) {}
};

// ДСЧ А.К.Карлина

class krand : public i_rand, public value {
public:
  krand(){
    _a = sqrt(2.0);
    _b = sqrt(3.0);
  }

  virtual real get();

private:
  double _a, _b;

  double frac( double x ) const { return x - floor(x); }
};

// Обобщенный модельный 10-битный линейный конгруентный ДСЧ

class lcg10 : public i_rand, public value {
public:
  lcg10( uint seed, uint a, uint c ){
    _x = seed;
    _a = a;
    _c = c;
  }

  virtual real get() { return real_cast(step()) / 1024.0; }

private:
  uint _x;
  uint _a, _c;

  uint step(){
    return _x = (_a * _x + _c) % 1024;
  }
};

// Простой модельный 10-битный ДСЧ

class drand1 : public lcg10 {
public:
  drand1( uint seed = 1 ) : lcg10(seed, 1021, 21) {}
};

// Еще один модельный 10-битный ДСЧ

class drand2 : public lcg10 {
public:
  drand2( uint seed = 1 ) : lcg10(seed, 997 /*77*/, 0 /*21*/ /*1*/) {}
};

// Обертка над рабочим датчиком frand

class tfrand : public i_rand, public value {
public:
  tfrand( uint seed = 1 ) : _rnd(seed) {}

  virtual real get() { return _rnd.get_real(); }

private:
  frand _rnd;
};

}; // namespace lwml

#endif // _RANDSET_
