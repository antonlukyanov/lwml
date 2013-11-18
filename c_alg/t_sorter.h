// Алгоритм пирамидальной сортировки
// lwml, (c) ltwood

#ifndef _SORTER_
#define _SORTER_

#include "defs.h"
#include "ivector.h"
#include "mdefs.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

// Класс t_sorter реализует алгоритм пирамидальной сортировки.
// При сортировке исходный объект не изменяется, а строится индекс,
// так, что если src реализует функции 
//   int len() const;
//   int comp( int i, int j ) const;
// и  
//   t_sorter<srcType> idx(src);
// то
//   src[idx[j]] <= src[idx[j+1]]

template<typename T>
class t_sorter : public value {
public:
  enum direction { SORT_DEC, SORT_INC };

  // Сконструировать сортировщик для объектов длины size.
  // Длина должна точно соответствовать длине сортируемых объектов.
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

  // Собственно сортировка объекту.
  // Исходный объект не изменяется, по нему строится индекс.
  // При вызове можно указать, сколько первых элементов
  // отсортированной последовательности нужно получить.
  void put( const T&, int n = -1 );

  // Получить длину индекса.
  int len() const { return _data.len(); }

  // Получить j-й элемент индекса.
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

// При реализации алгоритма пирамидальной сортировки
// за основу взят алгоритм из книги
// Т. Кормен, Ч. Лейзерсон, Р. Ривест Алгоритмы: построение и анализ.
// М.: МЦНМО, 2001. (См. стр. 138--145)
// Сортировка осуществляется в порядке, обратном заданному с тем,
// чтобы естественным образом реализовать отыскание нескольких первых
// элементов отсортированного массива (при пирамидальной сортировке
// по убыванию сначала получаются последние т.е. минимальные элементы).
// После сортировки массив индексов переворачивается.
// При этом сделаны следующие изменения, связанные с тем,
// что для нас естественная нумерация элементов начинается
// с 0, а не с 1 как в книге.
// 1. На стр. 138--139 вычисление предка осуществляется
// выражением (i-1)/2, вычисление левого ребенка - (2*i+1),
// правого ребенка - (2*i+2).
// 2. Проверка принадлежности элемента с номером i куче
// осуществляется так: (0 <= i && i < n), где n - размер кучи
// (такие проверки в книге равномерно распределены по всем исходникам).
// 3. На стр. 142 листьями являются элементы с номерами (n/2)...(n-1).
// Соответственно в коде на стр. 143 параметр цикла должен
// изменяться от n/2-1 до 0 включитально.
// 4. На стр. 145 параметр цикла должен изменяться от n-1 до 0
// включительно, а вызов функции Heapify должен иметь вид Heapify(A, 0).

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
