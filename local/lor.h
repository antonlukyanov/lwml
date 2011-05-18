#ifndef _LOR_
#define _LOR_

#include "mdefs.h"
#include "rk4de.h"
#include "stdmem.h"

/*#lake:stop*/

namespace lwml {

// —истема Ћоренца с хаотическим поведением.
// ’аотическое поведение реализуетс€ при следующих значених параметров:
// s = 10.0, b = 2.(6), r = 28.0, разумное значение шага: h = 0.01,
// начальное значение: (10.0,10.0,10.0).

const int LORENZ_NUMEQ = 3;               // number of equations

class lorenz_eq : public i_rk4de_func { // public refcount
private:
  lorenz_eq( real s, real b, real r ) : _s(s), _b(b), _r(r) {}

public:
  static referer<lorenz_eq> create( real s, real b, real r ) {
    return referer<lorenz_eq>(new(lwml_alloc) lorenz_eq(s, b, r));
  }

  virtual void func( real t, const vector& x, vector& dx );

private:
  real _s, _b, _r;
};

// —истема Ћоренца в касательном пространстве (т.н. система в вариаци€х).
// »спользуетс€ при расчете показателей Ћ€пунова.

/*
class lorenz_tang_eq : public i_rk4de_func { // public refcount
private:
  lorenz_tang_eq( real s, real b, real r );

public:
  static referer<lorenz_tang_eq> create( real s, real b, real r ) {
    return referer<lorenz_tang_eq>(new(lwml_alloc) lorenz_tang_eq(s, b, r));
  }

  virtual void func( real t, const vector& x, vector& dx );

private:
  real _s, _b, _r;
};
*/

// extern real **e;
// void lds_init(void);
// void lds_step(void);
// void lds_done(void);

}; // namespace lwml

#endif // _LOR_
