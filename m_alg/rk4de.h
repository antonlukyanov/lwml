#ifndef _MRK4DE_
#define _MRK4DE_

#include "lwml/m_base/mdefs.h"
#include "lwml/m_types/vector.h"
#include "lwml/base/refcount.h"

/*#lake:stop*/

namespace lwml {

// Runge-Kutt order 4

class i_rk4de_func : public interface, public refcount {
public:
  virtual void func( real t, const vector& x, vector& dx ) = 0;
};

class rk4de : public value {
public:
  rk4de( real t0, const vector& x0, referer<i_rk4de_func> func );
  virtual ~rk4de() {}

  void step( real h, int st = 1 );

  real time()                      { return _tcur; }
  int  len()                       { return _size; }

  real operator[]( int j ) const   { return _data[j]; }

  void get( vector& dst ) const;

private:
  int     _size;                   // порядок системы
  vector  _data;                   // текущее состоние
  vector  _xtmp;                   // промежуточное состоние
  vector  _k0, _k1, _k2, _k3;      // рабочие области
  real    _tcur;                   // текущий момент времени
  referer<i_rk4de_func> _func;     // правые части

  void add2tmp( vector& src, real al );
};

}; // namespace lwml

#endif // _MRK4DE_
