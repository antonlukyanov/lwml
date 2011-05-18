// ���������� ���������� �����
// lwml, (c) ltwood

#ifndef _FLYSUM_
#define _FLYSUM_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"

/*#lake:stop*/

namespace lwml {

// ���������� ���������� �����.
// � ������ ������� ������� dst ���������� ����� ���������
// ������� src, ������������� � ���� � ����������� len2,
// � ������� � ������� �������.
// ����������� ����� ���� ���������� 2 * len2 + 1 ��������.

class flysum : public scope {
public:
  static void calc( vector& dst, const vector& src, int len2 );
};

}; // namespace lwml

#endif // _FLYSUM_
