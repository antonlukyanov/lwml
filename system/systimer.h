// Системный таймер
// lwml, (c) ltwood

#ifndef _SYSTIMER_
#define _SYSTIMER_

#include "defs.h"

/*#lake:stop*/

namespace lwml {

// Системный таймер возвращает (функция time()) текущее время в секундах.
// Текущее время отсчитывется от произвольного момента времени.
// Функция res() возвращает разрешение таймера в секундах.
// По возможности используется таймер высокого разрешения.

class systimer : public scope {
public:
  static double res();

  static double time();

  static void sleep( int ms = 0 );
};

}; // namespace lwml

#endif // _SYSTIMER_
