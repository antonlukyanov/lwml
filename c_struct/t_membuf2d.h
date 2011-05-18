// Обобщенная матрица для POD
// lwml, (c) ltwood

#ifndef _T_MEMBUF2D_
#define _T_MEMBUF2D_

#include "defs.h"
#include "t_membuf.h"
#include "stream.h"

/*#lake:stop*/

namespace lwml {

template<typename T>
class t_membuf2d: public value {
public:
  // Конструирование пустой матрицы.
  t_membuf2d();

  // Конструирование по размерам.
  t_membuf2d( int m, int n );

  // Конструирование по размерам и начальному значению элементов.
  t_membuf2d( int m, int n, const T& val );

  t_membuf2d( const t_membuf2d<T>& );

  t_membuf2d<T>& operator=( const t_membuf2d<T>& src );

  // Изменение размеров матрицы.
  void resize( int m, int n );

  // присвоить всем элементам значение val
  void set_val( const T& val );

  // Получение размеров матрицы.
  int str() const { return _str; }
  int col() const { return _col; }

  T& operator()( int i, int j ) {
    TEST_INDEX2(i, _str, j, _col);
    return _repr[i*_col + j];
  }
  const T& operator()( int i, int j ) const {
    TEST_INDEX2(i, _str, j, _col);
    return _repr[i*_col + j];
  }

  // Низкоуровневый доступ к "сырым" данным.
        T* access_raw_data()       { return _repr.access_raw_data(); }
  const T* access_raw_data() const { return _repr.access_raw_data(); }

private:
  int _str, _col;
  t_membuf<T> _repr;
};

template<typename T>
t_membuf2d<T>::t_membuf2d()
  : _str(0), _col(0), _repr(0)
{
}

template<typename T>
t_membuf2d<T>::t_membuf2d( int m, int n )
  : _str(m), _col(n), _repr(_str*_col)
{
}

template<typename T>
t_membuf2d<T>::t_membuf2d( int m, int n, const T& val )
  : _str(m), _col(n), _repr(_str*_col, val)
{
}

template<typename T>
t_membuf2d<T>::t_membuf2d( const t_membuf2d<T>& src )
  : _str(src.str()), _col(src.col()), _repr(src._repr)
{
}

template<typename T>
t_membuf2d<T>& t_membuf2d<T>::operator=( const t_membuf2d<T>& src )
{
  if( &src == this ) return *this;
  test_size2(src.str(), str(), src.col(), col());
  _repr = src._repr;
  return *this;
}

template<typename T>
void t_membuf2d<T>::resize( int m, int n )
{
  _str = m;
  _col = n;
  _repr.resize(_str*_col);
}

template<typename T>
void t_membuf2d<T>::set_val( const T& val )
{
  _repr.set_val(val);
}

}; // namespace lwml

#endif // _T_MEMBUF2D_
