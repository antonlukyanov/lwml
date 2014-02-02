#include "randset.h"

/*#lake:stop*/

namespace lwml {

// ГСЧ Карлина Анатолия Кузьмича

real krand::get()
{
  double c = frac(_a * _b * 1e6);
  _a = _b;
  _b = c;
  while( _b < 0.1 )
    _b *= 10;
  return c;
}

}; // namespace lwml
