// ѕростой универсальный таймер
// lwml, (c) ltwood

#ifndef _TIMERS_
#define _TIMERS_

#include "defs.h"
#include "cstrng.h"
#include "lwmlconf.h"

#include "systimer.h"

/*#lake:stop*/

namespace lwml {

class timer : public value {
public:
  // Ќачать отсчет времени
  timer() { restart(); }

  // —бросить таймер в исходное состо€ние и начать новый отсчет времени
  void restart()  { _start = systimer::time(); }

  // ѕолучить врем€, прошедшее с момента последнего начала отсчета
  double left() const { return systimer::time() - _start; }

  // ѕолучить абсолютное текущее системное врем€
  static double time() { return systimer::time(); }

  // ѕолучить разрешение используемого таймера
  static double res() { return systimer::res(); }

  // ѕолучить полное текстовое описание текущего момента времени
  // ¬озвращает строку, содержащую полную временную метку
  // текущего момента времени (дата-врем€).
  static strng timestamp();

private:
  double _start;
};

// “аймер многократного запуска.
//
// “акой таймер можно многократно запускать и останавливать,
// получа€ в любой момент количество завершенных запусков,
// суммарное врем€ на все запуски и среднее врем€ на один запуск.

class mtimer : public value {
public:
  mtimer() : _num(0), _sum(0.0) {}

  void start() { _tm.restart(); }

  // возвращает врем€, прошедшее с момента старта
  double stop() {
    double left = _tm.left();
    _sum += left;
    _num++;
    return left;
  }

  double sum() const { return _sum; }
    // полное врем€ на все завершенные запуски
  int num() const { return _num; }
    // количество завершенных запусков
  double mid() const { return _sum / _num; }
    // среднее врем€ одного полного запуска
  double res() const { return _tm.res(); }
    // разрешение базового таймера

private:
  timer _tm;
  int _num;
  double _sum;
};

}; // namespace lwml

#endif // _TIMERS_
