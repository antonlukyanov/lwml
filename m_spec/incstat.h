// Simple incremental statistics
// lwml, (c) ltwood

#ifndef _INCSTAT_
#define _INCSTAT_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

// Класс для инкрементного вычисления среднего и дисперсии.
// Используется схема вычислений, минимизирующая накопление погрешности.
// Схема чувствительна к флагу '-ffast-math' компилятора gcc.

class inc_stat : public value {
public:
  inc_stat(){
    _mid = 0.0;
    _mid2 = 0.0;
    _num = 0;
  }

  void put( real x ){
    _mid = (_mid * _num + x) / (_num + 1);
    _mid2 = (_mid2 * _num + x*x) / (_num + 1);
    ++_num;
  }

  real mid() const { return _mid; }

  real disp() const {
    return _mid2 - fsqr(_mid);
  }

private:
  int _num;
  real _mid, _mid2;
};

}; // namespace lwml

#endif // _INCSTAT_
