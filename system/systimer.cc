#include "lwml/system/systimer.h"
#include "lwml/lwmlconf.h"

/*#lake:stop*/

#if OS_WIN
  #include <windows.h>
#else
  #include <time.h>
  #include <unistd.h>
#endif

namespace lwml {

namespace {
  double init();

  // Неказисто, зато не требует блокировок при каждом обращении.
  double _resol = init();

#if OS_WIN
  bool _is_hrt;

  //!! SIC: использование этой переменной вообще-то не обязательно,
  // но позволяет увеличить точность таймера высокого разрешения
  // при измерении больших интервалов времени за счет более
  // позднего выдавливания значащих разрядов в порядок.
  __int64 _start;
#endif

  double init()
  {
    double res = 1.0/CLOCKS_PER_SEC;

#if OS_WIN
    LARGE_INTEGER buf;
    _is_hrt = QueryPerformanceFrequency(&buf);
    if( _is_hrt ){
      res = 1.0/static_cast<double>(buf.QuadPart); //!! SIC: делим вещественные числа
      QueryPerformanceCounter(&buf);
      _start = buf.QuadPart;
    }
#endif

    return res;
  }
};

double systimer::res()
{
  return _resol;
}

double systimer::time()
{
#if OS_WIN
  if( _is_hrt ){
    LARGE_INTEGER tm;
    QueryPerformanceCounter(&tm);
    return static_cast<double>(tm.QuadPart - _start) * _resol; //!! SIC: умножаем вещественные числа
  } else
    return clock() * _resol;
#else
  return clock() * _resol;
#endif
}

void systimer::sleep( int ms )
{
#if OS_WIN
  Sleep(ms);
#else
  ::usleep(ms * 1000);
#endif
}

}; // namespace lwml
