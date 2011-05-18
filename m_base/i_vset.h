// ����� ��������
// lwml, (c) ltwood

#ifndef _I_VSET_
#define _I_VSET_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"

/*#lake:stop*/

namespace lwml {

// ��������� �������, ��������� ����� ��������.
// ������ ������������ � ���������� �������������.
// ������� �������������� ��������� ����������� �������:
//   len() - ���������� ����� ��������
//   dim() - ���������� ����������� ��������
//   get() - ���������� d-� ���������� ������� � ������� j
//   get( vector& ) - �������� � ���������� �������� ������ � ������� j

class i_vector_set  : public interface {
public:
  virtual int len() const = 0;
  virtual int dim() const = 0;
  virtual real get( int j, int d ) const = 0;
  virtual void get( vector&, int j ) const = 0;

  real operator()( int j, int d ) const { return get(j, d); }
};

}; // namespace lwml

#endif // _I_VSET_
