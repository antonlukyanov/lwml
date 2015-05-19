// Системные средства показа сообщений об ошибках
// lwml, (c) ltwood

#ifndef _SYSMSG_
#define _SYSMSG_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

class syssrv : public scope {
public:
  // Вывести сообщение с помощью системного сервиса вывода сообщений.
  static void message( const char *title, const char *msg );

  // Вывести запрос с помощью системного сервиса вывода диалогов.
  static int ask( const char *title, const char *msg );

  // Получение уникального идентификатора процесса.
  static uint32 get_pid();

  // Получение уникального идентификатора потока.
  static uint32 get_tid();

  // Активизировать JustInTime (JIT) отладчик т.е. прервать работу приложения.
  // При компиляции библиотеки пакетом Mingw используется JIT-отладчик DrMingw.
  static void call_jit();

  // Вывод сообщения в системный поток диагностики.
  static void debug_message( const char* );

private:
  static void exit( const char* msg );
};

}; // namespace lwml

#endif // _SYSMSG_
