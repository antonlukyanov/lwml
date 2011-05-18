// Простой универсальный таймер
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

}; // namespace lwml

#endif // _TIMERS_
