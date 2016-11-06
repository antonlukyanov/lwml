// Iterative statistic
// lwml, (c) ltwood

#ifndef _ISTAT_
#define _ISTAT_

#include "defs.h"
#include "mdefs.h"

#include "vector.h"
#include "t_array.h"

/*#lake:stop*/

namespace lwml {

/*
 * ����������� ���������� �������� � �������� (��-��. ���������).
 * ������� ��� ����� ��������� "��������".
 * �� ������ ���� ������� � �������� ����������� ������ �� "�������" ������.
 * ����� ����� ��� �����, �� �������� � alpha*dev-����������� ��������,
 * ����������� �� ��������� "�������".
 * ���������� ����������� �� ������������ ��������� "�������" �����.
 * ������� ���������� ���������� �����, �������������
 * � ���������� �������������� ������ mid � dev.
 */

class iter_stat : public scope {
public:
  // ���������� ��� ����������� ��������.
  static int calc( const vector& d, real alpha, real* mid, real* dev );

  // ���������� ��� �������� � ��������� ������.
  static int calc( const vector& d, const vector& w, real alpha, real* mid, real* dev );

private:
  static void calc_mid( real* mid, real* dev, const vector& d, const t_array<bool>& fl, const vector* w);

  static int common_calc( real* mid, real* dev, real alpha, const vector& d, const vector* w = 0 );
};

}; // namespace lwml

#endif // _ISTAT_
