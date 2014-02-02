#ifndef _INTEGRAL_
#define _INTEGRAL_

#include "defs.h"
#include "mdefs.h"
#include "i_func.h"

//!! TODO: ��������� ������ � ���������� �������

/*#lake:stop*/

namespace lwml {

// ���������� ���������
// ������� fixed() ���������� �������� ��������� �� ������� [a,b],
// ������������ ��� ������������� ��������� ������� �� sect ����������
// ������� adapt() ���������� �������� ��������� �� ������� [a,b],
// ����������� � ���������� ����������������� ����������� ��������� �������
// ������� � ��������� �� sect ����������
// ������ �� ������������ ������ � ������������� ��������� eps

const int INTEGRAL_STDSECT = 32;

class integral : public scope {
public:
  static real fixed( const i_function&, real a, real b, int sect = INTEGRAL_STDSECT );
  static real adapt( const i_function&, real a, real b, int sect = INTEGRAL_STDSECT, real eps = STDEPS );
};

}; // namespace lwml

#endif // _INTEGRAL_

