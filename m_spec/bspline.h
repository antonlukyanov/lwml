// ������� B-������

#ifndef _BSPLINE_
#define _BSPLINE_

// ����� �������� ���������� ��������� ���������� �������� B-�������.
//   ����� ������ ����� p_0, p_1, ..., p_{n-1},
//   ��������������� ��������� 0, 1, ..., n-1 ��������� t.
//   �� ���� ������ �������� �������-�������������� ������� (������) P(t).
//   �� ������ ������� � �������� ������� ���������� ���������� �������,
//   �������� �������� ���������� �� 0 �� 1.
//   ������� [j, j+1] ������������� ������� P_j(t), ������� ��������
//   �� ��������� p_{j-1}, p_j, p_{j+1}, p_{j+2} (�.�. 1 <= j < n-2).
//   ����� �������, ��� j < t < j+1 ��������� ���������
//     P(t) = P_j(t-j).
//   ��� ����������� ������� ����������� ������������ ���������:
//     P_j(0) = (p_{j-1} + 4p_j + p_{j+1})/6,
//     P_j(1) = (p_j + 4p_{j+1} + p_{j+2})/6,
//   �.�. P_j(0) � P_j(1) �� ��������� � p_j � p_{j+1}.
//   B-������ �� �������� ����� ����� �����, �� ������� �� ��������!
//   ����������� ������ � ��� ������ 2 ����������� ��������
//   ������������ ���������.
// ������� set() ��������� �������� ������������� ����������� ��������
// �� ��������� p_{j-1}, p_j, p_{j+1}, p_{j+2} - ��� ��������� p1, p2, p3, p4.
// ������� calc() ��������� �������� ������������ ��������
// ��� ��������� ��������� �� ������� [0, 1].

#include "defs.h"
#include "mdefs.h"
#include "polyn.h"

/*#lake:stop*/

namespace lwml {

class bspline : public value {
public:
  void set( real p1, real p2, real p3, real p4 );

  real calc( real );

private:
  real _data[4];
};

}; // namespace lwml

#endif // _BSPLINE_
