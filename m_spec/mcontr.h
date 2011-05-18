// Matrix contrasting
// lwml, (c) ltwood

#ifndef _MCONTR_
#define _MCONTR_

#include "defs.h"
#include "mdefs.h"

#include "matrix.h"

/*#lake:stop*/

namespace lwml {

// Изиенение контраста матрицы с сохранением диапазона изменения ее элементов.
// Функция upcontr() повышает контраст "изображения", функция decontr() -- снижает его.
// Параметр par принимает значения из диапазона (0, 0.5) и определяет степень изменения контраста.
// Параметр sh также принимает значения из диапазона (0, 0.5) и определяет ширину переходной зоны 
// противоположного изменения контраста, расположенной у краев шкалы яркостей 
// и предназначенной для сохранения диапазона значений элементов матрицы.

class mcontr : public scope {
public:
  static void upcontr( matrix&, real par, real sh = 0.0 );
  static void decontr( matrix&, real par, real sh = 0.0 );

private:
  static void transform( matrix&, real a, real b, const line_func& lf, const line_func& cf, const line_func& rf );
};

}; // namespace lwml

#endif // _MCONTR_
