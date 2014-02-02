// trigonometry generator
// lwml, (c) ltwood

#ifndef _TRIG_GEN_
#define _TRIG_GEN_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

// ��������� �������� sin, cos ������������������ ������� �����
// 0, alpha, 1*alpha, ..., n*alpha, ...
// alpha (�������) �������� � ������������ ��� ������� reset().
// � �������� ���������� �������� ���� ������������ 0.
// next() - ���������� ��������� ��������.

class trig_gen {
public:
  trig_gen( real alpha, real start = 0.0 ){
    reset(alpha, start);
  }
  real get_sin() const { return _sin; }
  real get_cos() const { return _cos; }

  void reset( real alpha, real start = 0.0 ){
    _sin_step = sin(alpha);
    _cos_step = cos(alpha);
    _sin = sin(start);
    _cos = cos(start);
  }
  void next(){
    // sin(a+b) = sin(a)cos(b) + cos(a)sin(b)
    // cos(a+b) = cos(a)cos(b) - sin(a)sin(b)
    long_real tmp_sin = _sin*_cos_step + _cos*_sin_step;
    _cos = _cos*_cos_step - _sin*_sin_step;
    _sin = tmp_sin;
  }

private:
  long_real _sin_step, _cos_step;
  long_real _sin, _cos;
};

}; // namespace lwml

#endif // _TRIG_GEN_
