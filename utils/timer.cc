#include "timer.h"

/*#lake:stop*/

namespace lwml {

// timer

const int TIMESTAMP_BUFLEN = 64;

strng timer::timestamp()
{
  char buf[TIMESTAMP_BUFLEN]; // локальный буфер потоко-безопасный
  time_t t = ::time(NULL);
  prot_strcpy(buf, ctime(&t), TIMESTAMP_BUFLEN);
  { // kill '\n'
    int strl = strlen(buf);
    if( buf[strl-1] == '\n' )
      buf[strl-1] = 0;
  }
  return strng(buf);
}

}; // namespace lwml
