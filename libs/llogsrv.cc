#include "llogsrv.h"

#include "syssrv.h"
#include "dload.h"
#include "mtask.h"

/*#lake:stop*/

namespace lwml {

namespace {
  typedef int (*getver_t)();
  typedef void (*outmsg_t)( const char* asp, const char* msg );
  typedef int (*isdump_t)();
  typedef uint (*getid_t)();
  typedef uint (*getct_t)();

  bool _is_first_call = true;
  bool _is_active = false;
  bool _is_log = false;
  bool _is_dump = false;
  bool _is_jit = false;

  getver_t _ver = 0;
  outmsg_t _log = 0;
  isdump_t _isdump = 0;
  getid_t  _getid = 0;
  getct_t  _getct = 0;

  locker _lock; // блокировщик для использования в init() на случай многопоточности
};

#define MIN_VER 3

bool llogsrv::init()
{
  _lock.lock();
  try {
    if( _is_first_call ){
      _is_first_call = false;

      if( getenv("LWML_ZZZ") != 0 ){
        try{
          void* dll = dl_load("llogsrv");
          if( dll ){
            _ver = LOAD_PROC(dll, "llogsrv_ver", getver_t);
            _log = LOAD_PROC(dll, "llogsrv_log", outmsg_t);
            _isdump = LOAD_PROC(dll, "llogsrv_isdump", isdump_t);
            _getid = LOAD_PROC(dll, "llogsrv_getid", getid_t);
            _getct = LOAD_PROC(dll, "llogsrv_getct", getct_t);
            _is_active = true;

            const char* zzz = getenv("LWML_ZZZ");
            _is_log = (strstr(zzz, ":log") != 0);
            _is_dump = (strstr(zzz, ":dump") != 0);
            _is_jit = (strstr(zzz, ":jit") != 0);

            if( _ver() < MIN_VER ){
              syssrv::message("Debug environment problems", "Deprecated version of llogsrv.dll");
              _is_active = false;
            }
          }
        }catch( ex_dll& ){
          syssrv::message("Debug environment problems", "Invalid llogsrv.dll, can't load");
          _is_active = false;
        }
      }
    }
  } catch(...){
    _lock.unlock();
    throw;
  }
  _lock.unlock();
  return _is_active;
}

void llogsrv::out_log( const char* asp, const char* msg )
{
  if( init() && _is_log ) 
    _log(asp, msg);
}

bool llogsrv::is_jit()
{
  return init() && _is_jit;
}

bool llogsrv::is_dump()
{
  if( !init() || !_is_dump )
    return false;
  return _isdump();
}

uint llogsrv::get_id()
{
  if( !init() || !_is_dump )
    return 0xffffffff;
  return _getid();
}

uint llogsrv::get_ct()
{
  if( !init() || !_is_dump )
    return 0xffffffff;
  return _getct();
}

}; // namespace lwml
