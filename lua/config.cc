#include "config.h"

#include "apphome.h"
#include "filename.h"
#include "refowner.h"
#include "debug.h"
#include "stdmem.h"

/*#lake:stop*/

namespace lwml {

namespace {
  referer<luaconf> _appcfg; //!! TODO: многопоточность?

  void init_appcfg( const char* fname = 0 )
  {
    if( fname ){
      zzz_ex("lwml:config", "reading specified config");
      _appcfg = luaconf::create(fname);
    } else if( filename::is_exists("config") ){
      zzz_ex("lwml:config", "reading local config");
      _appcfg = luaconf::create("config");
    } else {
      zzz_ex("lwml:config", "reading home config");
      _appcfg = luaconf::create(apphome::mk_fname("config").ascstr());
    }
  }
};

referer<luaconf> config()
{
  if( !_appcfg.is_ok() )
    init_appcfg();
  return _appcfg;
}

void reset_config( const char* fname )
{
  init_appcfg(fname);
}

void reset_config( const char* cfg, int len )
{
  zzz_ex("lwml:config", "reading config from specified buffer");
  _appcfg = luaconf::create(cfg, len);
}

}; // namespace lwml
