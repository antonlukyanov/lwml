// ��������� ������
// lwml, (c) ltwood

#ifndef _MEDIANFILT_
#define _MEDIANFILT_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"
#include "imatrix.h"

/*#lake:stop*/

namespace lwml {

class medianfilt : public scope {
public:
  enum topology { CIRCLE, LINE };
  
  // ��������� ������.
  // � ������ ������� ������� dst ���������� ������� ���������
  // ������� src, ������������� � ���� � ����������� rank,
  // � ������� � ������� �������.
  // ����������� ����� ���� ���������� 2 * rank + 1 ��������.
  // ���� topology == CIRCLE, �� ������ src �������� ��������� � ������,
  // ����� �� ������ ���������� ���������� �������� ����������
  // ret: true, ���� ���� ���������
  static bool calc( vector& dst, const vector& src, int rank, topology top = LINE );

  // ��������� ���������� ����������� �� ���������� ����������� ������� apt.
  // ������� ������� �������� ������������� ������� �����������
  // �� ���������� ���������� ���������.
  // ������� ���������� ���������� ���������� ��������� �����������.
  static int calc( int_matrix& dst, const int_matrix& src, int apt );

  // ��������� ���������� ����������� �� ������������� �����������.
  // � ��������� �������� ��������� � ���������� ���������� �������.
  static int calc( int_matrix& dst, const int_matrix& src, int apt_x, int apt_y );

  // �������� ����������.
  static void calc_test( int_matrix& dst, const int_matrix& src, int apt );
  static void calc_test( int_matrix& dst, const int_matrix& src, int apt_x, int apt_y );
};

}; // namespace lwml

#endif // _MEDIANFILT_
