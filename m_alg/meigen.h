#ifndef _MEIGEN_
#define _MEIGEN_

//!! TODO: проверить работу с абсолютной ошибкой

#include "defs.h"
#include "matrix.h"

/*#lake:stop*/

namespace lwml {

const int EIGEN_MAXIT = 10;

class eigenvalues : public scope {
public:
  static void calc( matrix& a, matrix& v, real eps = STDEPS, int maxit = EIGEN_MAXIT );

private:
  static void rrot( matrix&, int k, int l, real al, real be );
  static void lrot( matrix&, int k, int l, real al, real be );
  static void rot( matrix&, matrix& v, int k, int l, real al, real be );
  static void findoptel( const matrix&, const vector& r, int& k, int& l );
  static void calcr( const matrix&, vector& r );
  static void recalcr( const matrix&, vector& r, int k, int l );
  static void step( matrix& a, matrix& v, vector& r );
};

}; // namespace lwml

#endif // _MEIGEN_
