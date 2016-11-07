// FFT utilities
// lwml, (c) ltwood

#ifndef _FFTUTL_
#define _FFTUTL_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"

#include "lwml/m_types/matrix.h"

/*#lake:stop*/

namespace lwml {

class fftutl : public scope {
public:
  static void copy_cnt_spectrum( matrix& dst, const matrix& src );
};

}; // namespace lwml

#endif // _FFTUTL_
