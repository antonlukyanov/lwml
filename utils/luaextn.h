// lua extension library
// lwml, (c) ltwood

#ifndef _LUAEXTN_
#define _LUAEXTN_

#include "defs.h"
#include "mdefs.h"
#include "basex.h"

#include "liblua.h"

/*#lake:stop*/

namespace lwml {

// ���� ������ �� �������� ���������� � ����� ������ �����������.
// �� ������������ ������������� ��� ������������� � �������� ���������
// �������, ���������� ������ � ������ ��� �������, ���������� �������
// �������� ��������� lua_State*.
// ������ ������ ���� �������������� �� ����� �� �� �� ���� �����������
// �� ������ -- ������ � ���� ������ ������������� ������������ ����������
// ������� arg_num().

class luaextn : public hardcore {
public:
  luaextn( lua_State *L ) : _L(L), _out_count(0) {
    _arg_num = lua_gettop(_L);
  }

  // ��������� ���������� ���������� �������.
  int arg_num() const { return _arg_num; }

  // ��������� ���������� ������������ ��������.
  // ��� �������� ������������ ��� ������������ �������� �������.
  int ret_num() const { return _out_count; }

  // �������� ���� ���������� ������� �� �����.
  // �������� num -- ����� ��������� ��� ��������� ���������� ����� �������.
  // ������������� ����� ������������ �������� �� ��� nil.
  // ��� �������� ����� � ������������ ����� ��������������� ������ �������
  // is_num(), �.�. ��������� ������������ ����� �� �������� ���������.
  // ���������� ��� ������ ������ ���������, �������� �� ��� ����������� ���������
  // �.�. �������� �� ��� �������� ��������, ������������������ ������ �������.

  bool is_nil( int num ) const;
  bool is_bool( int num ) const;
  bool is_num( int num ) const;
  bool is_str( int num ) const;
  bool is_tbl( int num ) const;

  // ��������� ���������� ������� �� ����� � ��������� ����.
  // �������� num -- ����� ��������� ��� ��������� ���������� ����� �������.
  // ��� ������� � �������� ������������ ���� ������� get_len(), get_idx()
  // ��� ��������� ����� ������� � ��������� �������� ������� �� ��� �������.
  // ���������� ������ ������� �������, ���������� �������� ��������,
  // ������������������ ������ �������.
  // ��� ���������� �������� ������� ���������� � ����.

  bool get_bool( int num ) const;
  real get_real( int num ) const;
  int  get_int( int num ) const;
  const char* get_str( int num ) const;
  int  get_len( int num ) const;
  real get_idx( int num, int idx ) const;

  // ��������� ���������� �� ����.
  // �������������� �������������� ������� ����� ���������� �� ���� �����������.
  // ����� ����� ������������ �������� ����� ���� �������� ����� ������ ������ ret_num().
  // ������� put_error() �������� �� ���� nil � ��������� �� ������ (�������� ������� assert()).

  void put_nil();
  void put_bool( bool val );
  void put_real( real val );
  void put_int( int val );
  void put_str( const char* str );
  void put_fmt( const char* fmt, ... ) PRINTF(2,3);
  void put_error( const char* fmt, ... ) PRINTF(2,3);

  // ��������� ����������, ������������� �������������� Lua.
  // ������� arg_error() ���������� ���������� "bad argument #<num> to <func> (<msg>)".
  // ������� handlex() ���������� ������ �� ���������� ����������.

  void error( const char* fmt, ... ) const PRINTF(2,3);
  void arg_error( int num, const char* fmt, ... ) const PRINTF(3,4);
  void handlex( const ex_base& ex ) const;

  // ����������� ��������� � �������.

  void reg_library( const char* lib_name, const luaL_reg* lib_exptbl );
  void reg_class( const char* class_name, const luaL_reg* class_ftbl, const luaL_reg* class_mtbl );

  // �������� � ��������� ������� ������.

  template<typename T>
    T* create_object( const char* class_name, T* ptr );
  template<typename T>
    T* get_object( const char* class_name, int num = 1 );

  // ��������� ��� ����, ������������ ������������������ � lua-�������

  lua_State* get_raw_struct() { return _L; }

  void i_have_created_stack_element() { _out_count++; }
  void i_have_deleted_stack_element() { _out_count--; }

private:
  lua_State *_L;
  int _out_count;       // ���������� ������������ ��������
  int _arg_num;         // ���������� ���������� (������� ����� ��� �������� �������)
};

template<typename T>
T* luaextn::create_object( const char* class_name, T* ptr )
{
  T** a = static_cast<T**>(lua_newuserdata(_L, sizeof(T*)));
  *a = ptr;
  ++_out_count;
  luaL_getmetatable(_L, class_name);
  lua_setmetatable(_L, -2);
  return *a;
}

template<typename T>
T* luaextn::get_object( const char* class_name, int num )
{
  void *ud = luaL_checkudata(_L, num, class_name);
  if( ud == 0 ){
    const char* msg = lua_pushfstring(_L, "object <%s> expected", class_name);
    luaL_argerror(_L, num, msg);
  }
  T** a = static_cast<T**>(ud);
  return *a;
}

// ������� ��� ��������� ���������� � �������������� ��������

#define LUA_TRY                                                 \
  try{                                                          //

#define LUA_CATCH(lua)                                          \
  } catch( const ex_base& lua_catch_ex ) {                      \
    lua.handlex(lua_catch_ex);                                  \
    return 0;                                                   \
  }                                                             //

#define LUA_CATCH_RET(lua)                                      \
  } catch( const ex_base& lua_catch_ex ) {                      \
    lua.put_error(lua_catch_ex.msg());                          \
    return 2;                                                   \
  }                                                             //

// ������� ��� ����������� ��������� �������.

// ������ ��� ���������� C-��������� lua-�������.
// ������� ����� ���������� ��� � C-���� � lua-����.
#define LUA_DEF_FUNCTION( fn_name, arg_name )                   \
  int fn_name( lua_State* arg_name )                            //

// ����������� �������� ���������� �������.
#define LUA_FUNCTION( fn_name )                                 \
    {#fn_name, fn_name},                                        //

// ������ �������� ���������� �������.
// ������� ������� �������� � ������ [lib_name]_exptbl.
#define LUA_BEGIN_LIBRARY( lib_name )                            \
  static const luaL_reg lib_name##_exptbl[] = {                 //

// ���������� �������� ���������� �������.
#define LUA_END_LIBRARY                                         \
    {NULL, NULL}                                                \
  };                                                            //

// ������� ��� ����������� �������.
// ��� ������ ���������� �� ����� ������ � ����� ������ �� ������� [class]_[method].
// ��� ������� ������ ��������� ������� �������� � ������� [class]_ftbl � [class]_mtbl.
// � ������ ������ ������ �������������� ������ � �������
// [class]_init(), [class]_info() � [class]_done()
// ��� �������� �������, �������������� ������� � ������ � ���������� �������.

// ������ ��� ���������� C-��������� lua-������
#define LUA_DEF_METHOD( cl_name, mt_name, arg_name )            \
  int cl_name ## _ ## mt_name( lua_State* arg_name )            //

#define LUA_DEF_CTOR( cl_name, arg_name )                       \
  int cl_name##_init( lua_State* arg_name )                     //

#define LUA_DEF_DTOR( cl_name, arg_name )                       \
  int cl_name##_done( lua_State* arg_name )                     //

#define LUA_DEF_DUMP( cl_name, arg_name )                       \
  int cl_name##_info( lua_State* arg_name )                     //

#define LUA_DEF_LEN( cl_name, arg_name )                        \
  int cl_name##_meta_len( lua_State* arg_name )                 //

#define LUA_DEF_GETIDX( cl_name, arg_name )                     \
  int cl_name##_meta_getidx( lua_State* arg_name )              //

#define LUA_DEF_SETIDX( cl_name, arg_name )                     \
  int cl_name##_meta_setidx( lua_State* arg_name )              //

// ����������� �������� ���������� ������� (������� �����)
#define LUA_METHOD( cl_name, mt_name )                          \
    {#mt_name, cl_name##_##mt_name},                            //

// ������ �������� ������
#define LUA_BEGIN_CLASS(cl_name)                                \
  static const luaL_reg cl_name##_ftbl[] = {                    \
    {"new", cl_name##_init},                                    //

// ������� � ����������� ������� ������
#define LUA_META(cl_name)                                       \
    {NULL, NULL}                                                \
  };                                                            \
  static const luaL_reg cl_name##_mtbl[] = {                    \
    {"__tostring", cl_name##_info},                             \
    {"__gc", cl_name##_done},                                   //

// ������� � ����������� ������� ������ ��� �����������
#define LUA_META_IDX(cl_name)                                   \
    {NULL, NULL}                                                \
  };                                                            \
  static const luaL_reg cl_name##_mtbl[] = {                    \
    {"__len", cl_name##_meta_len},                              \
    {"__index", cl_name##_meta_getidx},                         \
    {"__newindex", cl_name##_meta_setidx},                      \
    {"__tostring", cl_name##_info},                             \
    {"__gc", cl_name##_done},                                   //

// ��������� �������� ������
#define LUA_END_CLASS                                           \
    {NULL, NULL}                                                \
  };                                                            //

// ������� ��� �������� � ��������� �������� ������
// �������� ��������� -- ���������� ������������ ������
// � C-���� � � lua-����.

#define CREATE_OBJECT( class_name, ptr )    create_object<class_name>(#class_name, ptr)
#define GET_OBJECT( class_name )            get_object<class_name>(#class_name)
#define GET_OBJECT_IDX( class_name, num )   get_object<class_name>(#class_name, num)

// ������ ��� �������� ��������� ������.

// ������ ������������ ������� �������� ����������� ������.
// ������� ������� �������� � ������ luaopen_[pack_name].
#define LUA_BEGIN_EXPORT( pack_name )                           \
  LWML_EXPORT                                                   \
    int luaopen_##pack_name( lua_State* lua_vm )                \
    {                                                           \
      luaextn lua_obj(lua_vm);                                  //

// ������� ���������� �������.
#define LUA_EXPORT_LIBRARY( lib_name )                          \
  lua_obj.reg_library(#lib_name, lib_name##_exptbl);            //

// ������� ������.
#define LUA_EXPORT_CLASS( cl_name )                             \
  lua_obj.reg_class(#cl_name, cl_name##_ftbl, cl_name##_mtbl);  //

// ��������� ������������ ������� �������� ����������� ������.
#define LUA_END_EXPORT                                          \
    return lua_obj.ret_num();                                   \
  }                                                             //

}; // namespace lwml

/* ������� �������������:

  // ���������� ���������� �������
  // ----------------------------------------------------------------------
  LUA_DEF_FUNCTION( myfunc, L ) // ���������� �������
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH_RET(ex)
  }

  LUA_BEGIN_LIBRARY(mylib) // ���������� ���������, ��������������� ���������� ��� ��������
    LUA_FUNCTION(myfunc)
  LUA_END_LIBRARY

  LUA_BEGIN_EXPORT(mypack) // ������������ ����������
    LUA_EXPORT_LIBRARY(mylib)
  LUA_END_EXPORT
  // ----------------------------------------------------------------------

  // ���������� �����
  // ----------------------------------------------------------------------
  LUA_DEF_CTOR(myclass, L) // �����������
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH_RET(ex)
  }

  LUA_DEF_DTOR(myclass, L) // ����������
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH(ex)
  }

  LUA_DEF_DUMP(myclass, L) // �������������� � ������
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH(ex)
  }

  LUA_DEF_METHOD(myclass, myclassfunc, L) // ������ ������
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH_RET(ex)
  }

  LUA_DEF_METHOD(myclass, mymethod, L) // ����� ������� ������
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH_RET(ex)
  }

  // ���������� ���������, ��������������� ������ ��� ��������
  LUA_BEGIN_CLASS(myclass)                // ��� ������� ������� ������
    LUA_METHOD(myclass, myclassfunc)
  LUA_META(myclass)                       // ��� ������� ������
    LUA_METHOD(myclass, mymethod)
  LUA_END_CLASS

  LUA_BEGIN_EXPORT(mypack) // ������������ �����
    LUA_EXPORT_CLASS(myclass)
  LUA_END_EXPORT
  // ----------------------------------------------------------------------

  // ���������� �����-���������
  // ����� ������� ������ ������������� ������, ��������� �����������
  // � �������� ���������, ��������������� ������ ��� ��������
  // (��� ���������� ������� LUA_META �� LUA_META_IDX)
  // ----------------------------------------------------------------------
  LUA_DEF_LEN(myclass, L) // ��������� �����
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH(ex)
  }

  LUA_DEF_GETIDX(myclass, L) // ��������� �������� �������� �� �������
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH(ex)
  }

  LUA_DEF_SETIDX(myclass, L) // ��������� �������� �������� �� �������
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH(ex)

  // ���������� ���������, ��������������� ������ ��� ��������
  LUA_BEGIN_CLASS(myclass)                // ��� ������� ������� ������
    LUA_METHOD(myclass, myclassfunc)
  LUA_META_IDX(myclass)                   // ��� ������� ������
    LUA_METHOD(myclass, mymethod)
  LUA_END_CLASS
  }
  // ----------------------------------------------------------------------
*/

#endif // _LUAEXTN_
