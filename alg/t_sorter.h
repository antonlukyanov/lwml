// �������� ������������� ����������
// lwml, (c) ltwood

#ifndef _SORTER_
#define _SORTER_

#include "defs.h"
#include "ivector.h"
#include "mdefs.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

// ����� t_sorter ��������� �������� ������������� ����������.
// ��� ���������� �������� ������ �� ����������, � �������� ������,
// ���, ��� ���� src ��������� ������� 
//   int len() const;
//   int comp( int i, int j ) const;
// �  
//   t_sorter<srcType> idx(src);
// ��
//   src[idx[j]] <= src[idx[j+1]]

template<typename T>
class t_sorter : public value {
public:
  enum direction { SORT_DEC, SORT_INC };

  // ��������������� ����������� ��� �������� ����� size.
  // ����� ������ ����� ��������������� ����� ����������� ��������.
  t_sorter( int size, direction dir = SORT_INC )
    : _data(size), _revers(dir == SORT_DEC)
  {
    set_eq();
  }

  t_sorter( const T& src )
    : _data(src.len()), _revers(false)
  {
    put(src);
  }

  // ���������� ���������� �������.
  // �������� ������ �� ����������, �� ���� �������� ������.
  // ��� ������ ����� �������, ������� ������ ���������
  // ��������������� ������������������ ����� ��������.
  void put( const T&, int n = -1 );

  // �������� ����� �������.
  int len() const { return _data.len(); }

  // �������� j-� ������� �������.
  int operator[]( int j ) const { return _data[j]; }

private:
  int_vector _data;
  bool _revers;

  int comp( const T& src, int i, int j ){
    return src.comp(_data[i], _data[j]) * (_revers ? 1 : -1);
  }

  int lchild( int parent ) const { return 2 * parent + 1; }
  int rchild( int parent ) const { return 2 * parent + 2; }
  int parent( int child )  const { return (child-1) / 2; }

  void heapify( const T& src, int i, int n );
  void build_heap( const T& src, int n );
  void heapsort( const T& src, int m );

  void set_eq();
};

// ��� ���������� ��������� ������������� ����������
// �� ������ ���� �������� �� �����
// �. ������, �. ���������, �. ������ ���������: ���������� � ������.
// �.: �����, 2001. (��. ���. 138--145)
// ���������� �������������� � �������, �������� ��������� � ���,
// ����� ������������ ������� ����������� ��������� ���������� ������
// ��������� ���������������� ������� (��� ������������� ����������
// �� �������� ������� ���������� ��������� �.�. ����������� ��������).
// ����� ���������� ������ �������� ����������������.
// ��� ���� ������� ��������� ���������, ��������� � ���,
// ��� ��� ��� ������������ ��������� ��������� ����������
// � 0, � �� � 1 ��� � �����.
// 1. �� ���. 138--139 ���������� ������ ��������������
// ���������� (i-1)/2, ���������� ������ ������� - (2*i+1),
// ������� ������� - (2*i+2).
// 2. �������� �������������� �������� � ������� i ����
// �������������� ���: (0 <= i && i < n), ��� n - ������ ����
// (����� �������� � ����� ���������� ������������ �� ���� ����������).
// 3. �� ���. 142 �������� �������� �������� � �������� (n/2)...(n-1).
// �������������� � ���� �� ���. 143 �������� ����� ������
// ���������� �� n/2-1 �� 0 ������������.
// 4. �� ���. 145 �������� ����� ������ ���������� �� n-1 �� 0
// ������������, � ����� ������� Heapify ������ ����� ��� Heapify(A, 0).

template<typename T>
void t_sorter<T>::heapify( const T& src, int i, int n )
{
  int l = lchild(i);
  int r = rchild(i);
  int largest = i;
  if( l < n && comp(src, l, largest) > 0 )
    largest = l;
  if( r < n && comp(src, r, largest) > 0 )
    largest = r;
  if( largest != i ){
    _data.swap(i, largest);
    heapify(src, largest, n);
  }
}

template<typename T>
void t_sorter<T>::build_heap( const T& src, int n )
{
  for( int i = n/2-1; i >= 0; i-- )
    heapify(src, i, n);
}

template<typename T>
void t_sorter<T>::heapsort( const T& src, int m )
{
  int n = src.len();
  build_heap(src, n);
  int k = 0;
  for( int i = n-1; i >= 1; i-- ){
    _data.swap(0, i);
    heapify(src, 0, i);
    if( ++k == m ) break;
  }
}

template<typename T>
void t_sorter<T>::set_eq()
{
  for( int j = 0; j < _data.len(); j++ )
    _data[j] = j;
}

template<typename T>
void t_sorter<T>::put( const T& src, int n )
{
  test_size(src.len(), _data.len());
  set_eq();
  heapsort(src, n);
  _data.revers();
}

}; // namespace lwml

#endif // _SORTER_
