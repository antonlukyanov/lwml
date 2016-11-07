#include "lwml/psgraph/minicvwrap.h"

/*#lake:stop*/

namespace lwml {

void minicvwrap::set_hsb( hsbcolor hsbcol )
{
  rgbcolor rgbcol = color_conv::hsb2rgb(hsbcol);
  _mcv->set_rgb(rgbcol.red(), rgbcol.green(), rgbcol.blue());
}

void minicvwrap::puts( real x, real y, const char* str, const align& algn, real alpha ){
  uint halgn = 0;
  if( algn.is_left() ) halgn = minicv::haLEFT;
  if( algn.is_hcenter() ) halgn = minicv::haCENTER;
  if( algn.is_right() ) halgn = minicv::haRIGHT;

  uint valgn = 0;
  if( algn.is_bot() ) valgn = minicv::vaBOT;
  if( algn.is_base() ) valgn = minicv::vaBASE;
  if( algn.is_vcenter() ) valgn = minicv::vaCENTER;
  if( algn.is_top() ) valgn = minicv::vaTOP;

  _mcv->puts(x, y, str, (valgn<<8) | halgn, alpha);
}

}; // namespace lwml
