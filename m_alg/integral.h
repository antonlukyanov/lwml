#ifndef _INTEGRAL_
#define _INTEGRAL_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/m_base/i_func.h"

//!! TODO: проверить работу с абсолютной ошибкой

/*#lake:stop*/

namespace lwml {

// вычисление интеграла
// функция fixed() возвращает значение интеграла на отрезке [a,b],
// вычисленного при фиксированном разбиении отрезка на sect интервалов
// функция adapt() возвращает значение интеграла на отрезке [a,b],
// вычисленное в результате последовательного измельчения исходного отрезка
// начиная с разбиения на sect интервалов
// вплоть до стабилизации оценки с относительной точностью eps

const int INTEGRAL_STDSECT = 32;

class integral : public scope {
public:
  static real fixed( const i_function&, real a, real b, int sect = INTEGRAL_STDSECT );
  static real adapt( const i_function&, real a, real b, int sect = INTEGRAL_STDSECT, real eps = STDEPS );
};

}; // namespace lwml

#endif // _INTEGRAL_

