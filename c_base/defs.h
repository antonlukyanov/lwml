// Общие определения макросов, типов и функций
// lwml, (c) ltwood

#ifndef _DEFS_
#define _DEFS_

#include "platforms.h"

// Стандартные заголовки ANSI-C.
// Функции определяются в пространстве имен std.

#include <cassert>
#include <cctype>
#include <cfloat>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cerrno>
#include <cstdint>

// Стандартные заголовки ANSI-C++.

#include <exception>
#include <new>
#include <typeinfo>

/*#lake:stop*/

// Пространство имен std импортируется в пространство имен lwml.

namespace lwml {
  using namespace std;
};

namespace lwml{

// Определение общих типов.

typedef unsigned char  uchar;      // беззнаковый однобайтовый символ
typedef   signed char  schar;      // знаковый однобайтовый символ
typedef unsigned int   uint;       // беззнаковое целое

// Определение типов с фиксированным размером.

typedef int16_t  int16;      // знаковое 16-битное целое
typedef uint16_t uint16;     // беззнаковое 16-битное целое
typedef int32_t  int32;      // знаковое 32-битное целое
typedef uint32_t uint32;     // беззнаковое 32-битное целое

// Шаблонные функции поиска минимума и максимума

template <typename T>
  inline T t_min( const T& t1, const T& t2 ) { return (t1 < t2) ? t1 : t2; }
template <typename T>
  inline T t_max( const T& t1, const T& t2 ) { return (t1 > t2) ? t1 : t2; }

template <typename T>
  inline T t_minize( T& dst, const T& src ) { return dst = t_min<T>(dst, src); }
template <typename T>
  inline T t_maxize( T& dst, const T& src ) { return dst = t_max<T>(dst, src); }

template <typename T>
  inline T t_min( const T& t1, const T& t2, const T& t3 ) { return t_min<T>(t_min<T>(t1, t2), t3); }
template <typename T>
  inline T t_max( const T& t1, const T& t2, const T& t3 ) { return t_max<T>(t_max<T>(t1, t2), t3); }

// Шаблонная функция обмена значений аргументов местами.

template <typename T> inline void t_swap( T& t1, T& t2 )
{
  T tmp = t1;
  t1 = t2;
  t2 = tmp;
}

// Пара значений

template <typename T1, typename T2 = T1>
struct t_pair {
  T1 a;
  T2 b;

  // обычная инициализация пары
  t_pair( const T1& aa, const T2& bb ) : a(aa), b(bb) {}

  // этот ctor приходится определять из-за присутствия предыдущего
  t_pair() : a(T1()), b(T2()) {}

  // обеспечиваем преобразования типов для элементов пары
  template <typename TT1, typename TT2>
    t_pair( const t_pair<TT1, TT2>& p ) : a(p.a), b(p.b) {}
};

// Предок классов, не поддерживающих создание экземпляров.
// Такие классы содержат только статические функции и служат
// фактически нерасширяемыми областями видимости.
// Содержит закрытый конструктор, что гарантирует невозможность
// создания экземпляра для класса-наследника этого класса.

class scope {
private:
  // no constructor
  scope();
};

// Предок классов-значений, полностью поддерживающих копирование
// т.е. содержащих корректный конструктор и оператор копирования.

class value {
public:
  // silly constructor ;)
  value() {}
};

// Предок для абстрактных интерфейсов
// Содержит пустой виртуальный деструктор, что гарантирует наличие
// виртуального деструктора у всех наследников этого класса.

class interface {
public:
  // virtual destructor
  virtual ~interface() {}
};

// Предок классов, поддерживающих подсчет ссылок.
// Схема корректного управления временем жизни объекта на основе подсчета ссылок
// реализована классом referer (refcount.h).
// Такие классы не должны предоставлять возможность непосредственного создания
// экземпляра т.е. должны иметь приватный конструктор.
//
// Класс refcount наследуется от refcount_imp виртуально
// с целью обеспечить возможность наследоваться от двух
// интерфесов, каждый из которых требует подсчета ссылок,
// т.е. также наследуется от refcount.

class refcount_imp {
public:
  refcount_imp() : _cnt(0) {}

  virtual ~refcount_imp() {}

  void inc_ref() const {
    ++_cnt;
  }

  void dec_ref() const {
    if( _cnt > 0 ){
      --_cnt;
      if( _cnt == 0 ) delete this;
    }
  }

private:
  mutable int _cnt;

  // no copy operations
  refcount_imp( const refcount_imp& );
  refcount_imp& operator=( const refcount_imp& );
};

class refcount : virtual public refcount_imp {
};

// Предок классов со специфическим контрактом, не поддерживающих
// копирование (конструктором копирования или оператором копирования).
// Такие классы обычно требуют особого внимания при использовании.
// Содержит приватные конструктор копирования и оператор копирования,
// что гарантирует невозможность копирования для наследников этого класса.

class hardcore {
public:
  // silly constructor ;)
  hardcore() {}

private:
  // no copy operations
  hardcore( const hardcore& );
  hardcore& operator=( const hardcore& );
};

// Функция безопасного копирования строки в буфер.
// В конец строки гарантированно прописывается символ-ограничитель.
// Если передан нулевой указатель src, то в буфере будет
// сформировна корректная строка нулевой длины.
char* prot_strcpy( char* dst, const char* src, int buflen );

// Функция безопасного форматного вывода в буфер.
// В конец строки гарантированно прописывается символ-ограничитель.
// Возвращает число выведенных символов или -1 при переполнении буфера.
// Гарантируется, что эти функции никогда не генерируют исключений.
int prot_vsprintf( char* buf, int buflen, const char* fmt, va_list va );
int prot_sprintf( char* buf, int buflen, const char* fmt, ... ) PRINTF(3,4);

// версия библиотеки
const char* lwml_ver();

// Общие константы

enum tick_mode {  // состояние прогресс-индикации
  tmON, tmOFF
};

enum real_format {  // формат представления чисел
  nfSCNT, nfFIXED
};

}; // namespace lwml

#endif // _DEFS_
