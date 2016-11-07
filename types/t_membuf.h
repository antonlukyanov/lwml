// Обобщенный массив POD-элементов
// lwml, (c) ltwood

#ifndef _T_MEMBUF_
#define _T_MEMBUF_

#include "lwml/base/defs.h"
#include "lwml/memory/stdmem.h"
#include "lwml/base/basex.h"
#include "lwml/lwmlconf.h"

/*#lake:stop*/

namespace lwml {

// Класс реализует простой обобщенный массив.
// Такой массив можно параметризовать только простыми POD-типами,
// не требующими нетривиального конструирования и копирования.
// Конструктор, конструктор копирования и деструктор для
// элементов не вызываются.

template<typename T>
class t_membuf : public value {
public:
  // Сконструировать массив заданной длины или пустой массив.
  explicit t_membuf( int len = 0 )
  {
    _data = raw_alloc<T>(_len = len);
  }

  // Сконструировать массив заданной длины с заданным состоянием элементов.
  t_membuf( int len, const T& val )
  {
    _data = raw_alloc<T>(_len = len);
    for( int j = 0; j < _len; j++ )
      _data[j] = val;
  }

  t_membuf( const t_membuf<T>& src )
  {
    _data = raw_alloc<T>(_len = src._len);
    memcpy(_data, src._data, _len * sizeof(T));
  }

  // Разрушение массива.
  ~t_membuf()
  {
    raw_free<T>(_data);
  }

  // Здесь длины массивов не обязаны совпадать.
  t_membuf<T>& operator=( const t_membuf<T>& src )
  {
    if( &src == this )
      return *this;
    _data = raw_realloc<T>(_data, _len = src._len);
    memcpy(_data, src._data, _len * sizeof(T));
    return *this;
  }

  // Изменение размера массива.
  void resize( int new_len )
  {
    _data = raw_realloc<T>(_data, _len = new_len);
  }

  // Установка значений всех элементов массива.
  void set_val( const T& val )
  {
    for( int j = 0; j < _len; j++ )
      _data[j] = val;
  }

  // Получение размера массива.
  int len() const { return _len; }

  // Операторы индексирования.
  T& operator[]( int j ) {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[j];
  }
  const T& operator[]( int j ) const {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[j];
  }

  // Низкоуровневый доступ к "сырым" данным.
        T* access_raw_data()       { return _data; }
  const T* access_raw_data() const { return _data; }

  int comp( int i, int j ) const {
    return (_data[i] < _data[j]) ? -1 : (_data[i] > _data[j]) ? 1 : 0;
  }

private:
  int   _len;
  T*    _data;
};

}; // namespace lwml

#endif // _T_MEMBUF_
