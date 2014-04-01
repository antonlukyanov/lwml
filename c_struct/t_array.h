// Обобщенный массив
// lwml, (c) ltwood

#ifndef _T_ARRAY_
#define _T_ARRAY_

#include "defs.h"
#include "stdmem.h"
#include "basex.h"
#include "lwmlconf.h"

/*#lake:stop*/

namespace lwml {

// Класс реализует простой обобщенный массив объектов.
template<typename T>
class t_array : public value {
public:
  // Сконструировать массив заданной длины или пустой массив.
  explicit t_array( int len = 0 )
  {
    _data = raw_alloc<T>(_len = len);
    for( int j = 0; j < len; j++ )
      new(_data + j) T();
  }

  // Сконструировать массив заданной длины с заданным состоянием элементов.
  t_array( int len, const T& val )
  {
    _data = raw_alloc<T>(_len = len);
    for( int j = 0; j < _len; j++ )
      new(_data + j) T(val);
  }

  // Сконструировать копию заданного массива.
  t_array( const t_array<T>& src )
  {
    _data = raw_alloc<T>(_len = src._len);
    for( int j = 0; j < _len; j++ )
      new(_data + j) T(src[j]);
  }

  // Разрушение массива.
  ~t_array()
  {
    destroy(_data, _len);
    raw_free<T>(_data);
  }

  // Копирование массивов (длины не обязаны совпадать).
  t_array<T>& operator=( const t_array<T>& src )
  {
    if( &src == this )
      return *this;

    destroy(_data, _len);
    raw_free<T>(_data);
    _data = raw_alloc<T>(_len = src._len);
    for( int j = 0; j < _len; j++ )
      new(_data + j) T(src[j]);
    return *this;
  }

  // Изменение размера массива.
  // В случае увеличения размера массива во втором аргументе
  // можно задать начальное состояние добавляемых элементов.
  void resize( int new_len, const T& t0 = T() )
  {
    T* old_data = _data;
    int old_len = _len;

    _data = raw_alloc<T>(_len = new_len);
    int copy_len = t_min<int>(old_len, new_len);
    for( int j = 0; j < copy_len; j++ )
      new(_data + j) T(old_data[j]);
    for( int j = copy_len; j < new_len; j++ )
      new(_data + j) T(t0);

    destroy(old_data, old_len);
    raw_free<T>(old_data);
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

  // Операторы индексирования без проверок индексов.
  T& at( int j ) { return _data[j]; }
  const T& at( int j ) const { return _data[j]; }

  // Низкоуровневый доступ к "сырым" данным.
        T* access_raw_data()       { return _data; }
  const T* access_raw_data() const { return _data; }

  int comp( int i, int j ) const {
    return (_data[i] < _data[j]) ? -1 : (_data[i] > _data[j]) ? 1 : 0;
  }

private:
  int   _len;
  T*    _data;

  void destroy( T* x, int n ){
    for( int j = 0; j < n; j++ )
      (x + j)->~T();
  }
};

}; // namespace lwml

#endif // _T_ARRAY_
