#include "luaconfig.h"

#include "debug.h"

/*#lake:stop*/

namespace lwml {

namespace {
  int lua_zzz( lua_State* L ){
    try{
      strng res;
      int n = lua_gettop(L);  /* number of arguments */
      lua_getglobal(L, "tostring");
      for( int i = 1; i <= n; i++ ){
        lua_pushvalue(L, -1);  // function to be called
        lua_pushvalue(L, i);   // value to print
        lua_call(L, 1, -1);
        const char *s = lua_tostring(L, -1);  // get result
        if( i > 1 ) res.concat('\t');
        res.concat(s ? s : "<no-value>");
        lua_pop(L, 1);  // pop result
      }
      zzz_ex("lwml:luaconf", "lua: %s", res.ascstr());
      return 0;
    } catch( ex_base& ){
      return luaL_error(L, "exception occured inside Lua debug function");
    }
  }
};

static int lua_panic( lua_State* vm )
{
  throw ex_luaconf("error in unprotected code, panic");
}

void luaconf::open_vm()
{
  _vm = luaL_newstate();
  if( !_vm )
    throw ex_luaconf("can't create interpreter");

  lua_atpanic(_vm, lua_panic);

  luaL_openlibs(_vm);

  // экспортируем функцию отладочного вывода
  lua_pushcfunction(_vm, lua_zzz);
  lua_setglobal(_vm, "zzz");
}

void luaconf::close_vm()
{
  lua_close(_vm);
}

void luaconf::throw_luaerror() const
{
  ex_luaint ex("%s", lua_tostring(_vm, -1));
  lua_pop(_vm, 1);
  throw ex;
}

void luaconf::run_file( const char* fname ) const
{
  if( luaL_loadfile(_vm, fname) != 0 )
    throw_luaerror();
  if( lua_pcall(_vm, 0, 0, 0) )
    throw_luaerror();
}

void luaconf::run_str( const char* prog ) const
{
  if( luaL_loadbuffer(_vm, prog, strlen(prog), "=internal") != 0 )
    throw_luaerror();
  if( lua_pcall(_vm, 0, 0, 0) )
    throw_luaerror();
}

void luaconf::run_chunk( const char* chunk, int len ) const
{
  if( luaL_loadbuffer(_vm, chunk, len, "=internal") != 0 )
    throw_luaerror();
  if( lua_pcall(_vm, 0, 0, 0) )
    throw_luaerror();
}

void luaconf::pushexpr( const char* expr ) const
{
  strng cmd = strng::form("_RES = %s", expr);
  run_str(cmd.ascstr());
  lua_getglobal(_vm, "_RES");
  if( lua_isnil(_vm, -1) )
    throw ex_luaconf("nil result for <%s>", expr);
}

luaconf::luaconf()
{
  open_vm();
}

luaconf::luaconf( const char* fname )
{
  open_vm();
  run_file(fname);
}

luaconf::luaconf( const char* chunk, int len )
{
  open_vm();
  run_chunk(chunk, len);
}

luaconf::~luaconf()
{
  close_vm();
}

#define DEF_VGETPROC( proc_name, ret_type, type_name )                  \
  ret_type luaconf::proc_name( const char* expr, va_list va ) const     \
  {                                                                     \
    strng ex_expr = strng::vform(expr, va);                             \
    pushexpr(ex_expr.ascstr());                                         \
    if( !lua_is##type_name(_vm, -1) )                                   \
      throw ex_luaconf("can't convert <%s> to %s", ex_expr.ascstr(), #type_name);  \
    ret_type res = lua_to##type_name(_vm, -1);                          \
    lua_pop(_vm, 1);                                                    \
    return res;                                                         \
  }

DEF_VGETPROC(vget_str, strng, string)
DEF_VGETPROC(vget_real, real, number)
DEF_VGETPROC(vget_bool, bool, boolean)

// эта функция необычна проверкой полученного значения на целочисленность
int luaconf::vget_int( const char* expr, va_list va ) const
{
  real res = vget_real(expr, va);
  if( fmod(res, 1.0) != 0.0 ){
    strng ex_expr = strng::vform(expr, va);
    throw ex_luaconf("value for <%s> seems not to be an integer", ex_expr.ascstr());
  }
  return int_cast(res);
}

void* luaconf::vget_data( const char* name, const char* expr, va_list va ) const
{
  strng ex_expr = strng::vform(expr, va);
  pushexpr(ex_expr.ascstr());

  if( !lua_isuserdata(_vm, -1) )
    throw ex_luaconf("can't convert <%s> to userdata", ex_expr.ascstr());
  void** res = reinterpret_cast<void**>(lua_touserdata(_vm, -1));

  lua_pop(_vm, 1);
  return *res;
}

void luaconf::vexec( const char* expr, va_list va ) const
{
  strng cmd = strng::vform(expr, va);
  run_str(cmd.ascstr());
}

#define DEF_GETPROC(proc_name, ret_type)                        \
  ret_type luaconf::proc_name( const char* expr, ... ) const    \
  {                                                             \
    ret_type res;                                               \
    va_list va;                                                 \
    va_start(va, expr);                                         \
    try{                                                        \
      res = v##proc_name(expr, va);                             \
    }catch( ex_base& ){                                         \
      va_end(va);                                               \
      throw;                                                    \
    }                                                           \
    va_end(va);                                                 \
    return res;                                                 \
  }

DEF_GETPROC(get_str, strng)
DEF_GETPROC(get_real, real)
DEF_GETPROC(get_bool, bool)
DEF_GETPROC(get_int, int)

void* luaconf::get_data( const char* name, const char* expr, ... ) const
{
  void* res;
  va_list va;
  va_start(va, expr);
  try{
    res = vget_data(name, expr, va);
  }catch( ex_base& ){
    va_end(va);
    throw;
  }
  va_end(va);
  return res;
}

void luaconf::exec( const char* expr, ... ) const
{
  va_list va;
  va_start(va, expr);
  try{
    vexec(expr, va);
  }catch( ex_base& ){
    va_end(va);
    throw;
  }
  va_end(va);
}

}; // namespace lwml
