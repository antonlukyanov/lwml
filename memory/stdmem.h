// Стандартные менеджеры памяти
// lwml, (c) ltwood

#ifndef _STDMEM_
#define _STDMEM_

#include "lwml/base/defs.h"
#include "lwml/base/basex.h"
#include "lwml/platforms.h"

/*#lake:stop*/

namespace lwml {

// Нехватка памяти

DEF_EX_TYPE(ex_base, ex_mem, "no more memory");

// ANSI-C спецификация поведения функций управления памятью:
//   malloc(0) = 0
//   free(0) = <do nothing>
//   realloc(0, size) = malloc(size)
//   realloc(mem, 0) = free(0), and returns 0

// Класс mem содержит статические функции, скрывающие
// одноименные системные функции выделения памяти.
// Единственное отличие от системных функций - в том, что здесь
// производится проверка успешности и при ошибке генерируется
// внутреннее для lwml исключение lex_mem вместо bad_alloc.
// Соответственно все варианты стандартного поведения, приводящие
// к возврату нулевого указателя, трактуются как ошибочные.
// Это -- функции выделения сырой памяти самого низкого уровня.
// Основное предназначение -- гарантировать успешность выделения памяти
// и дать возможность перехватить выделение и освобождение памяти при отладке.

class mem : public scope {
public:
  static void* alloc( int );
  static void  free( void* );
  static void* realloc( void*, int );

  static char* strdup( const char* );

  // Функция vform() конструирует строку по строке формата и списку аргументов.
  // Использование этой функции аналогично функции vprintf().
  // Функция автоматически размещает буфер необходимой длины и возвращает указатель на него.
  // Этот буфер необходимо потом освободить вызовом функции free().
  static char* vform( const char* fmt, va_list va );
  static char* form( const char* fmt, ... ) PRINTF(1,2);
};

// Функции выделения сырой памяти более высокого уровня.
// Кроме выделения памяти производится преобразование типов.
// Примеры:
//   int* ptr = raw_alloc<int>();
//     - завести в динамической памяти один объект типа int
//   char* cptr = raw_alloc<char>(10);
//     - завести в динамической памяти массив из 10 объектов типа char
//   cptr = raw_realloc(cptr, 15);
//     - изменить размер выделенной памяти
//   raw_free(cptr)
//     - освобождение выделенной памяти
// При вызове функции raw_realloc() нет необходимости указывать тип объектов,
// поскольку тип выводится из типа указателя ptr.

template <typename T>
inline T* raw_alloc( int len = 1 )
{
  return static_cast<T*>(mem::alloc(len * sizeof(T)));
}

template <typename T>
inline T* raw_realloc( T* ptr, int len )
{
  return static_cast<T*>(mem::realloc(ptr, len * sizeof(T)));
}

template <typename T>
inline void raw_free( T* ptr )
{
  mem::free(ptr);
}

}; // namespace lwml

// Здесь мы начинаем портить глобальную область видимости!

// Арена для аллокаторов, специфичных для lwml.

struct lwml_alloc_t {};

extern const lwml_alloc_t lwml_alloc;

// Операторы выделения памяти определяем в глобальной области видимости.
// Эти операторы не подменяют базовые глобальные аллокаторы,
// поскольку для их использования нужно явно указывать "арену" lwml_alloc.
// Эти операторы отличаются от стандартных тем, что по ошибке
// они генерируют внутреннее для lwml исключение вместо bad_alloc.
// Фактически реализованы через вызов функции lwml::mem::alloc().

inline void* operator new( size_t sz, const lwml_alloc_t& )
{
  return lwml::mem::alloc(sz);
}

inline void* operator new[]( size_t sz, const lwml_alloc_t& )
{
  return lwml::mem::alloc(sz);
}

inline void operator delete( void* ptr, const lwml_alloc_t& )
{
  lwml::mem::free(ptr);
}

inline void operator delete[]( void* ptr, const lwml_alloc_t& )
{
  lwml::mem::free(ptr);
}

#endif // _STDMEM_
