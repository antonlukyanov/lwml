#ifndef _LFACTOR_
#define _LFACTOR_

#include "defs.h"
#include "mdefs.h"
#include "t_membuf.h"

/*#lake:stop*/

namespace lwml {

class lfactor : public value {
public:
  lfactor( double, int maxlen = 100 );

  int len() const { return _len; }

  double mult( int idx ) const;
  int powr( int idx ) const;

private:
  int _maxlen, _len;
  t_membuf<double> _mult;
  t_membuf<int> _powr;

  void factorize( double n );
  void put( double d, int k = 1 );
};

inline double lfactor::mult( int idx ) const {
  test_index(idx, _len);
  return _mult[idx];
}

inline int lfactor::powr( int idx ) const {
  test_index(idx, _len);
  return _powr[idx];
}

}; // namespace lwml

#endif // _LFACTOR_
