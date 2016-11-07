// Алгоритм selection sort
// lwml, (c) ltwood

#ifndef _SEL_SORT_
#define _SEL_SORT_

#include "lwml/base/defs.h"
#include "lwml/m_types/ivector.h"
#include "lwml/m_types/vector.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/base/basex.h"

/*#lake:stop*/

namespace lwml {

// При сортировке исходный объект не изменяется, а строится индекс,
// так, что если src реализует функции 
//   int len() const;
//   int comp( int i, int j ) const;
// и  
//   sel_sort<srcT> idx(src);
// то
//   src[idx[j]] <= src[idx[j+1]]

template<typename T> 
class t_sel_sort : public value {
public:
  enum direction { SORT_INC, SORT_DEC };

  t_sel_sort() {}

  t_sel_sort( const T& src, direction dir = SORT_INC ) { put(src, dir); }

  void put( const T& src, direction dir = SORT_INC );

  // Получить длину индекса.
  int len() const { return _data.len(); }

  // Получить j-й элемент индекса.
  int operator[]( int j ) const { return _data[j]; }

private:
  int_vector _data;
};

template<typename T>
void t_sel_sort<T>::put( const T& src, direction dir )
{
  int n = src.len();
  _data.resize(n);

  for( int j = 0; j < n; j++ )
    _data[j] = j;

  for( int i = 0; i < n-1; ++i ){
    for( int j = n-1; j > i; --j ){
      int sc = src.comp(_data[j], _data[i]);
      if( sc < 0 || (sc > 0 && dir == SORT_DEC) )
        _data.swap(j, i);
    }
  }
}

}; // namespace lwml

#endif // _SEL_SORT_
