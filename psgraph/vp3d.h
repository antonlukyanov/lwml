// 3-������ �������
// lwml, (c) ltwood

//!! TODO: ����������

#ifndef _VP3D_
#define _VP3D_

#include "defs.h"
#include "mdefs.h"
#include "viewport.h"
#include "geom.h"
#include "geom3d.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// ����� scene3d ������������ 3-������ �����, ������� ���������
// �������������� �� ���������� ��������� � ���������� �����
// ���������� ���������� ����� ����� �������� ���������,
// ������������ ��� ��������������� �������
// ������� ��������� ���������� ��������� ������������ �������
// ax, bx �.�. �������� ���������� x ���������� �� ax.x() �� bx.x()
// �������� ��������� ��������� ����� ���������� �����������
// ������ ������������� �������
// ���������� ����� ���������� �� 0 �� ��������� ����� vb
// ��� ��������������� ����������� ������� ���������� ��������� (ax, bx)
// � ��������� ����� (vb)

class scene3d : public value {
public:
  scene3d( fpoint3d ax, fpoint3d bx, fpoint3d vb )
    : _ax(ax), _bx(bx), _vb(vb) {}

  // �������� �������������� ���������� ��������� 
  // � ���������� �����
  fpoint3d operator()( fpoint3d ) const;

  // ��������� ������ ��������� ���������
  fpoint3d viewbox() const { return _vb; }  // �����
  fpoint3d ax() const { return _ax; }       // ����������
  fpoint3d bx() const { return _bx; }

private:
  fpoint3d _ax, _bx;
  fpoint3d _vb;
};

inline fpoint3d scene3d::operator()( fpoint3d pnt ) const {
  real sx = scale::coord(pnt.x(), _ax.x(), _bx.x(), 0.0, _vb.x());
  real sy = scale::coord(pnt.y(), _ax.y(), _bx.y(), 0.0, _vb.y());
  real sz = scale::coord(pnt.z(), _ax.z(), _bx.z(), 0.0, _vb.z());
  return fpoint3d(sx, sy, sz);
}

// ����� viewpoint3d ��������� �������������� 3-������ �����
// � ����������� ����� �� �������� ��������� "������"
// ��� ��������������� ����������� ������������ ��������� ������
// ����� ������� ������� - ����������� "�������"
// ����������� ������� �������� ������ right � up
// ���������, ��� ��� X � Y ����������� � �������������� ���������,
// � ��� Z ��������������� �� � ���������� ����������� �����
// ���� right = up = 0, �� ������ ��������� ����� ��� Y
// ���� right ���������� ������� ����� ���������� ������, ���, ���
// ��� ���� ����� �������������� �� ������� ������ ������ ��� Z
// ���� up ���������� ������ ����� ���������� �����
// ��� ���� ���������� � ����� ������� ����
// �������� ����� ����� ��������� "�������" ����� - ����������
// �� ����� �� ��������� ������ � �������������, ��� ���������
// ������ �������� ����� ���� � ���� ����� ����������� �� �������,
// �� ���������� �� ��� �� ������ ������������

class viewpoint3d : public value {
public:
  viewpoint3d( real right, real up ){
    _theta = M_PI_2 * (-1.0 + right);
    _phi = M_PI_2 * (1.0 - up);
    _pv = fpoint3d::polar(_theta, _phi, 1.0);
  }

  // �������� �������������� �� �����
  fpoint operator()( fpoint3d ) const;

  // ���������� "�������" �����
  real depth( fpoint3d ) const;

  // ��������� ������� �������������� (����������� �������)
  fpoint3d prj_vect() const { return _pv; }

private:
  real _theta, _phi;
  fpoint3d _pv;
};

inline fpoint viewpoint3d::operator()( fpoint3d pnt ) const {
  return pnt.project(_theta, _phi);
}

inline real viewpoint3d::depth( fpoint3d pnt ) const {
  return fpoint3d::inner_mul(prj_vect(), pnt);
}

// ����� ��� ��������� 3-������ �������
// ��� ��������������� �������� ������� ����� ���������� i_canvas,
// ����� scene3d ��� �������������� ���������� ��������� �
// ���������� �����, �����-�������� viewpoint3d � ������
// �������� ������ (border) � �������� ������ ������
// ����������� ������� ��������� ���������� ������� ������ i_canvas,
// �� �������� � 3-������� ������������ �����, ����������� � fpoint3d
// ����� ������� ������ ������������ �������
// 1. ������� scene() � viewpoint() ��������� �������� ������ �� ����� � ��������
// 2. ������� prj() ���������� �������������� 3-������ ����� �� ���������
// 3. ������� cv() ��������� �������� ������ �� 2-������ ����� ���������
// 4. ��� ������ ������ ����������� 3-������ �����������, �������
// ������������� �� ��������� ��� ��������� ����������� ������ ������

class viewport3d : public refcount {
private:
  viewport3d( referer<i_canvas>, scene3d sc, viewpoint3d, real border );

public:
  static referer<viewport3d> create( referer<i_canvas> cv, scene3d sc, viewpoint3d vpnt, real border = 1.0 ){
    return referer<viewport3d>(new(lwml_alloc) viewport3d(cv, sc, vpnt, border));
  }

  void set_gray( real g )      { _cv->set_gray(g); }
  void set_rgb( rgbcolor rgb ) { _cv->set_rgb(rgb); }
  void set_hsb( hsbcolor hsb ) { _cv->set_hsb(hsb); }

  void pixel( fpoint3d );
  void marker( fpoint3d );
  void line( fpoint3d, fpoint3d );
  void box( fpoint3d, fpoint3d );
  void rectangle( fpoint3d, fpoint3d );
  void triangle( fpoint3d, fpoint3d, fpoint3d );
  void facet( fpoint3d, fpoint3d, fpoint3d );
  void puts( fpoint3d, const char *str, const align&, fpoint3d );

  void reset();
  void point( fpoint3d );

  const scene3d& scene() const { return _sc; }

  const viewpoint3d& viewpoint() const { return _vp; }

  referer <i_canvas> cv() { return _cv; }

  fpoint prj( fpoint3d ) const;

private:
  scene3d _sc;
  viewpoint3d _vp;
  referer<viewport> _cv;

  frect calcbound( scene3d sc, viewpoint3d vp );
  frect calcrect( referer<i_canvas> cv, real gsize );
};

inline fpoint viewport3d::prj( fpoint3d pnt ) const {
  return _vp(_sc(pnt));
}

}; // namespace lwml

#endif // _VP3D_
