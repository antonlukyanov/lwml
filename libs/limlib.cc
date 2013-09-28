#include "limlib.h"

#include "dload.h"

/*#lake:stop*/

namespace lwml {

#define LIMLIB_OK    0 // код нормального выполнения
#define LIMLIB_FAIL -1 // код ошибки

//!! TODO: Тут все плохо с потоками.
// Раз, неблокировнные вызовы dll
// Два, буфер в errmsg

namespace {
  typedef int (*ver_t)();
  typedef const char* (*errmsg_t)();
  typedef int (*getsize_t)( const char* fn, int* lx, int* ly );
  typedef int (*load_t)( const char* fn, uchar* data );
  typedef int (*save_t)( const char* fn, int lx, int ly, const uchar* data );

  ver_t _ver = 0;
  errmsg_t _errmsg = 0;
  getsize_t _getsize = 0;
  load_t _load = 0;
  load_t _load_rgb = 0;
  save_t _save = 0;

  bool _is_active = false;
  bool _is_first_call = true;
};

bool limlib::init()
{
  try{
    if( _is_first_call ){
      _is_first_call = false;

      if( !_is_active ){
        void* dll = dl_load("limlib");

        if( dll ){
          _ver = LOAD_PROC(dll, "limlib_ver", ver_t);
          _errmsg = LOAD_PROC(dll, "limlib_errmsg", errmsg_t);
          _getsize = LOAD_PROC(dll, "limlib_size", getsize_t);
          _load = LOAD_PROC(dll, "limlib_load", load_t);
          _load_rgb = LOAD_PROC(dll, "limlib_load_rgb", load_t);
          _save = LOAD_PROC(dll, "limlib_save", save_t);

          _is_active = true;
        }
      }
    }
  } catch(...){
    throw;
  }

  if( _is_active ){
    const int limlib_min_ver = 4;
    int ver = _ver();
    if( ver < limlib_min_ver )
      runtime("<limlib> version is too old, ver. %d.xx or later required", limlib_min_ver);
  }

  return _is_active;
}

void limlib::test()
{
  if( !init() )
    fail_assert("library is not loaded, please use is_active()");
}

bool limlib::getsize( const char* fn, int& lx, int& ly )
{
  test();
  return _getsize(fn, &lx, &ly) == LIMLIB_OK;
}

bool limlib::load( const char* fn, uchar* data )
{
  test();
  return _load(fn, data) == LIMLIB_OK;
}

bool limlib::load_rgb( const char* fn, uchar* data )
{
  test();
  return _load_rgb(fn, data) == LIMLIB_OK;
}

bool limlib::save( const char* fn, int lx, int ly, const uchar* data )
{
  test();
  return _save(fn, lx, ly, data) == LIMLIB_OK;
}

const char* limlib::errmsg()
{
  test();
  return _errmsg();
}

}; // namespace lwml
