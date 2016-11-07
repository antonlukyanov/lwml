// Класс-функция
// lwml, (c) ltwood

#ifndef _I_FUNC_
#define _I_FUNC_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"

/*#lake:stop*/

namespace lwml {

// Абстрактный предок классов-функций.
// Потомки переопределяют чистую виртуальную функцию func().

class i_function : public interface {
public:
  virtual real func( real ) const = 0;

  real operator()( real x ) const {
    return func(x);
  }
};

class i_function2 : public interface {
public:
  virtual real func( real, real ) const = 0;

  real operator()( real x, real y ) const {
    return func(x, y);
  }
};

}; // namespace lwml

#endif // _I_FUNC_
