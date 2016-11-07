#ifndef _HERST_
#define _HERST_

//!! TODO: переделать

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/m_types/vector.h"

/*#lake:stop*/

namespace lwml {

class herst : public value {
public:
  herst( int len );

  void put( const vector& x );

  int len() const { return _len; }

  const vector& arg() const { return _lp; }
  const vector& val() const { return _ls; }

private:
  int _len;
  vector _lp, _ls;
};

}; // namespace lwml

#endif // _HERST_
