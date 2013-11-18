// FFT utilities
// lwml, (c) ltwood

#ifndef _FFTUTL_
#define _FFTUTL_

#include "defs.h"
#include "mdefs.h"

#include "matrix.h"

/*#lake:stop*/

namespace lwml {

class fftutl : public scope {
public:
  static void copy_cnt_spectrum( matrix& dst, const matrix& src );
};

}; // namespace lwml

#endif // _FFTUTL_
