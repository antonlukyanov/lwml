// ������� ���������������� ������� ������� ��������
// lwml, (c) ltwood

#ifndef _LU3D_
#define _LU3D_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"

/*#lake:stop*/

namespace lwml {

//   ������� ���������������� ������� ������� ��������
//   ld - ������ ������������ (����� = n-1)
//   dg - ������� ��������� (����� = n)
//   ud - ������� ������������ (����� = n-1)
//   dd - ������ ������ ������ (����� = n)

class gj3d : public value {
public:
  gj3d( int sz );

  void calc( vector& x, const vector& ld, const vector& dg, const vector& ud, const vector& dd );

private:
  int _size;
  vector _aa, _bb;
};

}; // namespace lwml

#endif // _LU3D_
