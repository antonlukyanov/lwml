// Вычисление скользящей суммы
// lwml, (c) ltwood

#ifndef _FLYSUM_
#define _FLYSUM_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"

/*#lake:stop*/

namespace lwml {

// Вычисление скользящей суммы.
// В каждый элемент вектора dst помещается сумма элементов
// вектора src, расположенных в окне с полушириной len2,
// с центром в текущей позиции.
// Фактическая длина окна составляет 2 * len2 + 1 отсчетов.

class flysum : public scope {
public:
  static void calc( vector& dst, const vector& src, int len2 );
};

}; // namespace lwml

#endif // _FLYSUM_
