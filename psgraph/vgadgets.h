// Инструменты для рисования иллюстраций

#ifndef _VGADGETS_
#define _VGADGETS_

#include "lwml/base/defs.h"
#include "lwml/geometry/geom.h"

/*#lake:stop*/

namespace lwml {

// Класс viewrect содержит статические функции, предназначенные для
// вычисления размеров прямоугольных областей, выглядящих наиболее
// эстетически привлекательно (используется "золотое сечение")
// Для представления пары (ширина, высота) используется класс fpoint
// в котором x() интерпретируется как ширина, а y() - как высота

class viewrect : public scope {
public:
  // получить "золотой" прямоугольник по ширине
  static fpoint gold_rect( real width );

  // уместить в maxviewrect прямоугольник
  // с соотношением сторон как у objrect
  static fpoint correct( const fpoint& maxviewrect, const fpoint& objrect );

  // уместить в золотой прямоугольник ширины width
  // прямоугольник с соотношением сторон как у objrect
  static fpoint correct( real width, const fpoint& objrect );
};

// Класс viewgrids содержит статические функции, предназначенные
// для подбора расстояния между засечками на координатных осях

class viewgrids : public scope {
public:
  // нормализация шага до числа вида *0000.0 или 0.0000*
  static real round( real );

  // получить нормализованный шаг в объектных координатах
  // по рекомендуемому шагу в координатах изображения
  static real getgridstep(
    real obj_a, real obj_b,
    real view_a, real view_b,
    real view_step
  );

  // получить координату самой левой метки
  // кратной шагу и лежащей правее obj_a
  static real getfirstgrid( real obj_a, real step );
};

}; // namespace lwml

#endif // _VGADGETS_
