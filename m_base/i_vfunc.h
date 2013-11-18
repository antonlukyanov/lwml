// Функция вида f: R^n -> R.
// lwml, (c) ltwood

#ifndef _I_VFUNC_
#define _I_VFUNC_

#include "defs.h"
#include "vector.h"

/*#lake:stop*/

namespace lwml {

// Абстрактный предок классов-функций вида f: R^n -> R.
// Потомки переопределяют чистую виртуальную функцию func(),
// принимающую вектор и возвращающую вещественное число.

class i_vfunction : public interface {
public:
  // Переопределяемая функция.
  virtual real func( const vector& ) const = 0;

  // Функциональный оператор применения функции к вектору.
  real operator()( const vector& x ) const {
    return func(x);
  }
};

}; // namespace lwml

#endif // _I_VFUNC_
