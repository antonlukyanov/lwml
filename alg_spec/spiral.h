// Spiral iterator.
// lwml, (c) ltwood

#ifndef _SPIRAL_
#define _SPIRAL_

#include "defs.h"
#include "mdefs.h"

/*#build_stop*/

namespace lwml {

class spiral : public value {
public:
  spiral( int x, int y ) : _x(x), _y(y), _steps(1), _segm(0) {}

  void step();

  int x() const { return _x; }
  int y() const { return _y; }

private:
  int _x, _y;
  int _steps, _segm;
};

}; // namespace lwml

#endif // _SPIRAL_
