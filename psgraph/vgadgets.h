// »нструменты дл€ рисовани€ иллюстраций

#ifndef _VGADGETS_
#define _VGADGETS_

#include "defs.h"
#include "geom.h"

/*#lake:stop*/

namespace lwml {

//  ласс viewrect содержит статические функции, предназначенные дл€
// вычислени€ размеров пр€моугольных областей, выгл€д€щих наиболее
// эстетически привлекательно (используетс€ "золотое сечение")
// ƒл€ представлени€ пары (ширина, высота) используетс€ класс fpoint
// в котором x() интерпретируетс€ как ширина, а y() - как высота

class viewrect : public scope {
public:
  // получить "золотой" пр€моугольник по ширине
  static fpoint gold_rect( real width );

  // уместить в maxviewrect пр€моугольник
  // с соотношением сторон как у objrect
  static fpoint correct( const fpoint& maxviewrect, const fpoint& objrect );

  // уместить в золотой пр€моугольник ширины width
  // пр€моугольник с соотношением сторон как у objrect
  static fpoint correct( real width, const fpoint& objrect );
};

//  ласс viewgrids содержит статические функции, предназначенные
// дл€ подбора рассто€ни€ между засечками на координатных ос€х

class viewgrids : public scope {
public:
  // нормализаци€ шага до числа вида *0000.0 или 0.0000*
  static real round( real );

  // получить нормализованный шаг в объектных координатах
  // по рекомендуемому шагу в координатах изображени€
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
