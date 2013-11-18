// медианный фильтр
// lwml, (c) ltwood

#ifndef _MEDIANFILT_
#define _MEDIANFILT_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"
#include "imatrix.h"

/*#lake:stop*/

namespace lwml {

class medianfilt : public scope {
public:
  enum topology { CIRCLE, LINE };
  
  // Медианный фильтр.
  // В каждый элемент вектора dst помещается медиана элементов
  // вектора src, расположенных в окне с полушириной rank,
  // с центром в текущей позиции.
  // Фактическая длина окна составляет 2 * rank + 1 отсчетов.
  // Если topology == CIRCLE, то вектор src считаеся замкнутым в кольцо,
  // иначе на концах происходит дополнение крайними значениями
  // ret: true, если были изменения
  static bool calc( vector& dst, const vector& src, int rank, topology top = LINE );

  // Медианная фильтрация изображения по квадратной окрестности радиуса apt.
  // Входная матрица содержит целочисленные отсчеты изображения
  // из некоторого небольшого диапазона.
  // Функция возвращает количество измененных элементов изображения.
  static int calc( int_matrix& dst, const int_matrix& src, int apt );

  // Медианная фильтрация изображения по прямоугольной окрестности.
  // В остальном контракт совпадает с контрактом предыдущей функции.
  static int calc( int_matrix& dst, const int_matrix& src, int apt_x, int apt_y );

  // Тестовая реализация.
  static void calc_test( int_matrix& dst, const int_matrix& src, int apt );
  static void calc_test( int_matrix& dst, const int_matrix& src, int apt_x, int apt_y );
};

}; // namespace lwml

#endif // _MEDIANFILT_
