// ����� �������� �������
// lwml, (c) ltwood

#ifndef _GOLDSECT_
#define _GOLDSECT_

#include "defs.h"
#include "mdefs.h"
#include "i_func.h"

/*#lake:stop*/

namespace lwml {

// ����� gminf ��������� ����� �������� �������
// �� ������� [a,b] ������� �������� �������
// ������� calc() ���������� �������� ���������,
// ��������������� ������� �� ������� [a,b]
// ����� �������� ����� ������� � �������������
// ������������ eps
// ���������� ����� ������ �������������� ������
// ����������� ������� func()

class gminf : public scope {
public:
  static real calc( const i_function&, real a, real b, real eps = STDEPS );
};

}; // namespace lwml

#endif // _GOLDSECT_
