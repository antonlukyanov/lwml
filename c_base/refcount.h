// Reference counting
// lwml, (c) ltwood

#ifndef _REFCOUNT_
#define _REFCOUNT_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

// Класс-ссылка на объекта класса с подсчетом ссылок.

template<typename T>
class referer : public value {
public:
  referer( T* ptr = 0 )             : _ptr(ptr)       { inc_ref(); }
  referer( const referer<T>& obj )  : _ptr(obj._ptr)  { inc_ref(); }

  ~referer() { dec_ref(); }

  referer<T>& operator=( const referer<T>& ref ){
    if( &ref != this ){
      dec_ref();
      _ptr = ref._ptr;
      inc_ref();
    }
    return *this;
  }

  // Специфические конструктор и оператор копирования для обеспечения
  // возможности присвоения ссылке на базовый класс ссылки на класс-наследник.
  // Эти функции не заменяют собой обычный конструктор и оператор копирования
  // из-за специфических правил поиска конструктора и оператора копирования в C++.
  // На самом деле в строгом смысле они даже не являются конструктором и оператором
  // копирования, а представляют специфические операции преобразования типа.

  template<typename T2>
    referer( const referer<T2>& obj ) : _ptr(obj.capture_raw_pointer())  {}

  template<typename T2>
  referer<T>& operator=( const referer<T2>& ref ){
    dec_ref();
    _ptr = ref.capture_raw_pointer();
    return *this;
  }

  // Проверка инициализированности указателя.
  bool is_ok() const    { return _ptr != 0; }

  //!! SIC: эти операторы возвращают неконстантный объект, будучи сами константными!
  T& operator*()  const { return *_ptr; }
  T* operator->() const { return _ptr; }

  // Низкоуровневый захват и освобождение указателя.
  T* capture_raw_pointer() const    { inc_ref(); return _ptr; }
  void  release_raw_pointer()       { dec_ref(); }

private:
  T* _ptr;

  void inc_ref() const { if( _ptr ) _ptr->inc_ref(); }
  void dec_ref() const { if( _ptr ) _ptr->dec_ref(); }
};

}; // namespace lwml

#endif // _REFCOUNT_
