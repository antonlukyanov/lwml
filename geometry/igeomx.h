// ����������� ������������� ���������
// lwml, (c) ltwood

#ifndef _IGEOMX_
#define _IGEOMX_

#include "defs.h"
#include "mdefs.h"
#include "igeom.h"

/*#lake:stop*/

namespace lwml {

// ����� ��������� ���������� ������������� ������ ����� �� ������������� ���������.
// ������������ �������������� ������� �.�. "��������� ����������������� �����������"
// (Digital Differential Analyzer, DDA).
// �� ������ ���� �������� ����������� ��������� �������� �����
// �������� ������ ���������� ����������.
//
// ���������� ��� ���������� ���������� ������ i_pixel_map,
// � ������� ������� pixel(x, y, idx, w) ���������� ��� ������ ����� �������.
// �������� idx ���������� ����� ����� �� �����.
// �������� w ������ ��������� �������� 1.0 ��� ������ �� ������� line().
// ��� ������ �� ������� line_sp() �������� w ��������� �������� �� 0.0 �� 1.0
// � ����������� �� ���� ("�������") ������� � ������������� ������������� ������.
//
// ������� line() ������������ "�������" ����� � ��������� ������������ ������.
// ������� line_sp() ��������� ������������� ����������� ��������� DDA (����� Xiaolin Wu).
// �������� close ���������� ���� �� �������� � ����� �������� �����.
//
// ������� line_sp() �������� ����� ������������:
//  -- ��� �� �����������, ��� ������� pixel() ����� ���������� ������
//     ��� ����� �� ��������������, ��������������� �������.
//  -- ��� �� ����������� ������ ���������� ������ close
//     (���� ���� ���������� ��� ������������).

class i_pixel_map : public interface {
public:
  virtual void pixel( int x, int y, int idx, real w = 1.0 ) = 0;
};

class linegen : public scope {
public:
  enum closure { OPEN_END, CLOSED_END };

  static int  calc_len( real x1, real y1, real x2, real y2, closure cl = OPEN_END );
  static void line( i_pixel_map&, real x1, real y1, real x2, real y2, closure cl = OPEN_END );
  static void line_sp( i_pixel_map& pm, real x1, real y1, real x2, real y2, closure cl = OPEN_END );
};

// ����� ��������� �������� � �������������� �����
// � ������� (x0, y0) � �������� r �� ������������� �������
// ����� �������� ������ ����������� �������
// ������� is_in() ��������� ������ �� �������������� �����
// � ����������� �������������� �� �������
// ������� is_in_in() ��������� �� �� ��������, �� �� ���������
// �������� ���������� ������� �����
// ������� is_in_out() ��������� �� �� ��������, �� ���������
// ��������� ������� ����� ���������
// ������� weight() ���� ���� ������� �������, �������� � ����

class circle : public scope {
public:
  static bool is_in( int x, int y, int x0, int y0, real r );
  static bool is_in_in( int x, int y, int x0, int y0, real r );
  static bool is_in_out( int x, int y, int x0, int y0, real r );
  static real weight( int x, int y, int x0, int y0, real r );

private:
  static void conv( int& x, int& y, int x0, int y0 );
};

}; // namespace lwml

#endif // _IGEOMX_
