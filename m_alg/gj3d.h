// –ешение трехдиагональной системы методом прогонки
// lwml, (c) ltwood

#ifndef _LU3D_
#define _LU3D_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"

/*#lake:stop*/

namespace lwml {

//   –ешение трехдиагональной системы методом прогонки
//   ld - нижн€€ поддиагональ (длина = n-1)
//   dg - главна€ диагональ (длина = n)
//   ud - верхн€€ поддиагональ (длина = n-1)
//   dd - вектор правых частей (длина = n)

class gj3d : public value {
public:
  gj3d( int sz );

  void calc( vector& x, const vector& ld, const vector& dg, const vector& ud, const vector& dd );

private:
  int _size;
  vector _aa, _bb;
};

}; // namespace lwml

#endif // _LU3D_
