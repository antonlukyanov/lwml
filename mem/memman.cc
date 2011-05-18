#include "memman.h"

/*#lake:stop*/

namespace lwml {

char* i_memman::strdup( const char *str )
{
  int buflen = strlen(str)+1;
  char *res = static_cast<char*>(alloc(buflen));
  return prot_strcpy(res, str, buflen);
}

}; // namespace lwml
