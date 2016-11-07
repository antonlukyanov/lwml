// Менеджеры памяти
// lwml, (c) ltwood

#ifndef _MEMMAN_
#define _MEMMAN_

#include "lwml/base/defs.h"
#include "lwml/memory/stdmem.h"
#include "lwml/base/refcount.h"

/*#lake:stop*/

namespace lwml {

// Абстрактный интерфейс обобщенного менеджера памяти
// Этот интерфейс наследуется конкретными менеджерами
// Функция strdup() не является абстрактной и содержит реализацию.

class i_memman : public interface, public refcount {
public:
  void* alloc( uint el_num, uint el_sz ){
    return alloc(el_num * el_sz);
  }

  virtual void* alloc( uint size ) = 0;
  virtual void  free( void* ) = 0;

  char* strdup( const char* );
};

// Макросы управления памятью через абстрактный менеджер
// Имеют смысл, аналогичный описанным выше макросам

#define MM_NEW( mm, type )                                      \
  static_cast<type*>((mm)->alloc(sizeof(type)))                 //
#define MM_ARRAY( mm, type, size )                              \
  static_cast<type*>((mm)->alloc((size) * sizeof(type)))        //
#define MM_FREE( mm, ptr )                                      \
  (mm)->free( ptr )                                             //

// Конкретный менеджер памяти, реализующий абстрактный
// интерфейс i_memman посредством функций из класса mem

class std_memman : public i_memman { // public refcount
private:
  std_memman() {}

public:
  static referer<std_memman> create() {
    return referer<std_memman>(new(lwml_alloc) std_memman());
  }

  virtual void* alloc( uint size ) { return mem::alloc(size); }
  virtual void  free( void *ptr )  { mem::free(ptr); }
};

}; // namespace lwml

#endif // _MEMMAN_
