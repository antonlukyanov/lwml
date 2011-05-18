// Вычисление значений многочлена
// lwml, (c) ltwood

#ifndef _POLYN_
#define _POLYN_

#include "defs.h"
#include "mdefs.h"
#include "t_membuf.h"

/*#lake:stop*/

namespace lwml {

// Класс polyn представляет многочлен
// При конструировании указывается степень
// Поддержаны конструктор и оператор копирования
// Операторы operator[] обеспечивают доступ к коэффициентам
// Оператор operator() реализует схему Горнера вычисления значения
// Статические функции deriv() b integr() реализуют
// вычисление производной и интеграла от многочлена

class polyn : public value {
public:
  polyn( int deg ) : _data(deg + 1) {}
  polyn( const polyn& src ) : _data(src.len()) { _data = src._data; }

  polyn& operator=( const polyn& src ){
    if( &src == this ) return *this;
    _data = src._data;
    return *this;
  }

  int len() const { return _data.len(); }      // число коэффициентов
  int deg() const { return _data.len() - 1; }  // степень многочлена

        real& operator[]( int j )       { return _data[j]; }
  const real& operator[]( int j ) const { return _data[j]; }

  void set_zero(); // обнулить коэффициенты

  real operator()( real x ) const;

  static polyn deriv( const polyn& );
  static polyn integ( const polyn& );

private:
  t_membuf<real> _data;
};

}; // namespace lwml

#endif // _POLYN_
