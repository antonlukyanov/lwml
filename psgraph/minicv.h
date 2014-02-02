// External mini-canvas for stupid GUI applications
// lwml, (c) ltwood

#ifndef _MINICV_
#define _MINICV_

//!! NB: �����������:
// ���� ��������� �� �������� ��������� defs.h, mdefs.h ������ �� ������������ ������������� 
// ��� ����������� ����������� ������������� ����� ��������� �������� �� ��������� lwml.

/*#lake:stop*/

//!! NB: �����������:
// ����� ����������� ������������ ���� lwml ������ �� ������������ ������������� ���
// ����������� ����������� ������������� ����� ��������� �������� �� ��������� lwml.

#define HALIGN(al) ((al) & 0x00ff)
#define VALIGN(al) (((al) & 0xff00)>>8)

//!! NB: �����������:
// ���� ����� �� ����������� �� ������ interface ������ �� ������������ ������������� ���
// ����������� ����������� ������������� ����� ��������� �������� �� ��������� lwml.

class minicv {
public:
  enum {
    haLEFT = 0, haCENTER = 1, haRIGHT = 2,
    vaBOT = 0, vaBASE = 1, vaCENTER = 2, vaTOP = 3
  };

  virtual ~minicv() {}

  // �������� ������������� ���������� �����,
  // � ������� ���������� ���������
  virtual void boundrect( double& ax, double& bx, double& ay, double& by ) const = 0;

  // �������� ������� RLU (see canvas.h) � ����������� �����
  virtual double xunit() const = 0;
  virtual double yunit() const = 0;

  // ���������� ������
  // ��� ��������� ��������, �� �������������� ������� �������,
  // ������� ����� ���� ����������� ����� ��������� "���������"
  // ���������� ��� �����.
  virtual void pixel( double x, double y ) = 0;

  // ���������� "������" - �������������� ������� �����
  // ��� � ������ ������� ������� �� ���������.
  virtual void marker( double x, double y ) = 0;

  // ���������� �����
  virtual void line( double x1, double y1, double x2, double y2 ) = 0;

  // ���������� ����
  // ���� �������� ��� ������� �������������.
  virtual void box( double x1, double y1, double x2, double y2 ) = 0;

  // ���������� �������������
  // ������������� �������� ��� ������
  virtual void rectangle( double x1, double y1, double x2, double y2 ) = 0;

  // ���������� �����������
  // ����������� �������� � �������� ������� ������
  virtual void triangle(
    double x1, double y1, double x2, double y2, double x3, double y3
  ) = 0;

  // ���������� "�����" (facet)
  // "�����" �������� ��� ����������� � �������� ������ ���� (�����)
  virtual void facet(
    double x1, double y1, double x2, double y2, double x3, double y3
  ) = 0;

  // � ��������� �������� ������������� �������� ��������� ������������
  // ������������� ������� �� ��������� [0,1]

  // ��������� �������� ����� � �������� ����� grayscale
  virtual void set_gray( double graycol ) = 0;
  // ��������� �������� ����� � �������� ����� RGB
  virtual void set_rgb( double r, double g, double b ) = 0;

  // ����� ������ � �������� ������� � �������� �������������
  // algn - ����� ������������
  // alpha - ����, ������������ ����������� ������ ������
  virtual void puts(
    double x, double y, const char* str, unsigned int algn, double alpha
  ) = 0;

  // �������� ���������� ������������� ��� ���� �������
  // id - ������������� ������������� �������
  // cmd - ��������� �������
  virtual void special( double x, double y, int id, const char* cmd ) = 0;
};

#endif // _MINICV_
