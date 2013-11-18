// ���������������� ����������
// lwml, (c) ltwood

#ifndef _EXPFILT_
#define _EXPFILT_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"
#include "matrix.h"

/*#lake:stop*/

namespace lwml {

class expfilt : public scope {
public:
  // ���������������� ������������ ���������� ������� img � ���������� alpha [0, 1].
  // ������� ��������� alpha ������������� ������� �����������.
  static void calc( vector& img, real alpha );

  // ���������������� ������������ ���������� ������� img � ���������� alpha [0, 1].
  // ������� ��������� alpha ������������� ������� �����������.
  static void calc( matrix& img, real alpha );

  // ������ ��������� ������������� ���������������� ������� � ��������
  // fwhm (full width half magnitude)
  static real fwhm2alpha( real fwhm ) { return exp(-M_LN2 / (fwhm/2.0)); } // log(0.5)
};

}; // namespace lwml

#endif // _EXPFILT_
