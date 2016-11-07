// Экспоненциальная фильтрация
// lwml, (c) ltwood

#ifndef _EXPFILT_
#define _EXPFILT_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/m_types/vector.h"
#include "lwml/m_types/matrix.h"

/*#lake:stop*/

namespace lwml {

class expfilt : public scope {
public:
  // Экспоненциальная симметричная фильтрация вектора img с параметром alpha [0, 1].
  // Большим значениям alpha соответствует большее сглаживание.
  static void calc( vector& img, real alpha );

  // Экспоненциальная симметричная фильтрация матрицы img с параметром alpha [0, 1].
  // Большим значениям alpha соответствует большее сглаживание.
  static void calc( matrix& img, real alpha );

  // Расчет параметра симметричного эспоненциального фильтра с заданной
  // fwhm (full width half magnitude)
  static real fwhm2alpha( real fwhm ) { return exp(-M_LN2 / (fwhm/2.0)); } // log(0.5)
};

}; // namespace lwml

#endif // _EXPFILT_
