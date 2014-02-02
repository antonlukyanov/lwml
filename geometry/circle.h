// Cricle generator
// lwml, (c) ltwood

#ifndef _CIRCLE_
#define _CIRCLE_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

/* ����� �.�., �������� �.�. ������������ �������. ������������� ������.
   �������� ��������� ����������.
   �� ���� �������� ������� ���� �������������� ����������������
   ����� ���������� �� ����� (cx+r, cy) ������ ������� �������.
   ��� ���� ����� �� ���� � ������������ ����� ���������� ������.
*/

class i_circle_pixel_map : public interface {
public:
  virtual void pixel( int x, int y ) = 0;
};

class circlegen : public scope {
public:
  static void circle( i_circle_pixel_map& pm, int cx, int cy, int r );
};

}; // namespace lwml

#endif // _CIRCLE_
