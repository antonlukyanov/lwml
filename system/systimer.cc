#include "systimer.h"
#include "lwmlconf.h"

/*#lake:stop*/

#if _USE_WIN32API_
  #include <windows.h>
#endif

namespace lwml {

namespace {
  double init();

  // Неказисто, зато не требует блокировок при каждом обращении.
  double _resol = init();

#if _USE_WIN32API_
  bool _is_hrt;

  //!! SIC: использование этой переменной вообще-то не обязательно,
  // но позволяет увеличить точность таймера высокого разрешения
  // при измерении больших интервалов времени за счет более
  // позднего выдавливания значащих разрядов в порядок.
  __int64 _start;
#endif

  double init()
  {
    double res = 1.0/CLK_TCK;

#if _USE_WIN32API_
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
#if _USE_WIN32API_
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
  Sleep(ms);
}

}; // namespace lwml
