// ���������� ������������ ������������ ����������
// �� ���� ����� ������� EPS (Encapsulated PostScript)
// lwml, (c) ltwood

#ifndef _EPSFILE_
#define _EPSFILE_

//!! TODO: �������� ���������� ����� ������ bmp

#include "defs.h"
#include "canvas.h"
#include "stream.h"
#include "font.h"
#include "stdmem.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// ����������� �������� ��� ���������� EPS �������
// ��� ������� ������ � �����������

const int  std_eps_prec = 4;           // �������� ������ ����� � EPS-����

const real std_eps_linewidth  = 0.01;  // ������� �����
const real std_eps_pixelsize  = 0.1;   // ������ �������
const real std_eps_markersize = 0.3;   // ������ �������
const real std_eps_fontheight = 2.5;   // ������ ������

// ����� epsfile_param - ���������������
// �� ������ ������ ��������� ��������� EPS-�����
// ����������� ������������� ������������ ��������
// ������� ��������� ���������� ���������� ������ �� �����,
// ��� ��������� ���������� ����� ������� ��������:
// epsfile_param().set_pixelsize(0.07).set_linewidth(0.04);
// ������� ��������� ���������� ��������� ������ epsfile
// �������� ������������� ���������

class epsfile_param : public value {
public:
  epsfile_param(){
    _linewidth = std_eps_linewidth;
    _pixelsize = std_eps_pixelsize;
    _markersize = std_eps_markersize;
    _fontheight = std_eps_fontheight;
  }

  // ������� ��������� ����������
  // ����� ����������: ������� �����, ������ �������, ������ �������, ������ ������
  epsfile_param& set_linewidth( real linewidth )   { _linewidth = linewidth; return *this; }
  epsfile_param& set_pixelsize( real pixelsize )   { _pixelsize = pixelsize; return *this; }
  epsfile_param& set_markersize( real markersize ) { _markersize = markersize; return *this; }
  epsfile_param& set_fontheight( real fontheight ) { _fontheight = fontheight; return *this; }

  // ������� ��������� ����������
  real linewidth()  const { return _linewidth; }
  real pixelsize()  const { return _pixelsize; }
  real markersize() const { return _markersize; }
  real fontheight() const { return _fontheight; }

private:
  real _linewidth;
  real _pixelsize;
  real _markersize;
  real _fontheight;
};

// ���� ����� ��������� ��������� ����������� ���������, �������� ������� i_canvas
// � ����� canvas.h ����� ����� �������� ����������� �������
// ����� ������� ������� - �������, ������� ����� ������ ��� ������� ������ epsfile
// ��� ��������������� ����������� ��� ����� � ������ � ������ � �����������
// ����� ����� ������� �������� ������ ������������ ����� � EPS-����
// ������� set_param() ��������� ���������� ��������� ����������
// ������� special() ��������� ���������� ������-�������� � ����� ����������

class epsfile : public i_canvas { // public refcount
private:
  epsfile( const char *name, real width, real height, int pr );
  virtual ~epsfile();

public:
  static referer<epsfile> create( const char *name, real width, real height, int pr = std_eps_prec ){
    return referer<epsfile>(new(lwml_alloc) epsfile(name, width, height, pr));
  }

  void set_param( const epsfile_param& );

  virtual frect boundrect() const { return frect(0.0, _lx, 0.0, _ly); }
  virtual real xunit() const { return _fntheight; }
  virtual real yunit() const { return _fntheight; }

  virtual void pixel( real x, real y );
  virtual void marker( real x, real y );

  virtual void line( real x1, real y1, real x2, real y2 );
  virtual void box( real x1, real y1, real x2, real y2 );
  virtual void rectangle( real x1, real y1, real x2, real y2 );

  virtual void triangle( real x1, real y1, real x2, real y2, real x3, real y3 );
  virtual void facet( real x1, real y1, real x2, real y2, real x3, real y3 );

  virtual void set_gray( real graycol );
  virtual void set_rgb( rgbcolor rgbcol );
  virtual void set_hsb( hsbcolor hsbcol );

  virtual void puts( real x, real y, const char* str, const align& algn, real alpha );

  virtual void special( real x, real y, int id, const char* cmd );

private:
  int _prec;   // �������� ������������ ���������
  referer<stream> _file;
  real _lx, _ly;
  real _fntheight;

  font _fnt;

  color_scheme _colortype;
  real _gray;
  rgbcolor _rgb;
  hsbcolor _hsb;

  real mm2pspt( real mm );
  real pspt2mm( real pspt );

  void header( referer<stream> file, real w, real h );
};

}; // namespace lwml

#endif // _EPSFILE_
