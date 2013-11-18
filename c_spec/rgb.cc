#include "rgb.h"
#include "mdefs.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

const real RED_GRAY_WEIGHT   = 0.299;
const real GREEN_GRAY_WEIGHT = 0.587;
const real BLUE_GRAY_WEIGHT  = 0.114;

real rgbcolor::gray() const
{
  return RED_GRAY_WEIGHT * _red +
         GREEN_GRAY_WEIGHT * _green +
         BLUE_GRAY_WEIGHT * _blue;
}

real hsbcolor::gray() const
{
  return color_conv::hsb2rgb(*this).gray();
}

// color models converting

rgbcolor color_conv::hsb2rgb( const hsbcolor& hsb )
{
  real h = 360.0 * hsb.hue(); // important: hue as degree
  real s = hsb.saturation();
  real v = hsb.brightness();

  h = fmod(h, 360.0);
  h /= 60.0;
  int i = int_cast(h);
  real f = h - i;  // f is the fractional part of h
  real j = v * (1.0 - s);
  real k = v * (1.0 - s * f);
  real l = v * (1.0 - s * (1.0 - f));

  real r = 0.0, g = 0.0, b = 0.0; // has no mean!
  switch( i ){
    case 0: r = v; g = l; b = j; break;
    case 1: r = k; g = v; b = j; break;
    case 2: r = j; g = v; b = l; break;
    case 3: r = j; g = k; b = v; break;
    case 4: r = l; g = j; b = v; break;
    case 5: r = v; g = j; b = k; break;
    default:
      fail_unexpected();
  }

  return rgbcolor(r, g, b);
}

const real UNDEFINED_COLOR = 0.0;

hsbcolor color_conv::rgb2hsb( const rgbcolor& rgb )
{
  real h, s, v;

  real r = rgb.red();
  real g = rgb.green();
  real b = rgb.blue();

  // ����������� "��������"2
  real max = t_max<real>(r, g, b);
  v = max;

  // ����������� ������������
  real min = t_min<real>(r, g, b);
  if( v == 0.0 ) s = 0.0;
    else s = (v - min) / v;

  // ����������� ��������� ����
  if( s == 0 )
    h = UNDEFINED_COLOR;
  else{
    real delta = max - min;
    real cr = (max - r) / delta;
    real cg = (max - g) / delta;
    real cb = (max - b) / delta;

    h = 0; // has no mean!
    if( r == max ) 
      h = cb - cg;            // ���� ����� ������ � ���������
    else if( g == max ) 
      h = 2.0 + cr - cb; // ���� ����� ������� � ������
    else if( b == max ) 
      h = 4.0 + cg - cr; // ���� ����� ��������� � �������
    else 
      fail_unexpected();

    h *= 60.0;
    if( h < 0 ) h += 360.0; // ���������� � ������������� ���������
  }

  return hsbcolor(h / 360.0, s, v); // important: hsbcolor.hue as real
}

}; // namespace lwml
