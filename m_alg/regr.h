// Линейная регрессия
// lwml, (c) ltwood

#ifndef _REGR_
#define _REGR_

#include "defs.h"
#include "vector.h"

/*#lake:stop*/

namespace lwml {

// Класс lregr реализует вычисление коэффициентов
// линейной (возможно взвешенной) регрессии
// Вычислении обычных регрессионных коэффициентов
// реализует функция put(*,*), которой передаются вектор
// значений аргумента и вектор значений функции
// Вычислении обычных регрессионных коэффициентов
// реализует функция put(*,*,*), которой дополнительно
// передаются вектор весов отсчетов
// Функции k() и с() позволяют получить вычисленные параметры,
// а оператор operator() позволяет вычислить значение
// полученной линейной функции

class lregr : public value {
public:
  lregr() : _k(0.0), _c(0.0) {}

  void put( const vector& x, const vector& y );
  void put( const vector& x, const vector& y, const vector& w );

  real k() const { return _k; } // получить угловой коэффициент
  real c() const { return _c; } // получить смещение

  // вычислить значение линейной функции
  real operator()( real x ) const { return _k * x + _c; }

private:
  real _k, _c;

  real midw( const vector& x, const vector& w, real ws );
};

}; // namespace lwml

#endif // _REGR_
