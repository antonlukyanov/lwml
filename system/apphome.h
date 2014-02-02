// Сервис домашней директории приложения
// lwml, (c) ltwood

#ifndef _APPHOME_
#define _APPHOME_

#include "defs.h"
#include "mdefs.h"
#include "cstrng.h"
#include "platforms.h"

/*#lake:stop*/

// Сервис домашней директории приложения обеспечивает возможность
// открывать файлы, ассоциированные с приложением, а не пользователем.
// В версии под win32 файлы будут размещаться в той же директории,
// что и исполнимый модуль приложения.
// Под *nix файлы размещаются по фиксированному пути, который извлекается
// из переменной среды LWML_HOME (не реализовано).
// Функция mk_fname() по относительному имени файла (т.е. имени и расширению без пути)
// образует полное имя файла дампа, размещенного в поддиректории dump
// домашней директории приложения.

namespace lwml {

class apphome : public scope {
public:
  static strng mk_fname( const char* fnm );

  static strng mk_dump( const char* fmt, ... ) PRINTF(1,2);
  static strng mk_dump_v( const char* fmt, va_list va );
};

}; // namespace lwml

#endif // _APPHOME_
