// ����������� ������� ������� ����-������ (Hooke and Jeeves)
// lwml, (c) ltwood

#ifndef _HOOKE_
#define _HOOKE_

#include "defs.h"
#include "mdefs.h"

#include "i_vfunc.h"

/*#lake:stop*/

namespace lwml {

/*
 * ������ �� ��������� ������������� ������� f(X), ��� ���
 * �� ����������� � �� ��������� ������������� �����������.
 *
 * ������:
 * R.Hooke, T.A.Jeeves, "Direct Search Solution of Numerical
 * and Statistical Problems", Journal of the ACM, Vol.8,
 * April 1961, pp.212-229.
 * "Algorithm 178: Direct Search" by Arthur F. Kaupe Jr.,
 * Communications of the ACM, Vol.6, p.313 (June 1963).
 * Some improvements: Bell and Pike (CACM v.9, p.684, Sept 1966)
 * and Tomlin and Smith, "Remark on Algorithm 178" (CACM v.12).
 */

/*
 * ��������� ��������� ����� ��� �� �����������, �������
 * ����� ����� ����� ���� ������������.
 * ������� calc() ���������� ���������� ��������� �����.
 *
 * �������� rho ������������ ��� ���������, �� ������� ���������� ���
 * ����� ������ ��������.
 * ����� ����� �������� ������������ ������������ rho ** N = eps.
 * ������� �������� ��������� rho ���� ������� ����������� ���������� ���
 * ������� ������� �� ���� �������� ����� ����������� �����.
 * ������ ����� ���������� � ������������ �������� rho = 0.5
 * � ������� �� ������� ����������.
 * ��� ��������� ��������������������� ���������� ����� ���������
 * rho �� �������������� �������� ������� 0.85.
 */

// ������ �������� ��������� ��� ������ ����-������.

class i_hooke_progress : public interface {
public:
  // ����� ������� ���� �������� �������� ������� up(),
  // ��������� �� ����� ������� �������� ����������
  // � ����� -- ����� �� ���������� [0, 1].
  virtual void up( real ) = 0;
};

class hooke : public scope {
public:
  // fn - �������������� �������
  // x0 - ��������� �����
  // x - ��������� �����������
  // dx - ���� �� ������ ����������
  // rho - ��������, ����������� ����������� (0 <= rho <= 1.0)
  // eps - �������� ���������, ����������� ���������� ���
  // maxit - ������������ ����� ��������
  // ������������ �������� -- ����� ���� n, �� ������� ��������� ��������� (0 <= n < maxit).
  // ���� ��������� ��������� ��-�� ����, ��� ��� ������ maxit ��������,
  // �� ������������ �������� maxit.
  static int calc(
    const i_vfunction& fn,
    vector& x0, vector& x, vector& dx,
    double rho, double eps, int maxit,
    i_hooke_progress* prg = 0
  );

  // ��� ������� ���������� ������ ���, ��� ������ ������� dx ����� �� ������ ����������
  // ��������� ������������ ���, ������������ ��� ��������� ��� ��� ���� ���������.
  static int calc(
    const i_vfunction& fn,
    vector& x0, vector& x, real dx_val,
    double rho, double eps, int maxit,
    i_hooke_progress* prg = 0
  );

private:
  static real local_search( const i_vfunction& fn, const vector& dx, vector& x, real fmin );
};

}; // namespace lwml

#endif // _HOOKE_
