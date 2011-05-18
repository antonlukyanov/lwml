// ����������� ��� ��������� �����������

#ifndef _VGADGETS_
#define _VGADGETS_

#include "defs.h"
#include "geom.h"

/*#lake:stop*/

namespace lwml {

// ����� viewrect �������� ����������� �������, ��������������� ���
// ���������� �������� ������������� ��������, ���������� ��������
// ����������� �������������� (������������ "������� �������")
// ��� ������������� ���� (������, ������) ������������ ����� fpoint
// � ������� x() ���������������� ��� ������, � y() - ��� ������

class viewrect : public scope {
public:
  // �������� "�������" ������������� �� ������
  static fpoint gold_rect( real width );

  // �������� � maxviewrect �������������
  // � ������������ ������ ��� � objrect
  static fpoint correct( const fpoint& maxviewrect, const fpoint& objrect );

  // �������� � ������� ������������� ������ width
  // ������������� � ������������ ������ ��� � objrect
  static fpoint correct( real width, const fpoint& objrect );
};

// ����� viewgrids �������� ����������� �������, ���������������
// ��� ������� ���������� ����� ��������� �� ������������ ����

class viewgrids : public scope {
public:
  // ������������ ���� �� ����� ���� *0000.0 ��� 0.0000*
  static real round( real );

  // �������� ��������������� ��� � ��������� �����������
  // �� �������������� ���� � ����������� �����������
  static real getgridstep(
    real obj_a, real obj_b,
    real view_a, real view_b,
    real view_step
  );

  // �������� ���������� ����� ����� �����
  // ������� ���� � ������� ������ obj_a
  static real getfirstgrid( real obj_a, real step );
};

}; // namespace lwml

#endif // _VGADGETS_
