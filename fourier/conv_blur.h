// ������� ������� � ������������ �����
// lwml, (c) ltwood

#ifndef _CONV_BLUR_
#define _CONV_BLUR_

#include "defs.h"
#include "matrix.h"
#include "progress.h"
#include "i_func.h"
#include "geom.h"

/*#lake:stop*/

namespace lwml {

// ����� proc(src, func, tick) ����������� ���������� ������� � ����� func.
// �������������: ������� ������ ������� - ��� �����������, �� ���� �����
// ������ ������ ������������� ���� ���������� y �����������.
// tick - ��������� �������������� "��������-����������"

class conv_blur : public value {
public:
  conv_blur( int ly, int lx, int hly = 0, int hlx = 0 );

  void proc(
    const matrix& src, const i_function2& func, tick_mode tick = tmOFF
  );

  int lx() const { return _lx; }
  int ly() const { return _ly; }

  real operator()( int y, int x ) const { return _rx(y, x); }

  void get( matrix& dst ) const;

private:
  int _ly, _lx, _hly, _hlx;
  int _ny2, _nx2;
  matrix _rx, _ix;               // ��� �������������� ����� � ���������
  matrix _rg, _ig;               // ��� �������������� ����

};

// ��������� ������� (sigma) sx, sy �� ���� � ����������
// �� phi ������ ������� ������� �������.
class gauss2d_func : public i_function2, public value {
public:
  gauss2d_func( real sigma_x, real sigma_y, real phi = 0.0 )
    : _sx(sigma_x), _sy(sigma_y), _phi(phi)
  {}

  virtual real func( real x, real y ) const
  {
    fpoint p = fpoint(x, y).rot(-_phi);
    return gauss(p.x() / _sx) * gauss(p.y() / _sy) / (_sx * _sy);
  }

private:
  real _sx, _sy, _phi;
};

class square_gauss_func : public i_function2, public value {
public:
  square_gauss_func( real half_sq, real sigma )
    : _half_sq(half_sq), _sigma(sigma)
  {}

  virtual real func( real x, real y ) const
  {
    return gg(x) * gg(y);
  }

private:
  real _half_sq, _sigma;
  real gg( real t ) const {
    t = abs(t);
    if( t < _half_sq )
      return gauss(0.0) / _sigma;
    return gauss((t-_half_sq) / _sigma) / _sigma;
  }
};

}; // namespace lwml

#endif // _CONV_BLUR_
