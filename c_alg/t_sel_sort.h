// �������� selection sort
// lwml, (c) ltwood

#ifndef _SEL_SORT_
#define _SEL_SORT_

#include "defs.h"
#include "ivector.h"
#include "vector.h"
#include "mdefs.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

// ��� ���������� �������� ������ �� ����������, � �������� ������,
// ���, ��� ���� src ��������� ������� 
//   int len() const;
//   int comp( int i, int j ) const;
// �  
//   sel_sort<srcT> idx(src);
// ��
//   src[idx[j]] <= src[idx[j+1]]

template<typename T> 
class t_sel_sort : public value {
public:
  enum direction { SORT_INC, SORT_DEC };

  t_sel_sort() {}

  t_sel_sort( const T& src, direction dir = SORT_INC ) { put(src, dir); }

  void put( const T& src, direction dir = SORT_INC );

  // �������� ����� �������.
  int len() const { return _data.len(); }

  // �������� j-� ������� �������.
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
