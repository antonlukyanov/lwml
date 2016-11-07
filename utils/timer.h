// Простой универсальный таймер
// lwml, (c) ltwood

#ifndef _TIMERS_
#define _TIMERS_

#include "lwml/base/defs.h"
#include "lwml/types/cstrng.h"
#include "lwml/lwmlconf.h"

#include "lwml/system/systimer.h"

/*#lake:stop*/

namespace lwml {

class timer : public value {
public:
  // Начать отсчет времени
  timer() { restart(); }

  // Сбросить таймер в исходное состояние и начать новый отсчет времени
  void restart()  { _start = systimer::time(); }

  // Получить время, прошедшее с момента последнего начала отсчета
  double left() const { return systimer::time() - _start; }

  // Получить абсолютное текущее системное время
  static double time() { return systimer::time(); }

  // Получить разрешение используемого таймера
  static double res() { return systimer::res(); }

  // Получить полное текстовое описание текущего момента времени
  // Возвращает строку, содержащую полную временную метку
  // текущего момента времени (дата-время).
  static strng timestamp();

private:
  double _start;
};

// Таймер многократного запуска.
//
// Такой таймер можно многократно запускать и останавливать,
// получая в любой момент количество завершенных запусков,
// суммарное время на все запуски и среднее время на один запуск.

class mtimer : public value {
public:
  mtimer() : _num(0), _sum(0.0) {}

  void start() { _tm.restart(); }

  // возвращает время, прошедшее с момента старта
  double stop() {
    double left = _tm.left();
    _sum += left;
    _num++;
    return left;
  }

  double sum() const { return _sum; }
    // полное время на все завершенные запуски
  int num() const { return _num; }
    // количество завершенных запусков
  double mid() const { return _sum / _num; }
    // среднее время одного полного запуска
  double res() const { return _tm.res(); }
    // разрешение базового таймера

private:
  timer _tm;
  int _num;
  double _sum;
};

}; // namespace lwml

#endif // _TIMERS_
