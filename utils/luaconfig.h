// Файл конфигурации на основе интерпретатора lua
// lwml, (c) ltwood

#ifndef _LUACONF_
#define _LUACONF_

#include "defs.h"
#include "mdefs.h"
#include "cstrng.h"
#include "basex.h"
#include "stdmem.h"
#include "refcount.h"

#include "liblua.h"

/*#lake:stop*/

namespace lwml {

// Ошибка при работе с интерпретатором

DEF_EX_TYPE_MSG(ex_base, ex_luaconf, "luaconf error");

// Внутренняя ошибка интерпретатора

DEF_EX_TYPE_MSG(ex_base, ex_luaint, "internal lua error");

class luaconf : public refcount {
private:
  luaconf();
  luaconf( const char* fname );
  luaconf( const char* chunk, int len );
  ~luaconf();

public:
  // Конструирование поддержано только по имени файла и бинарному чанку.
  // Для конструирования по строке можно либо создать объект умалчиваемым
  // конструктором и вызвать run_str(), либо сконструировать объект
  // вызовом create(prog, strlen(prog)).

  static referer<luaconf> create(){
    return referer<luaconf>(new(lwml_alloc) luaconf());
  }
  static referer<luaconf> create( const char* fname ){
    return referer<luaconf>(new(lwml_alloc) luaconf(fname));
  }
  static referer<luaconf> create( const char* chunk, int len ){
    return referer<luaconf>(new(lwml_alloc) luaconf(chunk, len));
  }

  void run_file( const char* fname ) const;           // выполнить файл
  void run_str( const char* prog ) const;             // выполнить строку
  void run_chunk( const char* prog, int len ) const;  // выполнить бинарный chunk

  strng vget_str( const char* expr, va_list va ) const;
  real  vget_real( const char* expr, va_list va ) const;
  bool  vget_bool( const char* expr, va_list va ) const;
  int   vget_int( const char* expr, va_list va ) const;
  void* vget_data( const char* name, const char* expr, va_list va ) const;
  void  vexec( const char* expr, va_list va ) const;

  strng get_str( const char* expr, ... ) const PRINTF(2,3);
  real  get_real( const char* expr, ... ) const PRINTF(2,3);
  bool  get_bool( const char* expr, ... ) const PRINTF(2,3);
  int   get_int( const char* expr, ... ) const PRINTF(2,3);
  void* get_data( const char* name, const char* expr, ... ) const PRINTF(3,4);
  void  exec( const char* expr, ... ) const PRINTF(2,3);

private:
  lua_State* _vm;

  void open_vm();
  void close_vm();
  void throw_luaerror() const;
  void run_str_imp( const char* prog ) const;
  void pushexpr( const char* expr ) const;
};

}; // namespace lwml

#endif // _LUACONF_
