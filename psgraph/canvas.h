// ����������� ����� ��� ��������� ������ �������
// lwml, (c) ltwood

#ifndef _CANVAS_
#define _CANVAS_

#include "defs.h"
#include "geom.h"
#include "rgb.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// ��� ������������ �������� �����

class color_scheme : public value {
  enum { csGRAY = 0, csRGB = 1, csHSB = 2 };
public:
  // ��������� ������������ �������� �����
  // ������������ ������ ��������� ����� grayscale.
  color_scheme() : _type(csGRAY) {}

  // ��������� �������� ����� grayscale (�������� ������)
  void set_gray() { _type = csGRAY; }

  // ��������� �������� ����� RGB
  void set_rgb()  { _type = csRGB; }

  // ��������� �������� ����� HSB/HSV
  void set_hsb()  { _type = csHSB; }

  // ���������, �������� �� grayscale ������� �������� ������
  bool is_gray() const { return _type == csGRAY; }

  // ���������, �������� �� RGB ������� �������� ������
  bool is_rgb()  const { return _type == csRGB; }

  // ���������, �������� �� HSB/HSV ������� �������� ������
  bool is_hsb()  const { return _type == csHSB; }

private:
  char _type;
};

// ���� ������������ ������
// ������������ ���������� ������������ ����� �������� �� ��������� � ������.
// ��� ������� ������������ ������ �������� ���������� ����� ��������.
// �������� ������������ align().left().top() ���������, ��� ������ �������� �����
// ��������� ����� ������� ���� ��������������, ����������� ������,
// � ���� ������ ����� ����������� _������_ _�����_ �� ����� ��������.

class align : public value {
  enum horz_align { haLEFT = 0, haCENTER = 1, haRIGHT = 2 };
  enum vert_align { vaBOT = 0, vaBASE = 1, vaCENTER = 2, vaTOP = 3 };
public:
  // ���������� ������������ ������������
  // ������������ ��������� ������������, ���������������
  // ������������ ����� �������� ����� �����.
  align() : _horz(haLEFT), _vert(vaBOT) {}

  // ����������� ����� �������� � ������ ����
  align& left()    { _horz = haLEFT; return *this; }
  // ����������� ����� �������� � ������ �� �����������
  align& hcenter() { _horz = haCENTER; return *this; }
  // ����������� ����� �������� � ������� ����
  align& right()   { _horz = haRIGHT; return *this; }
  // ����������� ����� �������� � ������� ����
  align& bot()     { _vert = vaBOT; return *this; }
  // ����������� ����� �������� � ������� �����
  align& base()    { _vert = vaBASE; return *this; }
  // ����������� ����� �������� � ������ �� ���������
  align& vcenter() { _vert = vaCENTER; return *this; }
  // ����������� ����� �������� � �������� ����
  align& top()     { _vert = vaTOP; return *this; }

  // ���������, ����������� �� ����� �������� �����
  bool is_left()    const { return _horz == haLEFT; }
  // ���������, ����������� �� ����� �������� � ������ �� �����������
  bool is_hcenter() const { return _horz == haCENTER; }
  // ���������, ����������� �� ����� �������� ������
  bool is_right()   const { return _horz == haRIGHT; }
  // ���������, ����������� �� ����� �������� �����
  bool is_bot()     const { return _vert == vaBOT; }
  // ���������, ����������� �� ����� �������� �� ������� �����
  bool is_base()    const { return _vert == vaBASE; }
  // ���������, ����������� �� ����� �������� � ������ �� ���������
  bool is_vcenter() const { return _vert == vaCENTER; }
  // ���������, ����������� �� ����� �������� ������
  bool is_top()     const { return _vert == vaTOP; }

private:
  char _horz, _vert;
};

// ����������� ����� ��� ��������� ������ �������.
// ���������� �������������� ��� ������ ����������� ������� ���������.
//
// ��������� ���������� � ����������� ������������ �����������.
// ������� �������������� ����� ������������ �����������.
// ��������� ��� ������ �� ������� ����� ������������ �����������.
//
// ������������ ����������� ���������������� ����� ������ -
// ���������� �� ���� ������� line() ������� "���������������" �������
// � ���������� reset() � point().
// point() ��������� �������� ������� ���������,
// ������ ��� ��������� ������ ����� ���� ������ ����������� � ��� �����,
// � ��� ��������� ����������� ����� ���������� ��������� �������.
// reset() ���������� ���� ����������� ������� � �������� � ���������
// ����� �����.
//
// - ������ ������ ������������ ����������� � ������������ ���
// "������������� ������� �����" - "relative length unit" - RLU.
// - ����������� � ����� ������������ ��� ��������� ������������.
// - ������� ��������� �������� ����� ������ �� ��� �������� ���������,
// �� ����������� �������� ������� ������������ ����� (facet).
//
// ������� special() ��������� �������� ���������� ����������� ��� ����
// �������, ��������� �� ���������, ������ ����� (������������� �������)
// � ������ (char*) � ����������� �������.

class i_canvas : public interface, public refcount {
public:
  // ������������� ��������� "����������" �������.
  i_canvas() { reset(); }

  // �������� ������������� ���������� �����,
  // � ������� ���������� ���������
  virtual frect boundrect() const = 0;

  // �������� x-������ RLU � ����������� �����
  virtual real xunit() const = 0;

  // �������� y-������ RLU � ����������� �����
  virtual real yunit() const = 0;

  // ���������� ������
  // ��� ��������� ��������, �� �������������� ������� �������,
  // ������� ����� ���� ����������� ����� ��������� "���������"
  // ���������� ��� �����.
  virtual void pixel( real x, real y ) = 0;

  // ���������� "������" - �������������� ������� �����
  // ��� � ������ ������� ������� �� ���������.
  virtual void marker( real x, real y ) = 0;

  // ���������� �����
  virtual void line( real x1, real y1, real x2, real y2 ) = 0;

  // ���������� ����
  // ���� �������� ��� ������� �������������.
  virtual void box( real x1, real y1, real x2, real y2 ) = 0;

  // ���������� �������������
  // ������������� �������� ��� ������
  virtual void rectangle( real x1, real y1, real x2, real y2 ) = 0;

  // ���������� �����������
  // ����������� �������� � �������� ������� ������
  virtual void triangle(
    real x1, real y1, real x2, real y2, real x3, real y3
  ) = 0;

  // ���������� "�����" (facet)
  // "�����" �������� ��� ����������� � �������� ������ ���� (�����)
  virtual void facet(
    real x1, real y1, real x2, real y2, real x3, real y3
  ) = 0;

  // ��������� �������� ����� � �������� ����� grayscale
  virtual void set_gray( real graycol ) = 0;
  // ��������� �������� ����� � �������� ����� RGB
  virtual void set_rgb( rgbcolor rgbcol ) = 0;
  // ��������� �������� ����� � �������� ����� HSB
  virtual void set_hsb( hsbcolor hsbcol ) = 0;

  // ����� ������ � �������� ������� � �������� �������������
  // algn - ����� ������������
  // alpha - ����, ������������ ����������� ������ ������
  virtual void puts(
    real x, real y, const char* str, const align& algn, real alpha
  ) = 0;

  // �������� ���������� ������������� ��� ���� �������
  // id - ������������� ������������� �������
  // cmd - ��������� �������
  virtual void special( real x, real y, int id, const char* cmd ) = 0;

  // ������ ��������� ����� ������� � "���������������" �������
  void reset();

  // ���������� ��������� ����� ������� � "���������������" �������
  void point( real x, real y );

private:
  bool _newflag;             // moveto/lineto flag for point()
  real _cx, _cy;             // current point for point()
};

}; // namespace lwml

#endif // _CANVAS_
