// Общие математические определения и функции
// lwml, (c) ltwood

#ifndef _MDEFS_
#define _MDEFS_

#include "lwml/base/defs.h"
#include "lwml/base/basex.h"

/*#lake:stop*/

namespace lwml {

// Математические исключения

DEF_EX_CLASS(ex_base, ex_math);

DEF_EX_TYPE(ex_math, ex_sing, "numerical singularity");
DEF_EX_TYPE(ex_math, ex_noconv, "no convergence");
DEF_EX_TYPE(ex_math, ex_loss, "loss of significance");

// Базовый вещественный тип.

typedef double real;
typedef long double long_real;

// Получение т.н. тихого (quiet) нечисла (NaN)

real real_nan();
bool is_nan( real );

void nan_filling( real*, int sz );

// Математические константы, дублирующие
// определения из файла float.h

// STDEPS    - стандартное значение "малого" числа,
//             используемого как точность, погрешность и пр.
// REAL_DIGS - число десятичных цифр после запятой
// REAL_BITS - число битов в мантиссе
// REAL_EPS  - минимальное положительное число, отличное от нуля
// REAL_MIN  - минимальное значение вещественной переменной
// REAL_MAX  - максимальное значение вещественной переменной

const real STDEPS    = 1e-12;
const int  REAL_DIGS = DBL_DIG;         // 15
const int  REAL_BITS = DBL_MANT_DIG;    // 53
const real REAL_EPS  = DBL_EPSILON;     // 2.22E-16
const real REAL_MIN  = DBL_MIN;         // 2.23E-308
const real REAL_MAX  = DBL_MAX;         // _max_dble

// Основные математические константы

#undef M_E
#undef M_LOG2E
#undef M_LOG10E
#undef M_LN2
#undef M_LN10
#undef M_PI
#undef M_PI_2
#undef M_PI_4
#undef M_1_PI
#undef M_2_PI
#undef M_2_SQRTPI
#undef M_SQRT2
#undef M_1_SQRTPI
#undef M_SQRT1_2

#define M_E         2.71828182845904523536   // $e$
#define M_LOG2E     1.44269504088896340736   // $\log_2(e)$
#define M_LOG10E    0.434294481903251827651  // $\log_10(e)$
#define M_LN2       0.693147180559945309417  // $\log_e(2)$
#define M_LN10      2.30258509299404568402   // $\log_e(10)$
#define M_PI        3.14159265358979323846   // $\pi$
#define M_PI_2      1.57079632679489661923   // $\pi/2$
#define M_PI_4      0.785398163397448309616  // $\pi/4$
#define M_1_PI      0.318309886183790671538  // $1/\pi$
#define M_2_PI      0.636619772367581343076  // $2/\pi$
#define M_2_SQRTPI  1.12837916709551257390   // $2/\pi^{1/2}$
#define M_SQRT2     1.41421356237309504880   // $2^{1/2}$
#define M_1_SQRTPI  0.564189583547756286948  // $1/\pi^{1/2}$
#define M_SQRT1_2   0.707106781186547524401  // $1/2^{1/2}$

// Дополнительные математические константы

const real M_SQRTPI   = 1.77245385090551603; // $\pi^{1/2}$
const real M_SQRT2PI  = 2.50662827463100050; // $(2\pi)^{1/2}$
const real M_SQRT3    = 1.73205080756887729; // $3^{1/2}$
const real M_SQRT5    = 2.23606797749978970; // $5^{1/2}$
const real M_GOLDSECT = 0.61803398874989485; // $(5^{1/2}-1)/2$

// Функции безопасного приведения типов.
// Эти функции используются для более четкого выделения
// случая безопасного static_cast от int к real
// и относительно безопасного случая static_cast от real к int.

// Приведение от типа real к типу int
inline int int_cast( real x ) { return static_cast<int>(x); }

// Приведение от типа real к типу uint
inline uint uint_cast( real x ){ return static_cast<uint>(x); }

// Приведение от типа int к типу real
inline real real_cast( int x ) { return static_cast<real>(x); }

// Приведение от типа uint к типу real
inline real real_cast( uint x ){ return static_cast<real>(x); }

// знак числа

inline real fsgn( real x ){
  return x < 0.0 ? -1.0 : (x > 0.0 ? 1.0 : 0.0);
}

// (-1)^n в вещественной и целочисленной версии

inline int  loddsgn( int n ) { return (n&1) ? -1 : 1; }
inline real foddsgn( int n ) { return (n&1) ? -1.0 : 1.0; }

// извлечение корня из целого числа

inline real fsqrt( int x ){ return sqrt(real_cast(x)); }
inline real fsqrt( real x ){ return sqrt(x); }

// возведение числа в целую степень n

real intpow( real x, int n );

// вычисление целочисленного двоичного логарифма

int intlog2( int );

// округление вверх до ближайшей целой степени двойки

int up2pow2( int );

// проверка является ли число целой степенью двойки

inline bool ispow2( int x ){ return !((~(~0u>>1)|x) & (x - 1)); }

// возведение в квадрат

inline real fsqr( real t ){ return t * t; }
inline real fsqr( int t ){ return t * t; }

// нахождение суммы квадратов аргументов

inline real mag( real x, real y ){
  return fsqr(hypot(x, y));
}

// корень из суммы квадратов для трех аргументов

inline real hypot3( real x, real y, real z ){
  return hypot(x, hypot(y, z));
}

// перевод в логарифмические единицы амплитуды (децибеллы)

inline real log_dba( real x, real min = -120.0 ){
  real xmin = pow(10.0, min/20.0);
  return (x > xmin) ? 20.0 * log10(x) : min;
}

// перевод в логарифмические единицы мощности (децибеллы)

inline real log_dbp( real x, real min = -60.0 ){
  real xmin = pow(10.0, min/10.0);
  return (x > xmin) ? 10.0 * log10(x) : min;
}

#if defined(__BORLANDC__) || defined(_MSC_VER)
  inline real log2( real x ){ return log(x) / M_LN2; }
#endif

// функция x*log(x)  в случае двоичного логарифма
// с доопределением по непрерывности в точке 0.0

inline real log2c( real x ){
  if( x == 0.0 )
    return -0.0;
  else
    return x * log2(x);
}

// функция sin(x)/x с доопределением по непрерывности в точке 0.0

inline real sinc( real x ){
  if( x == 0.0 )
    return 1.0;
  else
    return sin(x) / x;
}

// стандартная сигмоида
// с областью значений [0,1]

inline real sigm( real x ){
  return 1.0 / (1.0 + exp(-x));
}

// Функция Гаусса единичной площади,
// являющаяся плотностью нормального распределения.

inline real gauss( real x, real sigma = 1.0 ){
  return exp(-fsqr(x/sigma)/2.0) / (sigma * M_SQRT2PI);
}

// Весовая функция Гаусса.
// В точке x = 0 принимает единичное значение,
// а в точке x0 значение уменьшается в d раз.
// При d = 2 величина x0 совпадает с половиной параметра FWHM.

inline real gauss_weight( real x, real x0, real d ){
  return exp(-fsqr(x/x0) * log(d));
}

// перевод угла из градусов в радианы

inline real deg2rad( real gr ){
  return gr * (M_PI / 180.0);
}

// перевод угла из радианов в градусы

inline real rad2deg( real rd ){
  return rd * (180.0 / M_PI);
}

// Округления для вещественных чисел.
// Реализовано отбрасывание дробной части и округление.
// Эти функции присутствуют в C99, но не в C++.
// К тому же в C99 round() не соответствует привычному
// определению математического округления.

namespace fpr {
  inline real trunc( real x ) { return (x < 0.0) ? ceil(x) : floor(x); }
  inline real round( real x ) { return (x < 0.0) ? ceil(x - 0.5) : floor(x + 0.5); }

  // округление числа до d знаков после запятой
  inline real fround10( real x, int d ){
    real a = pow(10.0, d);
    return round(x * a) / a;
  }

  inline int ltrunc( real x ) { return int_cast(trunc(x)); }
  inline int lround( real x ) { return int_cast(round(x)); }
};

// масштабирование чисел

class scale : public scope {
public:
  // мастштабирование вещественного числа x из диапазона [a,b] в диапазон [c,d]
  static real coord( real x, real a, real b, real c, real d ){
    return c + (d-c) * (x-a)/(b-a);
  }

  // определение того как меняется длина отрезка
  // при масштабировании из диапазона [a,b] в диапазон [c,d]
  static real length( real l, real a, real b, real c, real d ){
    return (d-c) * l / (b-a);
  }

  // преобразование вещественного числа x из диапазона [a,b]
  // в целое число из диапазона [c,d]
  static int interval( real x, real a, real b, int c, int d ){
    int res = int_cast(scale::coord(x, a, b, c, d));
    return (res < c) ? c : ((res > d) ? d : res);
  }
};

// работа с разбиением отрезка на систему интервалов
// класс содержит только статические методы
// методы класса проверяют диапазон аргументов

class intervals : public scope {
public:
  // преобразовать число x из диапазона [ax,bx]
  // в номер интервала из диапазона [0, len-1]
  static int val2idx( real x, real ax, real bx, int len );

  // преобразовать номер интервала из диапазона [0, len-1]
  // в число, соответствующее левой границе интервала
  static real idx2val( int idx, real ax, real bx, int len );

  // преобразовать номер интервала из диапазона [0, len-1]
  // в число, соответствующее середине интервала
  static real idx2cnt( int idx, real ax, real bx, int len );
};

// Класс, поддерживающий IEEE-представление вещественных чисел.
// | type   bytes bits mant exp exp.limits  shift interval  digs  eps
// | float  4     32   24   8   -126  +127  127   10e+-38   6     1.19E-07f
// | double 8     64   53   11  -1022 +1023 1023  10e+-308  15    2.22E-16
// Статическая функция isexactint() проверяет вещественное число
// на возможность его точного представления как целого.

class ieeereal : public value {
public:
  // инициализировать IEEE-представление по числу
  ieeereal( real x = 0.0 ){ set(x); }

  // инициализировать IEEE-представление по компонентам представления
  ieeereal( real mnt, int exp ){ set(mnt, exp); }

  // установить значение
  void set( real x );

  // установить компоненты представления
  void set( real mnt, int exp );

  // получить вещественное число
  real get();

  // получить мантиссу
  real mant() const { return _mant; }

  // получить экспоненту
  int expn() const { return _expn; }

  // проверить число на возможность его точного представления как целого
  static bool is_exactint( real n ){
    real d = n + 1.0;
    return n + 3.0 == d + 2.0;
  }

private:
  real _mant;
  int  _expn;
};

// Сумматор с защитой от накопления погрешности.
// Алгоритм Кахана.

class summer : public value {
public:
  summer( real v = 0.0 ) : _sum(v), _cur(0.0) {}

  void reset( real v = 0.0 ) {
    _sum = v;
    _cur = 0.0;
  }

  real get() const { return _sum; }

  real add( real v ) {
    double y = v - _cur;
    double t = _sum + y;
    _cur = (t - _sum) - y;
    return _sum = t;
  }

private:
  real _sum;
  real _cur;
};

// Нахождение минимума и максимума без предварительной инициализации

class minmax : public value {
public:
  minmax() : _is_first(true), _min(0.0), _max(0.0) {}

  void put( real x ){
    if( _is_first ){
      _min = _max = x;
      _is_first = false;
    } else {
      t_minize(_min, x);
      t_maxize(_max, x);
    }
  }

  void reset() { _is_first = true; }

  real min() const { return _min; }
  real max() const { return _max; }

  // было ли в объект помещено хоть одно значение
  bool is_ok() const { return !_is_first; }

private:
  bool _is_first;
  real _min, _max;
};

// Построение линейной функции по двум точкам

class line_func : public value {
public:
  line_func( real x1, real y1, real x2, real y2 )
  {
    reset(x1, y1, x2, y2);
  }

  void reset( real x1, real y1, real x2, real y2 ){
    if( x2 == x1 )
      throw ex_sing();

    _x1 = x1;
    _y1 = y1;
    _k = (y2 - y1) / (x2 - x1);
  }

  real operator()( real x ) const { return _y1 + _k * (x - _x1); }

  real inv( real y ){
    if( _k == 0.0 )
      throw ex_sing();

    return _x1 + (y - _y1) / _k;
  }

private:
  real _x1, _y1, _k;
};

// Построение квадратичной функции по трем точкам
// Предполагается, что точка соответствуют значения
// аргумента x=0.0, x=1.0 и x=2.0.

class quad_func : public value {
public:
  quad_func( real y0, real y1, real y2 )
  {
    reset(y0, y1, y2);
  }

  void reset( real y0, real y1, real y2 ){
    _y0 = y0;
    _d1 = y1 - y0;
    _d2 = 0.5 * (y0 - 2*y1 + y2);
  }

  real operator()( real x ) const { return _y0 + x * (_d2*x + (_d1-_d2)); }

private:
  real _y0, _d1, _d2;
};

// Функция-клей.
// Предназначена для склеивания кусочно-заданных функций.
// По сути определена на отрезке [0, 1].
// На концах отрезка принимает значения 0.0 и 1.0 соответственно.
// Производная функции принимает на концах значения d1 и d2.

class glue_func : public value {
public:
  glue_func( real d0, real d1 ){
    _a = d0 + d1 - 2.0;
    _b = 3.0 - 2.0 * d0 - d1;
    _c = d0;
  }

  real operator()( real x ) const { return x * (x * (_a * x + _b) + _c); }

private:
  real _a, _b, _c;
};

}; // namespace lwml

#endif // _MDEFS_
