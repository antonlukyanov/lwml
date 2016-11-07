// Метод золотого сечения
// lwml, (c) ltwood

#ifndef _GOLDSECT_
#define _GOLDSECT_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/m_base/i_func.h"

/*#lake:stop*/

namespace lwml {

// Класс gminf реализует поиск минимума функции
// на отрезке [a,b] методом золотого сечения
// Функция calc() возвращает значение аргумента,
// минимизирующего функцию на отрезке [a,b]
// Точка минимума будет найдена с относительной
// погрешностью eps
// Наследники этого класса переопределяют чистую
// виртуальную функцию func()

class gminf : public scope {
public:
  static real calc( const i_function&, real a, real b, real eps = STDEPS );
};

}; // namespace lwml

#endif // _GOLDSECT_
