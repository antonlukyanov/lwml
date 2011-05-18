// ���������� ����������� �������

#ifndef _SPFUNC_
#define _SPFUNC_

#include "defs.h"
#include "mdefs.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

class spfunc : public scope {
public:
  // �����-�������
  // �(a) = \int_0^{\infty} t^{a-1} e^{-t} dt,
  // �(a+1) = a! = a * (a-1)! = a * �(a),
  // �(1) = 0! = 1, �(2) = 1 * �(1) = 1.
  static real log_gamma( real x );

  // �������� ���������� ������ �����
  static real log_factr( int n );

  // �������� ����-�������
  static real log_beta( real a, real b );

  // ����-�������
  // B(a,b) = \int_{0}^{1} t^{a-1} (1-t)^{b-1} dt
  static real beta( real a, real b );

  // �������� ������������� ������������
  static real log_bico( int n, int k );

  // �������� ������������� ������������
  static real bico( int n, int k );

private:
  static const long double lg_coeff[20];
};

inline real spfunc::log_factr( int n )
{
  if( n < 0 )
    fail_assert("inadmissible negative argument");
  if( n <= 1 )
    return 0.0;
  return log_gamma(n + 1.0);
}

inline real spfunc::log_beta( real a, real b )
{
  return log_gamma(a) + log_gamma(b) - log_gamma(a + b);
}

inline real spfunc::beta( real a, real b )
{
  return exp(log_beta(a, b));
}

inline real spfunc::log_bico( int n, int k ){
  return log_factr(n) - log_factr(k) - log_factr(n - k);
}

inline real spfunc::bico( int n, int k ){
  return floor(0.5 + exp(log_bico(n, k)));
}

}; // namespace lwml

#endif // _SPFUNC_
