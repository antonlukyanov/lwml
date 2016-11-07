#include "lwml/m_types/polyn.h"

/*#lake:stop*/

namespace lwml {

void polyn::set_zero()
{
  for( int j = 0; j < _data.len(); j++ )
    _data[j] = 0.0;
}

real polyn::operator()( real x ) const
{
  real res = _data[deg()];
  for( int k = deg() - 1; k >= 0; k-- )
    res = res * x + _data[k];
  return res;
}

polyn polyn::deriv( const polyn& src )
{
  polyn res(src.deg() - 1);
  res.set_zero();
  for( int j = 0; j <= src.deg() - 1; j++ )
    res[j] = (j+1) * src[j+1];
  return res;
}

polyn polyn::integ( const polyn& src )
{
  polyn res(src.deg() + 1);
  res.set_zero();
  for( int j = 1; j <= src.deg() + 1; j++ )
    res[j] = src[j-1] / j;
  return res;
}

}; // namespace lwml
