// Reference counting
// lwml, (c) ltwood

#ifndef _REFCOUNT_
#define _REFCOUNT_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"

/*#lake:stop*/

namespace lwml {

/*
  Всё плохо.

  1. Текущий вариант не будет работать с объектами произвольных классов,
     необходимо наследоваться от refcount - сильно мешает и ограничивает.
  2. Слово referer написано с ошибкой. Правильно referrer.
  3. if( _cnt == 0 ) delete this; --- концептуальная ошибка, если объект был
     сконструирован на какой-либо арене (например, lwml_alloc). C++ не умеет
     отслеживать каким образом был создан объект, поэтому объекты, которые были
     созданы на какой-то арене должны удалятся соответствующим этой арене оператором,
     а перед удалением должен вызываться деструктор (Bjarne Stroustrup - The C++
     Programming Language, 4th Edition, 11.2.4).
  4. operator delete( void* ptr, const lwml_alloc_t& ) вызывается только в случае
     невозможности выделения памяти (например, при выбросе исключения из конструктора).
  5. Если объект, хранящийся в referer был создан в каком-то пуле памяти, то вызов
     delete снесёт весь пул со всеми остальным объектами, которые там могут быть.
  6. Явный вызов operator delete(this, lwml_alloc) не будет компилироваться.
  7. Если в referer хранится масссив Tp[], то должен вызываться delete[] this, а не просто
     delete this.

  В качестве одного из решений (как в STL), при конструировании referer вторым шаблонным
  параметром можно передавать deleter<X>, который и будет осуществлять удаление
  объекта правильным образом. Дефолтный deleter можно прописать шаблонным параметром
  по умолчанию.
*/

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
