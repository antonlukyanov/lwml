#include "lwml/utils/utest.h"
#include "lwml/io/stream.h"

/*#lake:stop*/

namespace lwml {

bool unit_test( const tft_item* tft, const char* fn )
{
  referer<stream> log = stream::create(fn, fmWRITE, fmTEXT);

  int err_num = 0;
  for( int j = 0; tft[j].func; j++ ){
    bool res = true;
    bool exs = false;
    try{
      res = tft[j].func();
    }catch( ex_base& ex ){
      ++err_num;
      exs = true;
      log->printf("exception: %s [%s] ", tft[j].func_name, tft[j].file_name);
      log->printf("%s\n", ex.msg());
      log->flush();
    }
    if( !exs && !res ){
      ++err_num;
      log->printf("error: %s [%s]", tft[j].func_name, tft[j].file_name);
      log->printf("\n");
      log->flush();
    }
  }
  return err_num == 0;
}

}; // namespace lwml
