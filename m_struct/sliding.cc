#include "sliding.h"

/*#lake:stop*/

namespace lwml {

sliding::sliding( const vector& v, int len, int shift )
{
  _src = &v;
  _win_len = len;
  _shift = shift;
  _win_num = calc_win_num(v.len(), len, shift);
}

}; // namespace lwml
