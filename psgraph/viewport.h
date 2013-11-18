// ���������� ������������ ������������ ����������
// � ���� ����������� ������������ ����������
// lwml, (c) ltwood

#ifndef _VIEWPORT_
#define _VIEWPORT_

#include "defs.h"
#include "mdefs.h"
#include "canvas.h"
#include "stdmem.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// ����� viewport - �.�. ���������� ����������� ����������
// ���������� ���������� ������ ��� �������������� ���������
// ��� ��������� ������� �������� � ����� ������������ �������,
// � ���� ��������� ������ � ��������������, ������� ��
// ������ ���������� ������ i_canvas �� ����� �������� ���������
// ����� viewport ��������� ��������� ������ i_canvas
// � ����� canvas.h ����� ����� �������� ����� ����������
// ����� ������� ������ ����������� ������ viewport
// ��� ��������������� ����� ������� ������ ����� ����������,
// � ������� ����� ����������� ��������� � ������� �����
// ������ ������� viewport
// � ���� ������ ��������� ���� �� ���� ��������� ����� ����������
// ���� ��� ��������������� ����������� �������� dstrect,
// �� ���� �������� ������ ������������� �� ����� ����������,
// � ������� ����� ������������� ���������
// ������� xunit(), yunit() ���� ������� RLU � ����������� viewport
// ������� ������������ ������� ������� viewport �� ����� ���������� (�����)
// ������� set_clip() ������������� ����� ��������� ����� �� �������� viewport
// ��������� �������� ������ ��� ������� line() � point()

class viewport : public i_canvas { // public refcount
protected:
  // ����������� ��� ��������� �� ���� ����� ����� ����������
  viewport( referer<i_canvas>, frect srcrect );

  // ����������� ��� ��������� � �������������� dstrect,
  // ������� �� ����� ����� ����������
  viewport( referer<i_canvas>, frect srcrect, frect dstrect );

public:
  // ����������� ��� ��������� �� ���� ����� ����� ����������
  static referer<viewport> create( referer<i_canvas> pcv, frect srcrect = frect(0, 1, 0, 1) ){
    return referer<viewport>(new(lwml_alloc) viewport(pcv, srcrect));
  }

  // ����������� ��� ��������� � �������������� dstrect,
  // ������� �� ����� ����� ����������
  static referer<viewport> create( referer<i_canvas> pcv, frect srcrect, frect dstrect ){
    return referer<viewport>(new(lwml_alloc) viewport(pcv, srcrect, dstrect));
  }

  virtual frect boundrect() const { return _src; }
  virtual real xunit() const;
  virtual real yunit() const;

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

  virtual void puts(
    real x, real y, const char* str, const align& algn, real alpha = 0.0
  );

  virtual void special( real x, real y, int id, const char* cmd );

  // ������������ ������� ������� viewport �� ����� ����������
  void show_viewbox();

  // ���������� ����� ��������� ����� �� �������� viewport
  void set_clip( bool state = true ) { _clip_state = state; }

private:
  referer<i_canvas> _cv;  // ����� ��� ���������
  frect _src;             // ������� �������������
  frect _dst;             // ������������� ����������

  color_scheme _colortype;        // ������� ����� ������ �����
  real _gray;                     // ���� ��� ����� GRAY
  rgbcolor _rgb;                  // ���� ��� ����� RGB
  hsbcolor _hsb;                  // ���� ��� ����� HSV

  bool _clip_state;

  void setup();

protected:
  void setcol();

  real X( real x );
  real Y( real y );

  referer<i_canvas> basecv() const { return _cv; }
  frect baserect() const { return _dst; }
};

// ����������� �������� �������� ��� ��������� (��. ����� diagram)
// ��� �������� ���� � ����� RLU

const real std_diagram_top_shift = 0.25;
const real std_diagram_bot_shift = 0.15;
const real std_diagram_xgridsize = 0.35;
const real std_diagram_ygridsize = 0.35;

// ����� diagram_param - ���������������
// �� ������ ������ ��������� ��������� ���������
// ����������� ������������� ������������ ��������
// ������� ��������� ���������� ���������� ������ �� �����,
// ��� ��������� ���������� ����� ������� ��������:
// diagram_param().set_xshift(0.16).set_ygridsize(0.38);
// ������� ��������� ���������� ��������� ������ diagram
// �������� ������������� ���������

class diagram_param : public value {
public:
  diagram_param(){
    _top_shift = std_diagram_top_shift;
    _bot_shift = std_diagram_bot_shift;
    _xgridsize = std_diagram_xgridsize;
    _ygridsize = std_diagram_ygridsize;
  }

  // ������� ��������� ����������
  // ����� ����������: �������, ������� �������
  // ��� ������� ���������� � ����� RLU
  diagram_param& set_top_shift( real top_shift ){
    _top_shift = top_shift; return *this;
  }
  diagram_param& set_bot_shift( real bot_shift ){
    _bot_shift = bot_shift; return *this;
  }
  diagram_param& set_xgridsize( real xgridsize ){
    _xgridsize = xgridsize; return *this;
  }
  diagram_param& set_ygridsize( real ygridsize ){
    _ygridsize = ygridsize; return *this;
  }

  // ������� ��������� ����������
  real top_shift() const { return _top_shift; }
  real bot_shift() const { return _bot_shift; }
  real xgridsize() const { return _xgridsize; }
  real ygridsize() const { return _ygridsize; }

private:
  real _top_shift, _bot_shift;
  real _xgridsize, _ygridsize;
};

// diagram - ������������� ���������� ����������� ����������
// ���������� �� ������ viewport ���, ��� ���� �������� �����
// ��� ��������� � ����� ����������, ����� ������ ��� �������� � �������

class diagram : public viewport { // public refcount
private:
  diagram( referer<i_canvas>, frect srcrect, const diagram_param& par );

public:
  static referer<diagram> create( 
    referer<i_canvas> pcv, frect srcrect = frect(0, 1, 0, 1),
    const diagram_param& par = diagram_param()
  ){
    return referer<diagram>(new(lwml_alloc) diagram(pcv, srcrect, par));
  }

  // ��������� ������� � �������� ������� �� ��� X ��� Y
  // � �������������� ��������
  void xgrid( real, const char *subs = 0 );
  void ygrid( real, const char *subs = 0 );

  // ��������� ��������� (������ ������)
  void title( const char* );

  // ��������� ��� X ��� Y � �����
  // ������� ��������� �������� � ���������� �������
  void xsubscript( const char* bl = 0, const char* br = 0 );
  void ysubscript( const char* lb = 0, const char* lt = 0 );

private:
  real _tdx, _tdy;   // ������ ��� �������� � ����������� ����� ����������
  real _bdx, _bdy;   // ������ ��� �������� � ����������� ����� ����������
  real _xgsz, _ygsz; // ������ ����� � ����������� ����� ����������

  frect blow( referer<i_canvas> cv, const diagram_param& );
};

}; // namespace lwml

#endif // _VIEWPORT_
