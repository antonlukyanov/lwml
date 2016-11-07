// Алгоритмы бинарного поиска
// lwml, (c) ltwood

#ifndef _SEARCH_
#define _SEARCH_

#include "lwml/base/defs.h"
#include "lwml/m_types/ivector.h"
#include "lwml/m_types/vector.h"
#include "lwml/alg/t_sorter.h"

/*#lake:stop*/

namespace lwml {

// Класс search содержит алгоритмы поиска в упорядоченных массивах.
// Класс search содержит статические функции, реализующие алгоритм
// двоичного поиска в упорядоченных массивах.

class search : public scope {
public:
  // Поиск элемента el в упорядосченном целочисленном массиве src.
  // Упорядоченность гарантируется передачей объекта t_sorter с построенным индексом.
  // Функция возвращают (-1) в случае безуспешного поиска.
  static int element( const int_vector& src, const t_sorter<int_vector>& idx, int el );

  // Поиск элемента el в упорядосченном целочисленном массиве src.
  // Упорядоченность массива гарантируется вызывающей функцией.
  // Функция возвращают (-1) в случае безуспешного поиска.
  static int element( const int_vector& src, int el );

  // Поиск межэлементного интервала, содержащего элемент el в
  // упорядосченном вещественном массиве.
  // Отыскивается такой элемент j массива src, что src[j] <= el < src[j+1].
  // Упорядоченность гарантируется передачей объекта t_sorter с построенным индексом.
  // Функция возвращают (-1) в случае безуспешного поиска.
  static int interval( const vector& src, const t_sorter<vector>& idx, real el );

  // Поиск межэлементного интервала, содержащего элемент el в
  // упорядосченном вещественном массиве.
  // Отыскивается такой элемент j массива src, что src[j] <= el < src[j+1].
  // Упорядоченность массива гарантируется вызывающей функцией.
  // Функция возвращают (-1) в случае безуспешного поиска.
  static int interval( const vector& src, real el );
};

}; // namespace lwml

#endif // _SEARCH_
