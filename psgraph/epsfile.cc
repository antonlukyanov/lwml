#include "epsfile.h"
#include "filename.h"

/*#lake:stop*/

namespace lwml {

const char CREATOR[] = "light wood math library - lwml, (c) ltwood, ver 6.0";
const real COLOREPS = 0.001;
const char EPS_VERSION[] = "version-5.0";

const real MM_PER_INCH   =  25.4;
const real PSPT_PER_INCH =  72.0;

real epsfile::mm2pspt( real mm )
{
  return PSPT_PER_INCH * (mm / MM_PER_INCH);
}

real epsfile::pspt2mm( real pspt )
{
  return (MM_PER_INCH * pspt) / PSPT_PER_INCH;
}

const char SETGRAY[]  =  "G";  // set Grayscale color
const char SETRGB[]   =  "C";  // set rgb Color
const char SETHSB[]   =  "H";  // set Hsb color
const char LINE[]     =  "L";  // Line
const char BOX[]      =  "B";  // filled Box
const char RECT[]     =  "R";  // Rectangle
const char TRIANGLE[] =  "T";  // filled Triangle
const char FACET[]    =  "F";  // Facet
const char PIXEL[]    =  "P";  // Pixel
const char MARKER[]   =  "M";  // Marker

void epsfile::header( referer<stream> file, real w, real h )
{
  file->printf("%%!PS-Adobe-2.0 EPSF-2.0\n");
  file->printf("%%%%Creator: %s\n", CREATOR);
  file->printf("%%%%Pages: 0 0\n");
  file->printf("%%%%BoundingBox: 0.0 0.0 %.3f %.3f\n", w, h);
  file->printf("%%%%EndComments\n");
  file->printf("0 0 translate\n");
  file->printf("%%lwml:eps %s\n", EPS_VERSION);
  file->printf("%%lwml:boundrect %.3f %.3f\n", w, h);
  file->printf("%%lwml:library-begin\n");
  file->printf("/extrectcoord {2 index exch 2 index exch 5 index 1 index} def\n");
  file->printf("/erasetriangle {gsave 1 setgray trianglepath fill grestore} def\n");
  file->printf("/linepath {newpath moveto lineto} def\n");
  file->printf("/trianglepath {newpath moveto lineto lineto closepath} def\n");
  file->printf("/rectpath {extrectcoord newpath moveto lineto lineto lineto closepath} def\n");
  file->printf("/x%s {} def /%s {setgray} def\n", SETGRAY, SETGRAY);
  file->printf("/x%s {} def /%s {setrgbcolor} def\n", SETRGB, SETRGB);

  file->printf("/x%s {} def /%s {sethsbcolor} def\n", SETHSB, SETHSB);
  file->printf("/x%s {} def /%s {linepath stroke} def\n", LINE, LINE);
  file->printf("/x%s {} def /%s {4 copy rectpath stroke rectpath fill} def\n", BOX, BOX);
  file->printf("/x%s {} def /%s {rectpath stroke} def\n", RECT, RECT);
  file->printf("/x%s {} def /%s {6 copy trianglepath stroke trianglepath fill} def\n", TRIANGLE, TRIANGLE);
  file->printf("/x%s {} def /%s {6 copy erasetriangle trianglepath stroke} def\n", FACET, FACET);
  file->printf("/x%s {} def /%s {pixelsize 0 360 newpath arc fill} def\n", PIXEL, PIXEL);
  file->printf("/x%s {} def /%s {markersize 0 360 newpath arc stroke} def\n", MARKER, MARKER);
  file->printf("%%lwml:library-end\n");
}

void epsfile::set_param( const epsfile_param& par )
{
  _file->printf("%%lwml:param-begin\n");
  _file->printf("%.3f setlinewidth\n", mm2pspt(par.linewidth()));
  _file->printf("/pixelsize {%.3f} def\n", mm2pspt(par.pixelsize()));
  _file->printf("/markersize {%.3f} def\n", mm2pspt(par.markersize()));
  _file->printf("%%lwml:param-end\n");
  _fntheight = par.fontheight();
}

epsfile::epsfile( const char *name, real width, real height, int pr )
  : _file(stream::create(name, fmWRITE, fmTEXT))
{
  _prec = pr;
  _lx = width;
  _ly = height;
  header(_file, mm2pspt(width), mm2pspt(height));
  set_param(epsfile_param());
  _file->printf("%%lwml:body-begin\n");
  _colortype.set_gray();
  _gray = 0.0;
}

epsfile::~epsfile()
{
  _file->printf("%%lwml:body-end\n");
  _file->printf("showpage\n");
  _file->printf("%%%%Trailer\n");
}

void epsfile::pixel( real x, real y )
{
  _file->printf("x%s %.*f %.*f %s\n", PIXEL, _prec, mm2pspt(x), _prec, mm2pspt(y), PIXEL);
}

void epsfile::marker( real x, real y )
{
  _file->printf("x%s %.*f %.*f %s\n", MARKER, _prec, mm2pspt(x), _prec, mm2pspt(y), MARKER);
}

void epsfile::line( real x1, real y1, real x2, real y2 )
{
  _file->printf("x%s %.*f %.*f %.*f %.*f %s\n", LINE,
    _prec, mm2pspt(x1), _prec, mm2pspt(y1), _prec, mm2pspt(x2), _prec, mm2pspt(y2), LINE);
}

void epsfile::box( real x1, real y1, real x2, real y2 )
{
  _file->printf("x%s %.*f %.*f %.*f %.*f %s\n", BOX,
    _prec, mm2pspt(x1), _prec, mm2pspt(y1), _prec, mm2pspt(x2), _prec, mm2pspt(y2), BOX);
}

void epsfile::rectangle( real x1, real y1, real x2, real y2 )
{
  _file->printf("x%s %.*f %.*f %.*f %.*f %s\n", RECT,
    _prec, mm2pspt(x1), _prec, mm2pspt(y1), _prec, mm2pspt(x2), _prec, mm2pspt(y2), RECT);
}

void epsfile::triangle(
  real x1, real y1, real x2, real y2, real x3, real y3
)
{
  _file->printf("x%s %.*f %.*f %.*f %.*f %.*f %.*f %s\n", TRIANGLE,
    _prec, mm2pspt(x1), _prec, mm2pspt(y1),
    _prec, mm2pspt(x2), _prec, mm2pspt(y2),
    _prec, mm2pspt(x3), _prec, mm2pspt(y3), TRIANGLE);
}

void epsfile::facet(
  real x1, real y1, real x2, real y2, real x3, real y3
)
{
  _file->printf("x%s %.*f %.*f %.*f %.*f %.*f %.*f %s\n", FACET,
    _prec, mm2pspt(x1), _prec, mm2pspt(y1),
    _prec, mm2pspt(x2), _prec, mm2pspt(y2),
    _prec, mm2pspt(x3), _prec, mm2pspt(y3), FACET);
}

void epsfile::set_gray( real graycol )
{
  if( _colortype.is_gray() && fabs(_gray - graycol) < COLOREPS )
    return;

  _file->printf("x%s %.3f %s\n", SETGRAY, graycol, SETGRAY);

  _colortype.set_gray();
  _gray = graycol;
}

void epsfile::set_rgb( rgbcolor rgbcol )
{
  if( _colortype.is_rgb() &&
      fabs(_rgb.red() - rgbcol.red()) < COLOREPS &&
      fabs(_rgb.green() - rgbcol.green()) < COLOREPS &&
      fabs(_rgb.blue() - rgbcol.blue()) < COLOREPS )
    return;

  _file->printf("x%s %.3f %.3f %.3f %s\n", SETRGB, rgbcol.red(), rgbcol.green(), rgbcol.blue(), SETRGB);

  _colortype.set_rgb();
  _rgb = rgbcol;
}

void epsfile::set_hsb( hsbcolor hsbcol )
{
  if( _colortype.is_hsb() &&
      fabs(_hsb.hue() - hsbcol.hue()) < COLOREPS &&
      fabs(_hsb.saturation() - hsbcol.saturation()) < COLOREPS &&
      fabs(_hsb.brightness() - hsbcol.brightness()) < COLOREPS )
    return;

  _file->printf("x%s %.3f %.3f %.3f %s\n", SETHSB, hsbcol.hue(), hsbcol.saturation(), hsbcol.brightness(), SETHSB);

  _colortype.set_hsb();
  _hsb = hsbcol;
}

void epsfile::puts( real x, real y, const char* str, const align& algn, real alpha )
{
  _fnt.puts(referer<i_canvas>(this), x, y, str, _fntheight, algn, alpha);
}

void epsfile::special( real, real, int, const char* cmd )
{
  _file->printf("%s\n", cmd);
}

}; // namespace lwml
