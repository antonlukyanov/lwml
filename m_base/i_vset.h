// Набор векторов
// lwml, (c) ltwood

#ifndef _I_VSET_
#define _I_VSET_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"

/*#lake:stop*/

namespace lwml {

// Интерфейс объекта, хранящего набор векторов.
// Обычно используется в алгоритмах кластеризации.
// Потомки переопределяют следующие виртуальные функции:
//   len() - возвращает число векторов
//   dim() - возвращает размерность векторов
//   get() - возвращает d-ю компоненту вектора с номером j
//   get( vector& ) - помещает в переданный хранимый вектор с номером j

class i_vector_set  : public interface {
public:
  virtual int len() const = 0;
  virtual int dim() const = 0;
  virtual real get( int j, int d ) const = 0;
  virtual void get( vector&, int j ) const = 0;

  real operator()( int j, int d ) const { return get(j, d); }
};

}; // namespace lwml

#endif // _I_VSET_
