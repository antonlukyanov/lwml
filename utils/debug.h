// Простые средства организации отладочного вывода.
// lwml, (c) ltwood

#ifndef _DEBUG_
#define _DEBUG_

#include "defs.h"
#include "cstrng.h"
#include "i_stream.h"
#include "platforms.h"

/*#lake:stop*/

namespace lwml {

// Контекст вывода отладочных сообщений

class zzz_ctx_holder : public value {
public:
  zzz_ctx_holder( const char* ctx );
  ~zzz_ctx_holder();
};

#define zzz_ctx( nm ) zzz_ctx_holder zzz_ctx_object(nm)

// Функции отладочного вывода в единый для приложения файл лога.
// Файл лога открывается на запись при первой попытке доступа.

void zzz_ex_v( const char* asp, const char* msg, va_list va );
void zzz_ex( const char* asp, const char* msg, ... ) PRINTF(2,3);
void zzz( const char *fmt, ... ) PRINTF(1,2);

// Функция zzz_dump() позволяет использовать общий механизм llogsrv для управления
// отладочным выводом внутренних структур данных приложения в файлы дампов.
// Функция возвращает истину, если успешно загружена библиотека llogsrv
// и в домашней директории приложения присутствует директория dump.
// Сервис домашней директории предоставляет возможность автоматической генерации
// имен для файлов дампа.

bool zzz_dump();

// Функция zzz_dump_name() по относительному имени файла
// (т.е. имени и расширению без пути) образует полное имя файла дампа

strng zzz_dump_name( const char* fmt, ... ) PRINTF(1,2);

// Реализация потока вывода поверх потока отладочного вывода

class zstream : public i_stream, public value {
public:
  zstream( const char* asp = 0 ) : _asp( asp ? asp : "") {}

  // вывод строки
  virtual void out( const strng& s );

  // выталкивание потока
  virtual void flush() {}

private:
  cstrng _asp;
};

}; // namespace lwml

#endif // _DEBUG_
