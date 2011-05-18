// Среда для консольных численных программ
// lwml, (c) ltwood

#ifndef _CONSOLE_
#define _CONSOLE_

#include "defs.h"

#include "basex.h"
#include "cstrng.h"
#include "progress.h"

/*#lake:stop*/

namespace lwml {

// Исключение консоли при отсутствии параметров или опций

DEF_EX_TYPE_MSG(ex_base, ex_console, "console error");

// Класс console реализует среду выполнения консольных вычислительных программ.
// При конструировании задаются аргументы функции main().
// Основная функциональность:
//   - немедленная остановка программы по ошибке
//   - обработка исключений и вывод сообщения об ошибке
//   - получение аргументов командной строки и опций
// Дополнительно консоль регистрирует консольный рендерер прогресс-индикатора
// и сервис домашней директории приложения.
//
// Опции командной строки состоят из одного символа, перед которым
// стоит знак '-' и могут располагаться в произвольном месте командной строки.
// Аргумент опции может следовать непосредственно за символом опции
// или отделяется от него символом ':' или символом '='.
// Аргументы считаются и нумеруются без учета опций.

class console : public scope {
public:
  enum errmsg_mode { USE_SYSMSG, USE_CONSOLE };

  static void init( int argc, char *argv[], errmsg_mode errmsg = USE_CONSOLE );

  // Консольные обработчики ошибок

  // Непосредственная остановка программы с выдачей сообщения об ошибке.
  static void abort( const char *msg );

  // Остановка по неверным аргументам командной строки.
  // Аргументы - название программы и паттерн корректного вызова.
  static void usage( const char *name, const char *usage );

  // Обработчик перехваченного исключения.
  // Печатает сообщение и останавливает программу
  static void handlex( const ex_base& ex );

  // Интерфейс доступа к командной строке

  // получение параметров приложения
  static strng app_name();          // имя исполнимого модуля программы
  static strng app_path();          // путь к исполнимому модулю программы

  // получение аргументов
  static int   argc();              // число аргументов командной строки
  static strng argv( int k = 0 );   // k-й аргумент командной строки

  // получение опций
  static bool  is_opt( char ch );   // проверить наличие опции
  static strng get_opt( char ch );  // получить аргумент опции как строку

private:
  static void print_msg( const char* title, const char* msg );
  static void print_msg2( const char* title, const char* msg );
};

}; // namespace lwml

#endif // _CONSOLE_
