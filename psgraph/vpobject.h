// ����������� ��������� ��������
// lwml, (c) ltwood

#ifndef _VPOBJECT_
#define _VPOBJECT_

//!! TODO: ������ ���������� ��������� � �������������
//!! TODO: ��������� ������ � ���������� �������

#include "defs.h"
#include "viewport.h"
#include "vector.h"
#include "matrix.h"
#include "histogram.h"
#include "geom3d.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// ������ ���������� ��� ��������������� �������� �����,
// � ������� ���������� ���������
// �� ���� ����� ��������� ����������� �����, ������� ���
// ��������� �������
// ��������� ����� ����� ��������, ������ ����� vp()
// ��������� � �����, ���������� ����� ������� ������������
// ������� ������ �� ���������� �����������, ������� ���
// �������� ������������

// ����� viewgrid ��������� ��������� � ����� ��������� ���������,
// ��������� �� ny ����� � nx �������� ������� ������������� �����.
// �������� �� ��������� �� ��������� ���������� ������.
// �������� ������� ����� ���������� ��� ��������������� ������� dir.
// ������ ������ ����� ���� ���������� ��� ������ ��� ������ ������.
// ������� ���� ��������� ����� ���� ���������� ��������� set_XXX().
// ����� ����� ���������� ����� ����� �� �������� �����.
// ��������� viewport ����� ���������� (0, nx), (0, ny).
// ������ �������� ������ � �������������� ���������.

class viewgrid : public value {
public:
  enum direction { NORMAL, REVERSE };

  // ��������� ��������� �� ny ����� � nx �������� �����
  viewgrid( referer<i_canvas> vp, int nx, int ny, direction dir = NORMAL );

  // ���������� ������� ����
  void set_gray( real col )     { _vp->set_gray(col); }
  void set_rgb( rgbcolor col )  { _vp->set_rgb(col); }
  void set_hsb( hsbcolor col )  { _vp->set_hsb(col); }

  // ���������� ������ � ������ y � ������� x
  void box( int x, int y );   // � ��������
  void rect( int x, int y );  // ��� ������

  // ���������� ����� ����� �� �������� �����
  void show_grid( real dens = 0.75 );

  // �������� ����� ��������� ���������
  referer<viewport> vp() { return _vp; }

private:
  referer<viewport> _vp;
  bool _revers;
  int _nx, _ny;
};

// ����� vector_as_line ������������ ������ � ���� �������
// ��������� viewport ����� ���������� (0, x.len()-1), (x.min(), x.max())

class vector_as_line : public value {
public:
  // ���������� ������ � �����
  vector_as_line( referer<i_canvas> cv, const vector& x );

  // �������� ����� �������
  referer<viewport> vp() { return _vp; }

private:
  referer<viewport> _vp;

  real correct_min( const vector& );
  real correct_max( const vector& );

  void draw( const vector& );
};

// ����� colorer - ��������������� ��� ������ matrix_as_grayscale.
// �� ������ ��� �������������� �������� �������� ������� � ����.
// �������� operator() �������� �������� �������� �������,
// ��������������� �� �������� [0,1] � ����������� ��� � ���� � ������ HSB.

class i_colorer : public interface {
public:
  // �������������� �������� � ����
  virtual hsbcolor operator()( real val ) const = 0;
};

// ���������� colorer, ������������� �������� � ����.
// ��� ��������������� �������� ��� � ������������ ����������� �� ������ �����,
// � ��� ������� �������� ��������, � ������� ������� ������������� ������� ��������.
// ���� revers ��������� �������� �������: ���� ���� ���� �������,
// �� ������� ��������� ������������� ������ �����.
// ��������� ������� mk_color() ��������� ������ ����������� ���� � ������������
// �� ����������� �������� �������� �������.

const real std_simple_colorer_blackcolor = 0.1;
const real std_simple_colorer_whitecolor = 0.9;

const real COL_POW = 0.5;

class simple_colorer : public i_colorer, public value {
public:
  enum color_mode { POSITIVE, NEGATIVE };

  // ��� ����� �������� ���, ������������ � �������� ��������� �������
  // ������������� �������� ���� �������������� ��������
  simple_colorer(
    real hue = 0.0,
    real satur = 0.0,
    real bc = std_simple_colorer_blackcolor,
    real wc = std_simple_colorer_whitecolor,
    color_mode cm = POSITIVE
  ){
    _hue = hue;
    _satur = satur;
    _bc = bc;
    _wc = wc;
    _revers = (cm == NEGATIVE);
  }

  virtual hsbcolor operator()( real val ) const {
    real cb = pow(val, COL_POW);
    hsbcolor res = mk_color(val);
    if( _revers )
      res.set_brightness(scale::coord(cb, 0.0, 1.0, _wc, _bc));
    else
      res.set_brightness(scale::coord(cb, 0.0, 1.0, _bc, _wc));
    return res;
  }

  virtual hsbcolor mk_color( real val ) const {
    return hsbcolor(_hue, _satur, 0.0);
  }

private:
  real _hue, _satur;
  real _bc, _wc;
  bool _revers;
};

// ����� matrix_as_grayscale ������������ ������� � ����
// �������� � ��������� ������.
// ������ ������������� �������������, ������� - �����������.
// �������� �� ��������� �� ��������� ���������� ������.
// �������� ������� ����� ���������� ��� ��������������� ������� dir.
// ��� ��������������� ����� ������� ������������� ������� �����������
// �������� � ����, ������� ���������� ������ colorer.
// ��������� ����� �������� ����� �������, ������� ������� ��������-���������,
// ����� �������� ���������� ����� ���������� ��������� �����.
// ��������� viewport ����� ���������� (0, m.col()), (0, m.str()).
// ��������� ������� ������������� ������ � �������������� ���������.

class matrix_as_grayscale : public value {
public:
  enum direction { NORMAL, REVERSE };

  // ���������� ������� m � ��������� ������
  matrix_as_grayscale(
    referer<i_canvas> cv, const matrix& m, 
    direction dir = NORMAL,
    const i_colorer& col = simple_colorer(), 
    tick_mode tick = tmOFF
  );

  // �������� ����� ��������
  referer<viewport> vp() { return _vp; }

private:
  referer<viewport> _vp;

  void draw( const matrix&, bool revers, const i_colorer&, tick_mode tick );
};

// ����� matrix_as_constline ������ ����� ������ �������,
// �������� ���������, �������������� � �������.
// ������ ������������� �������������, ������� - �����������.
// �������� �� ��������� �� ��������� ���������� ������.
// �������� ������� ����� ���������� ��� ��������������� ������� dir.
// ��� ��������������� ����������� ������ ������� �������.
// ���� ������� ���������� ������� ��������� ����� ������.
// ��� ��������������� ����� ����� �������� ������������� �����������,
// ������������ ��� ������������� ����������� (��������������) ������.
// ��������� ����� ������ ����� �������� ����� �������, �������
// ������� ��������-���������, ����� �������� ����������
// ����� ���������� ��������� �����.
// ����� ���������� ����� ����� �� �������� �����.
// ��������� viewport ����� ���������� (0, m.col()-1), (0, m.str()-1).
// ��������� ������� ������������� _�����_ � �������������� ���������.

const real std_matrix_as_constline_eps = 1e-6;

class matrix_as_constline : public value {
public:
  enum direction { NORMAL, REVERSE };

  // ����������� ��������� ����� ������
  matrix_as_constline(
    referer<i_canvas> cv, int str, int col,
    direction dir = NORMAL,
    real eps = std_matrix_as_constline_eps
  );

  // ���������� ����� ������ �� ������ c
  void draw( const matrix& m, real c, tick_mode tick = tmOFF );

  // ���������� ����� ����� �� �������� �����
  void show_grid( real dens = 0.75 );

  // �������� ����� ��������
  referer<viewport> vp() { return _vp; }

private:
  referer<viewport> _vp;
  int _str, _col;
  bool _revers;
  real _eps;

  void cline( fpoint3d p1, fpoint3d p2, real c );
  void trcline( fpoint3d p1, fpoint3d p2, fpoint3d p3, real c );
};

}; // namespace lwml

#endif // _VPOBJECT_
