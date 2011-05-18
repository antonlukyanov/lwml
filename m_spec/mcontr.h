// Matrix contrasting
// lwml, (c) ltwood

#ifndef _MCONTR_
#define _MCONTR_

#include "defs.h"
#include "mdefs.h"

#include "matrix.h"

/*#lake:stop*/

namespace lwml {

// ��������� ��������� ������� � ����������� ��������� ��������� �� ���������.
// ������� upcontr() �������� �������� "�����������", ������� decontr() -- ������� ���.
// �������� par ��������� �������� �� ��������� (0, 0.5) � ���������� ������� ��������� ���������.
// �������� sh ����� ��������� �������� �� ��������� (0, 0.5) � ���������� ������ ���������� ���� 
// ���������������� ��������� ���������, ������������� � ����� ����� �������� 
// � ��������������� ��� ���������� ��������� �������� ��������� �������.

class mcontr : public scope {
public:
  static void upcontr( matrix&, real par, real sh = 0.0 );
  static void decontr( matrix&, real par, real sh = 0.0 );

private:
  static void transform( matrix&, real a, real b, const line_func& lf, const line_func& cf, const line_func& rf );
};

}; // namespace lwml

#endif // _MCONTR_
