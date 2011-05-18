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

// Этот объект не является безопасным с точки зрения копирования.
// Он предназначен исключительно для использования в качестве стекового
// объекта, доступного только в рамках той функции, аргументом которой
// является указатель lua_State*.
// Объект должен быть сконструирован до каких бы то ни было манипуляций
// со стеком -- только в этом случае гарантируется корректность результата
// функции arg_num().

class luaextn : public hardcore {
public:
  luaextn( lua_State *L ) : _L(L), _out_count(0) {
    _arg_num = lua_gettop(_L);
  }

  // Получение количества аргументов функции.
  int arg_num() const { return _arg_num; }

  // Получение количества возвращаемых значений.
  // Это значение используется как возвращаемое значение функции.
  int ret_num() const { return _out_count; }

  // Проверка типа аргументов функции со стека.
  // Аргумент num -- номер аргумента при нумерации аргументов слева направо.
  // Дополнительно здесь присутствует проверка на тип nil.
  // Для проверки целых и вещественных чисел предоставляется единая функция
  // is_num(), т.е. запрещено осуществлять выбор по значению аргумента.
  // Аналогично для таблиц нельзя проверить, являются ли они корректными массивами
  // т.е. содержат ли они числовые значения, проиндексированные целыми числами.

  bool is_nil( int num ) const;
  bool is_bool( int num ) const;
  bool is_num( int num ) const;
  bool is_str( int num ) const;
  bool is_tbl( int num ) const;

  // Получение аргументов функции со стека с проверкой типа.
  // Аргумент num -- номер аргумента при нумерации аргументов слева направо.
  // Для доступа к массивам используется пара функций get_len(), get_idx()
  // для получения длины массива и получения элемента массива по его индексу.
  // Поддержаны только простые массивы, содержащие числовые значения,
  // проиндексированные целыми числами.
  // При индексации элементы массива нумеруются с нуля.

  bool get_bool( int num ) const;
  real get_real( int num ) const;
  int  get_int( int num ) const;
  const char* get_str( int num ) const;
  int  get_len( int num ) const;
  real get_idx( int num, int idx ) const;

  // Помещение результата на стек.
  // Обеспечивается автоматический подсчет числа помещенных на стек результатов.
  // Общее число возвращаемых значений может быть получено путем вызова метода ret_num().
  // Функция put_error() помещает на стек nil и сообщение об ошибке (контракт функции assert()).

  void put_nil();
  void put_bool( bool val );
  void put_real( real val );
  void put_int( int val );
  void put_str( const char* str );
  void put_fmt( const char* fmt, ... ) PRINTF(2,3);
  void put_error( const char* fmt, ... ) PRINTF(2,3);

  // Генерация исключения, передаваемого интерпретатору Lua.
  // Функция arg_error() генерирует исключение "bad argument #<num> to <func> (<msg>)".
  // Функция handlex() генерирует ошибку по пойманному исключению.

  void error( const char* fmt, ... ) const PRINTF(2,3);
  void arg_error( int num, const char* fmt, ... ) const PRINTF(3,4);
  void handlex( const ex_base& ex ) const;

  // Регистрация библиотек и классов.

  void reg_library( const char* lib_name, const luaL_reg* lib_exptbl );
  void reg_class( const char* class_name, const luaL_reg* class_ftbl, const luaL_reg* class_mtbl );

  // Создание и получение объекта класса.

  template<typename T>
    T* create_object( const char* class_name, T* ptr );
  template<typename T>
    T* get_object( const char* class_name, int num = 1 );

  // интерфейс для кода, нетривиально взаимодействующего с lua-машиной

  lua_State* get_raw_struct() { return _L; }

  void i_have_created_stack_element() { _out_count++; }
  void i_have_deleted_stack_element() { _out_count--; }

private:
  lua_State *_L;
  int _out_count;       // количество возвращаемых значений
  int _arg_num;         // количество аргументов (глубина стека при создании объекта)
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

// Макросы для обработки исключений в экспортируемых функциях

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

// Макросы для определения библиотек функций.

// Макрос для порождения C-заголовка lua-функции.
// Функция имеет одинаковое имя в C-коде и lua-коде.
#define LUA_DEF_FUNCTION( fn_name, arg_name )                   \
  int fn_name( lua_State* arg_name )                            //

// Определение элемента библиотеки функций.
#define LUA_FUNCTION( fn_name )                                 \
    {#fn_name, fn_name},                                        //

// Начало создания библиотеки функций.
// Создает таблицу экспорта с именем [lib_name]_exptbl.
#define LUA_BEGIN_LIBRARY( lib_name )                            \
  static const luaL_reg lib_name##_exptbl[] = {                 //

// Завершение создания библиотеки функций.
#define LUA_END_LIBRARY                                         \
    {NULL, NULL}                                                \
  };                                                            //

// Макросы для определения классов.
// Имя метода образуется из имени класса и имени метода по правилу [class]_[method].
// Для каждого класса создаются таблицы экспорта с именами [class]_ftbl и [class]_mtbl.
// В каждом классе должны присутствовать методы с именами
// [class]_init(), [class]_info() и [class]_done()
// для создания объекта, преобразования объекта в строку и разрушения объекта.

// Макрос для порождения C-заголовка lua-метода
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

// Определение элемента библиотеки методов (обычный метод)
#define LUA_METHOD( cl_name, mt_name )                          \
    {#mt_name, cl_name##_##mt_name},                            //

// Начало создания класса
#define LUA_BEGIN_CLASS(cl_name)                                \
  static const luaL_reg cl_name##_ftbl[] = {                    \
    {"new", cl_name##_init},                                    //

// Переход к определению методов класса
#define LUA_META(cl_name)                                       \
    {NULL, NULL}                                                \
  };                                                            \
  static const luaL_reg cl_name##_mtbl[] = {                    \
    {"__tostring", cl_name##_info},                             \
    {"__gc", cl_name##_done},                                   //

// Переход к определению методов класса для контейнеров
#define LUA_META_IDX(cl_name)                                   \
    {NULL, NULL}                                                \
  };                                                            \
  static const luaL_reg cl_name##_mtbl[] = {                    \
    {"__len", cl_name##_meta_len},                              \
    {"__index", cl_name##_meta_getidx},                         \
    {"__newindex", cl_name##_meta_setidx},                      \
    {"__tostring", cl_name##_info},                             \
    {"__gc", cl_name##_done},                                   //

// Окончание создания класса
#define LUA_END_CLASS                                           \
    {NULL, NULL}                                                \
  };                                                            //

// Макросы для создания и получения объектов класса
// Основное допущение -- одинаковое наименование класса
// в C-коде и в lua-коде.

#define CREATE_OBJECT( class_name, ptr )    create_object<class_name>(#class_name, ptr)
#define GET_OBJECT( class_name )            get_object<class_name>(#class_name)
#define GET_OBJECT_IDX( class_name, num )   get_object<class_name>(#class_name, num)

// Макрос для экспорта элементов пакета.

// Начало формирования функции экспорта содержимого пакета.
// Создает функцию экспорта с именем luaopen_[pack_name].
#define LUA_BEGIN_EXPORT( pack_name )                           \
  LWML_EXPORT                                                   \
    int luaopen_##pack_name( lua_State* lua_vm )                \
    {                                                           \
      luaextn lua_obj(lua_vm);                                  //

// Экспорт библиотеки функций.
#define LUA_EXPORT_LIBRARY( lib_name )                          \
  lua_obj.reg_library(#lib_name, lib_name##_exptbl);            //

// Экспорт класса.
#define LUA_EXPORT_CLASS( cl_name )                             \
  lua_obj.reg_class(#cl_name, cl_name##_ftbl, cl_name##_mtbl);  //

// Окончание формирования функции экспорта содержимого пакета.
#define LUA_END_EXPORT                                          \
    return lua_obj.ret_num();                                   \
  }                                                             //

}; // namespace lwml

/* Примеры использования:

  // определяем библиотеку функций
  // ----------------------------------------------------------------------
  LUA_DEF_FUNCTION( myfunc, L ) // определяем функцию
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH_RET(ex)
  }

  LUA_BEGIN_LIBRARY(mylib) // определяем структуру, соответствующую библиотеке при экспорте
    LUA_FUNCTION(myfunc)
  LUA_END_LIBRARY

  LUA_BEGIN_EXPORT(mypack) // экспортируем библиотеку
    LUA_EXPORT_LIBRARY(mylib)
  LUA_END_EXPORT
  // ----------------------------------------------------------------------

  // определяем класс
  // ----------------------------------------------------------------------
  LUA_DEF_CTOR(myclass, L) // конструктор
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH_RET(ex)
  }

  LUA_DEF_DTOR(myclass, L) // деструктор
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH(ex)
  }

  LUA_DEF_DUMP(myclass, L) // преобразование в строку
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH(ex)
  }

  LUA_DEF_METHOD(myclass, myclassfunc, L) // методы класса
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH_RET(ex)
  }

  LUA_DEF_METHOD(myclass, mymethod, L) // метод объекта класса
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH_RET(ex)
  }

  // определяем структуру, соответствующую классу при экспорте
  LUA_BEGIN_CLASS(myclass)                // все функции объекта класса
    LUA_METHOD(myclass, myclassfunc)
  LUA_META(myclass)                       // все функции класса
    LUA_METHOD(myclass, mymethod)
  LUA_END_CLASS

  LUA_BEGIN_EXPORT(mypack) // экспортируем класс
    LUA_EXPORT_CLASS(myclass)
  LUA_END_EXPORT
  // ----------------------------------------------------------------------

  // определяем класс-коллекцию
  // здесь описаны только дополительные методы, требующие определения
  // и описание структуры, соответствующей классу при экспорте
  // (оно отличается заменой LUA_META на LUA_META_IDX)
  // ----------------------------------------------------------------------
  LUA_DEF_LEN(myclass, L) // получение длины
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH(ex)
  }

  LUA_DEF_GETIDX(myclass, L) // получение значения элемента по индексу
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH(ex)
  }

  LUA_DEF_SETIDX(myclass, L) // установка значения элемента по индексу
  {
    luaextn ex(L);
  LUA_TRY
    return ex.ret_num();
  LUA_CATCH(ex)

  // определяем структуру, соответствующую классу при экспорте
  LUA_BEGIN_CLASS(myclass)                // все функции объекта класса
    LUA_METHOD(myclass, myclassfunc)
  LUA_META_IDX(myclass)                   // все функции класса
    LUA_METHOD(myclass, mymethod)
  LUA_END_CLASS
  }
  // ----------------------------------------------------------------------
*/

#endif // _LUAEXTN_
