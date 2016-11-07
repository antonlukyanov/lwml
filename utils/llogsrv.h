// Сервис вывода диагностических сообщений
// lwml, (c) ltwood

#ifndef _LLOGSRV_
#define _LLOGSRV_

#include "lwml/base/defs.h"

/*#lake:stop*/

namespace lwml {

// Обертка над библиотекой llogsrv.dll, поддерживающей
// вывод диагностических сообщений в файлы отчета и лога.
// При неудачной загрузке библиотеки llogsrv функции вывода
// диагностических сообщений не производят никаких действий.
//
// is_active() - возвращает истину, если подсистема диагностики активна.
// out_log(asp, msg) - выводит в поток диагностики сообщение msg с аспектом asp.
// is_dump() - возвращает истину, если требуется вывод дампа.
// is_jit() - возвращает истину, если требуется возбуждение jit-исключения при ошибке.
// get_id() - возвращает идентификатор процесса.
// get_ct() - возвращает идентификатор момента времени.
// Последние две функции используются для формирования имени файла дампа.
// set_mode() - позволяет установить опции отладки принудительно.
// Функция инициализирует систему логгирования, если она еще не использовалась
// на момент вызова.
// Эта функция возвращает флаг успешности установки опций отладки.

// Метод setup_log_func() позволяет задать внутренний обработчик сообщений лога.
// Этот метод может быть вызван в любой момент.
// При этом независимо от текущего состояния логгер переходит в состояние,
// при котором вывод лога активирован, а вывод дампа и отлов событий jit выключены.
// Вызов метода setup_log_func() с нулевым аргументом переводит логгер
// в начальное состояние (при следующем выводе в лог будет выполнена обычная
// процедура инициализации лога).
// Если активирован внутренний обработчик сообщений лога, то функция set_mode()
// всегда возвращает false.

typedef void (*log_func_t)( const char* asp, const char* msg );

class llogsrv : public scope {
public:
  static void setup_log_func( log_func_t fn );

  static bool is_active();

  static void out_log( const char* asp, const char* msg );

  static bool is_dump();
  static bool is_jit();

  static uint get_id();
  static uint get_ct();

  enum {
    LOG_ON   = 0x01,
    LOG_OFF  = 0x02,
    DUMP_ON  = 0x04,
    DUMP_OFF = 0x08,
    JIT_ON   = 0x10,
    JIT_OFF  = 0x20
  };

  static bool set_mode( uint mask );

private:
  static bool init();
};

}; // namespace lwml

#endif // _LLOGSRV_
