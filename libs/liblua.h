// ��������� ���������� �������������� Lua
// lwml, (c) ltwood

#ifndef _LIBLUA_
#define _LIBLUA_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

// ��� ���������� lua ������������ g++ � ���������� ���������� ����,
// ������� ������������ ����� ���������� ��� extern "C"

extern "C" {
  #include "lauxlib.h"
  #include "lua.h"
  #include "lualib.h"
};

#endif // _LIBLUA_
