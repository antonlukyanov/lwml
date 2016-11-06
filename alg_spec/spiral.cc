#include "spiral.h"

/*#build_stop*/

namespace lwml {

namespace {
  int dx[4] = { 1, 0, -1, 0 };
  int dy[4] = { 0, -1, 0, 1 };
};

void spiral::step()
{
  _x += dx[_segm % 4];
  _y += dy[_segm % 4];
  _steps--;
  if( _steps == 0 ){
    _segm++;
    _steps = _segm/2 + 1;
  }
}

}; // namespace lwml
