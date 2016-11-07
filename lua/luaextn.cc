#include "lwml/lua/luaextn.h"

/*#lake:stop*/

namespace lwml {

bool luaextn::is_nil( int num ) const
{
  return lua_isnil(_L, num);
}

bool luaextn::is_bool( int num ) const
{
  return lua_isboolean(_L, num);
}

bool luaextn::is_num( int num ) const
{
  return lua_isnumber(_L, num);
}

bool luaextn::is_str( int num ) const
{
  return lua_isstring(_L, num);
}

bool luaextn::is_tbl( int num ) const
{
  return lua_istable(_L, num);
}

// Получение аргументов функции со стека с проверкой типа.

bool luaextn::get_bool( int num ) const
{
  luaL_checkany(_L, num);  // любой тип включая nil
  return lua_toboolean(_L, num);
}

real luaextn::get_real( int num ) const
{
  return luaL_checknumber(_L, num);
}

int luaextn::get_int( int num ) const
{
  return int_cast(luaL_checknumber(_L, num));
}

const char* luaextn::get_str( int num ) const
{
  return luaL_checkstring(_L, num);
}

int luaextn::get_len( int num ) const
{
  luaL_checktype(_L, num, LUA_TTABLE);
  return luaL_len(_L, num);
}

real luaextn::get_idx( int num, int idx ) const
{
  luaL_checktype(_L, num, LUA_TTABLE);
  lua_rawgeti(_L, num, idx+1);
  luaL_argcheck(_L, lua_isnumber(_L, -1), num, "array of number expected");
  real val = lua_tonumber(_L, -1);
  lua_pop(_L, 1);
  return val;
}

// Помещение результата на стек.

void luaextn::put_nil()
{
  lua_pushnil(_L);
  ++_out_count;
}

void luaextn::put_bool( bool val )
{
  lua_pushboolean(_L, val);
  ++_out_count;
}

void luaextn::put_real( real val )
{
  lua_pushnumber(_L, val);
  ++_out_count;
}

void luaextn::put_int( int val )
{
  lua_pushnumber(_L, real_cast(val));
  ++_out_count;
}

void luaextn::put_str( const char* str )
{
  lua_pushstring(_L, str);
  ++_out_count;
}

void luaextn::put_fmt( const char* fmt, ... )
{
  va_list va;
  va_start(va, fmt);
  lua_pushvfstring(_L, fmt, va);
  ++_out_count;
  va_end(va);
}

void luaextn::put_error( const char* fmt, ... )
{
  lua_pushnil(_L);
  ++_out_count;
  va_list va;
  va_start(va, fmt);
  lua_pushvfstring(_L, fmt, va);
  ++_out_count;
  va_end(va);
}

// Генерация ошибки, передаваемой интерпретатору Lua.

void luaextn::error( const char* fmt, ... ) const
{
  va_list va;
  va_start(va, fmt);
  lua_pushvfstring(_L, fmt, va);
  va_end(va);
  lua_error(_L);
}

void luaextn::arg_error( int num, const char* fmt, ... ) const
{
  va_list va;
  va_start(va, fmt);
  const char* msg = lua_pushvfstring(_L, fmt, va);
  va_end(va);
  luaL_argerror(_L, num, msg);
}

void luaextn::handlex( const ex_base& ex ) const
{
  lua_pushstring(_L, ex.msg());
  lua_error(_L);
}

// Регистрация библиотек и классов.

void luaextn::reg_library( const char* lib_name, const luaL_Reg* lib_exptbl )
{
  // Создает пустую таблицу t, присваивает ее глобальной переменной lib_name
  // и регистрирует в ней все функции из списка lib_exptbl.
  luaL_register(_L, lib_name, lib_exptbl);
  ++_out_count;
}

void luaextn::reg_class( const char* class_name, const luaL_Reg* class_ftbl, const luaL_Reg* class_mtbl )
{
  luaL_newmetatable(_L, class_name);

  lua_pushstring(_L, "__index");  // index
  lua_pushvalue(_L, -2);          // value
  lua_settable(_L, -3);           // mt[index] = value

  luaL_register(_L, NULL, class_mtbl);        // put mtbl into mt

  luaL_register(_L, class_name, class_ftbl);  // put ftbl into class_name
  ++_out_count;
}

}; // namespace lwml
