#include "fre.h"

#include "alphabet.h"
#include "lwmlconf.h"
/*#lake:stop*/

namespace lwml {

bool fre::is_match( const char* re, const char* fn )
{
  if( *re == 0 )
    return *fn == 0;
  if( *re == '*' )
    return matchstar(re+1, fn);
  
  if( *fn != 0 && (*re == '?' || 
      alphabet::tolower(*re, _FILENAME_CODEPAGE_ ) == alphabet::tolower(*fn, _FILENAME_CODEPAGE_ ))
  )
    return is_match(re+1, fn+1);
  return false;
}

bool fre::matchstar( const char* re, const char* fn )
{
  while( 1 ){
    if( is_match(re, fn) )
      return true;
    if( *fn == 0 )
      return false;
    fn++;
  }
}

}; // namespace lwml
