// ������� ���� f: R^n -> R.
// lwml, (c) ltwood

#ifndef _I_VFUNC_
#define _I_VFUNC_

#include "defs.h"
#include "vector.h"

/*#lake:stop*/

namespace lwml {

// ����������� ������ �������-������� ���� f: R^n -> R.
// ������� �������������� ������ ����������� ������� func(),
// ����������� ������ � ������������ ������������ �����.

class i_vfunction : public interface {
public:
  // ���������������� �������.
  virtual real func( const vector& ) const = 0;

  // �������������� �������� ���������� ������� � �������.
  real operator()( const vector& x ) const {
    return func(x);
  }
};

}; // namespace lwml

#endif // _I_VFUNC_
