#include "canvas.h"

/*#lake:stop*/

namespace lwml {

void i_canvas::reset()
{
  _newflag = true;
}

void i_canvas::point( real x, real y )
{
  if( !_newflag )
    line(_cx, _cy, x, y);
  _cx = x;
  _cy = y;
  _newflag = false;
}

}; // namespace lwml
