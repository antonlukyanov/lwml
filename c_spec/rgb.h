// �������� �����
// lwml, (c) ltwood

#ifndef _RGB_
#define _RGB_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

// ����� rgbcolor ������������ �������� ����� RGB,
// ������ ������������� �������� ��������� ������������
// ������������� ������� �� ��������� [0,1]
// �������� ��������� �������� �������� ��������� ��������
// ������� �� ����������� ����� �������

class rgbcolor : public value {
public:
  rgbcolor(){
    _red = 0.0;
    _green = 0.0;
    _blue = 0.0;
  }
  rgbcolor( real r, real g, real b ){
    _red = correct(r);
    _green = correct(g);
    _blue = correct(b);
  }

  // ���������� �������� �������� �����������
  void set( real r, real g, real b ){
    _red = correct(r);
    _green = correct(g);
    _blue = correct(b);
  }

  // �������� �������� �������� �����������
  real red() const    { return _red; }
  real green() const  { return _green; }
  real blue() const   { return _blue; }

  // �������� �������� ����������
  void set_red( real r )   { _red = correct(r); }
  void set_green( real g ) { _green = correct(g); }
  void set_blue( real b )  { _blue = correct(b); }

  // �������� ����������� ������� �����
  real gray() const;

private:
  real _red, _green, _blue;

  real correct( real x ) const { return (x < 0) ? 0.0 : ((x > 1.0) ? 1.0 : x); }
};

// ����� hsbcolor ������������ �������� ����� HSB/HSV
// � �������� ������ HSB ��������� ������������ ��������
// ��� (hue), ������������ (saturation) � ������� (brightness)
// ��� �������� ���������� ������������ �������������
// ������� �� ������� [0,1]
// ��� ��������� ���� � �������� �����
// ���� �������� ���� � ��������, �� ������������ ����� ���������
// ��������� ����� � ������� ���������:
//   330-30    - �������,
//   30-90  - ������,
// � ��� ����� �� 60 �� �������, �������, �����, ����������.
// ��������, ��. ������ https://www.development-tutorial.com/how-to-code-color/
// ������������ - ��� ������� ����� (�����-������)
// ������� - ������������� ����� (������-�����)

class hsbcolor : public value {
public:
  hsbcolor(){
    _hue = 0.0;
    _saturation = 0.0;
    _brightness = 0.0;
  }
  hsbcolor( real h, real s, real b ){
    _hue = correct(h);
    _saturation = correct(s);
    _brightness = correct(b);
  }

  // ���������� �������� �������� �����������
  void set( real h, real s, real b ){
    _hue = correct(h);
    _saturation = correct(s);
    _brightness = correct(b);
  }

  // �������� �������� �������� �����������
  real hue()        const { return _hue; }
  real saturation() const { return _saturation; }
  real brightness() const { return _brightness; }

  // �������� �������� ����������
  void set_hue( real h )        { _hue = correct(h); }
  void set_saturation( real s ) { _saturation = correct(s); }
  void set_brightness( real b ) { _brightness = correct(b); }

  // �������� ����������� ������� �����
  real gray() const;

private:
  real _hue, _saturation, _brightness;

  real correct( real x ) const { return (x < 0) ? 0.0 : ((x > 1.0) ? 1.0 : x); }
};

// ����� color_conv �������� ����������� �������-�����,
// ����������� �������������� �������� ����� ���� � �����

class color_conv : public scope {
public:
  // �������������� �� ����� hsb � ����� rgb
  static rgbcolor hsb2rgb( const hsbcolor& );

  // �������������� �� ����� rgb � ����� hsb
  static hsbcolor rgb2hsb( const rgbcolor& );

};

}; // namespace lwml

#endif // _RGB_
