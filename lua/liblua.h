// Включение заголовков интерпретатора Lua
// lwml, (c) ltwood

#ifndef _LIBLUA_
#define _LIBLUA_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

// Для компиляции lua используется g++ с включенным мэнглингом имен,
// поэтому заголовочные файлы включаются без extern "C".

extern "C" {
  // В Lua 5.2 нет luaL_register и некоторых других функций/структур.
  #define LUA_COMPAT_MODULE
  #include <lauxlib.h>
  #include <lua.h>
  #include <lualib.h>
};

#endif // _LIBLUA_
