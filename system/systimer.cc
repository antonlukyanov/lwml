#include "systimer.h"
#include "lwmlconf.h"

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

  // ���������, ���� �� ������� ���������� ��� ������ ���������.
  double _resol = init();

#if OS_WIN
  bool _is_hrt;

  //!! SIC: ������������� ���� ���������� ������-�� �� �����������,
  // �� ��������� ��������� �������� ������� �������� ����������
  // ��� ��������� ������� ���������� ������� �� ���� �����
  // �������� ������������ �������� �������� � �������.
  __int64 _start;
#endif

  double init()
  {
    double res = 1.0/CLOCKS_PER_SEC;

#if OS_WIN
    LARGE_INTEGER buf;
    _is_hrt = QueryPerformanceFrequency(&buf);
    if( _is_hrt ){
      res = 1.0/static_cast<double>(buf.QuadPart); //!! SIC: ����� ������������ �����
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
    return static_cast<double>(tm.QuadPart - _start) * _resol; //!! SIC: �������� ������������ �����
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
