// Динамическая загрузка библиотек
// lwml, (c) ltwood

#ifndef _DLOAD_
#define _DLOAD_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/base/basex.h"

/*#lake:stop*/

namespace lwml {

// Исключение при неудачной загрузке библиотеки

DEF_EX_TYPE_MSG(ex_base, ex_dll, "can't load proc from dll");

// Процедура, загружаемая из динамической библиотеки
typedef int (*dproc)();

// Загрузка динамической библиотеки.
// При неудачной загрузке возвращает 0.
void* dl_load( const char* );
void* dl_load_nozzz( const char* ); // no log output

// Загрузка процедуры из ранее загруженной библиотеки.
// При нулевой библиотеке генерирует assert,
// при неудачной загрузке генерирует исключение ex_dll.
dproc dl_proc( void*, const char* );

// Макрос для загрузки функции заданного типа

#define LOAD_PROC(dll_ptr, proc_name, proc_type) \
  reinterpret_cast<proc_type>(dl_proc(dll_ptr, proc_name));

}; // namespace lwml

#endif // _DLOAD_
