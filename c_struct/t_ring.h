// Обобщенная циклическая очередь
// lwml, (c) ltwood

#ifndef _T_RING_
#define _T_RING_

#include "defs.h"
#include "stdmem.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

// Класс реализует простую обобщенную кольцевую очередь.
// Элементы очереди можно индексировать напрямую,
// причем элемент с нулевым индексом - последний помещенный в очередь.
template<typename T>
class t_ring : public value {
public:
  // Создание очереди заданной максимальной длины.
  explicit t_ring( int size )
  {
    _data = raw_alloc<T>(_size = size);
    init_ptr();
  }

  // Оператор копирования.
  t_ring( const t_ring<T>& src )
  {
    _data = raw_alloc<T>(_size = src._size);
    _start = src._start;
    _finish = src._finish;
    _len = src._len;
    copy_data(src._data);
  }

  // Разрушение очереди.
  ~t_ring()
  {
    destroy();
    raw_free<T>(_data);
  }

  // Копирование очередей.
  t_ring<T>& operator=( const t_ring<T>& src )
  {
    if( &src == this )
      return *this;

    destroy();
    raw_free<T>(_data);
    _data = raw_alloc<T>(_size = src._size);
    _start = src._start;
    _finish = src._finish;
    _len = src._len;
    copy_data(src._data);
    return *this;
  }

  int  size() const     { return _size; }       // Получить макимальную длину очереди.
  int  len() const      { return _len; }        // Получить число элементов в очереди.
  bool is_empty() const { return _len == 0; }   // Проверить очередь на пустоту.

  // Очистить очередь.
  void clear() {
    destroy();
    init_ptr();
  }

  // Добавить элемент в конец очереди.
  t_ring<T>& push( const T& el ) {
    if( _len == _size )
      runtime("ring overfull (sz=%d)", _size);
    _finish = nextidx(_finish);
    new(_data + _finish) T(el);
    _len++;
    return *this;
  }

  // Протолкнуть в конец очереди.
  t_ring<T>& roll( const T& el ) {
    if( _len == _size )
      pop();
    return push(el);
  }

  // Получить копию начала очереди и удалить его
  // (удаляемый элемент возвращается по значению).
  T pop() {
    if( is_empty() )
      fail_assert("empty ring");
    T res = _data[_start];
    (_data + _start)->~T();
    _start = nextidx(_start);
    _len--;
    return res;
  }

  // Функции доступа к началу очереди.
  T& top() {
    if( is_empty() )
      fail_assert("empty ring");
    return _data[_start];
  }
  const T& top() const {
    if( is_empty() )
      fail_assert("empty ring");
    return _data[_start];
  }

  // Операторы индексирования.
  T& operator[]( int j ) {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[(_finish - j + _size) % _size];
  }
  const T& operator[]( int j ) const {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[(_finish - j + _size) % _size];
  }

  // Индексирование без проверки индекса.
  // Для применения в надежных алгоритмах.
        T& at( int j )       { return _data[(_finish - j + _size) % _size]; }
  const T& at( int j ) const { return _data[(_finish - j + _size) % _size]; }

private:
  int   _size;         // максимальный размер очереди
  T*    _data;         // данные очереди
  int   _start;        // самый старый внесенный элемент (начало очереди)
  int   _finish;       // последний внесенный элемент (конец очереди)
  int   _len;          // фактическая длина очереди

  int nextidx(int idx) const { return (idx+1) % _size; }

  void init_ptr(){
    _finish = _size - 1;
    _start = nextidx(_finish);
    _len = 0;
  }

  void destroy(){
    for( int j = 0; j < _len; j++ ){
      int idx = (_finish - j + _size) % _size;
      (_data + idx)->~T();
    }
  }

  void copy_data( T* data ){
    for( int j = 0; j < _len; j++ ){
      int idx = (_finish - j + _size) % _size;
      new(_data + idx) T(data[idx]);
    }
  }
};

}; // namespace lwml

#endif // _T_RING_
