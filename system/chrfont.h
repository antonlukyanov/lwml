// �����, ��������������� �� ������� CHR (BGI) ����������� CHR2C.
// ��������������� C-���� ������������� �������.
// lwml, (c) ltwood

#ifndef _CHRFONT_
#define _CHRFONT_

#include "defs.h"

/*#lake:stop*/

namespace lwml {

// ����� �������� ������ CHR-������
// �������������� ������ � ���������� ������ ���������� ����� font.

class chrfont : public scope {
public:
  // ���������, �������������� ����� � ���������� ������.
  // ���������� ������� ������� ������������ �����
  // ������������������ �����.
  typedef struct {
    signed char t; // ��� �����
    signed char x; // x-����������
    signed char y; // y-����������
  } stroke_type;

  static const char name[];        // ��� ������
  static const char uppermargin;   // ������ ������
  static const char baseline;      // ��������� ������� �����
  static const char descender;     // ������� ������

  static const unsigned char width[];   // ������ ��������
  static const signed short  offset[];  // �������� ���������� ������� � ����� �������
  static const stroke_type   stroke[];  // ����� ������� ���������� ��������
};

}; // namespace lwml

#endif // _CHRFONT_
