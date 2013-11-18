// ¬ключение заголовков интерпретатора Lua
// lwml, (c) ltwood

#ifndef _LIBLUA_
#define _LIBLUA_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

// дл€ компил€ции lua используетс€ g++ с включенным мэнглингом имен,
// поэтому заголовочные файлы включаютс€ без extern "C"

extern "C" {
  #include "lauxlib.h"
  #include "lua.h"
  #include "lualib.h"
};

#endif // _LIBLUA_
