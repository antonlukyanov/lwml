#include "lwml/m_alg/rk4de.h"

/*#lake:stop*/

namespace lwml {

rk4de::rk4de( real t0, const vector& x0, referer<i_rk4de_func> func )
  : _size(x0.len()),
    _data(x0.len()), _xtmp(x0.len()),
    _k0(x0.len()), _k1(x0.len()), _k2(x0.len()), _k3(x0.len()),
    _tcur(t0), _func(func)
{
  _data = x0;
}

void rk4de::add2tmp( vector& src, real al )
{
  for( int j = 0; j < _size; j++ )
    _xtmp[j] = _data[j] + al * src[j];
}

void rk4de::step( real h, int st )
{
  for( int k = 0; k < st; k++ ){
    real t = _tcur;

    _func->func(t, _data, _k0);
    add2tmp(_k0, h / 2.0);
    _func->func(t + h / 2.0, _xtmp, _k1);
    add2tmp(_k1, h / 2.0);
    _func->func(t + h / 2.0, _xtmp, _k2);
    add2tmp(_k2, h);
    _func->func(t + h, _xtmp, _k3);

    for( int j = 0; j < _size; j++ )
      _data[j] += (h/6.0) * (_k0[j] + 2.0*(_k1[j] + _k2[j]) + _k3[j]);
    _tcur += h;
  }
}

void rk4de::get( vector& dst ) const
{
  test_size(dst.len(), _size);

  dst = _data;
}

}; // namespace lwml
