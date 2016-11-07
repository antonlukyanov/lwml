// Операции с текущим набором символов

#ifndef _ALPHABET_
#define _ALPHABET_

#include "lwml/base/defs.h"

/*#lake:stop*/

namespace lwml {

class alphabet : public scope {
public:
  enum { alALT = 0, alISO = 1, alKOI = 2, alWIN = 3 };

  static int tolower( uchar ch, int cp );
  static int toupper( uchar ch, int cp);

  static int recode( uchar ch, int cp_dst, int cp_src );

private:
  static const char* cp2tbl( int );

};

}; // namespace lwml

#endif // _ALPHABET_
