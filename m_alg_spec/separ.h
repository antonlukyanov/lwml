#ifndef _SEPAR_
#define _SEPAR_

#include "lwml/base/defs.h"
#include "lwml/m_types/vector.h"
#include "lwml/m_base/i_vset.h"

/*#lake:stop*/

namespace lwml {

class delta_separ : public scope {
public:
  static real calc( const i_vector_set& m1, const i_vector_set& m2 );
  static void weights( vector& a, const i_vector_set& m1, const i_vector_set& m2 );

private:
  static void setmid( vector& mid, const i_vector_set& matr );
  static real inner( const vector& a, const i_vector_set& m, int j );
};

class gamma_separ : public value {
public:
  gamma_separ( const i_vector_set& m1, const i_vector_set& m2 );

  real val() const;
  real max_val( real p ) const;

  real p1() const { return real_cast(_self1) / _num1; }
  real p2() const { return real_cast(_self2) / _num2; }

private:
  static real dist( const i_vector_set& m1, int j, const i_vector_set& m2, int i );
  static int num( const i_vector_set& m1, const i_vector_set& m2 );

  int _num1, _num2;
  int _self1, _self2;

  static real mid( int n1, int n2 );
  static real sigma2( int n1, int n2 );
};

}; // namespace lwml

#endif // _SEPAR_
