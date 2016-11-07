// ��������� ���������� �������������� Lua
// lwml, (c) ltwood

#ifndef _LIBLUA_
#define _LIBLUA_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

// ��� ���������� lua ������������ g++ � ���������� ���������� ����,
// ������� ������������ ����� ���������� ��� extern "C".

extern "C" {
  // � Lua 5.2 ��� luaL_register � ��������� ������ �������/��������.
  #define LUA_COMPAT_MODULE
  #include <lauxlib.h>
  #include <lua.h>
  #include <lualib.h>
};

#endif // _LIBLUA_
