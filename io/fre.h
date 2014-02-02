// Файловые регулярные выражения.
// lwml, (c) ltwood

#ifndef _FRE_
#define _FRE_

#include "defs.h"

/*#lake:stop*/

namespace lwml {

// Сопоставление имени файла fn с файловой маской re.
// Файловая маска представляет собой подмножество
// файловых масок Posix (исключены множества символов).
// По сравнению с соглашениями DOS/Win здесь допускается
// присутствие в регулярном выражении нескольких символов '*'.
// Сравнение производится без учета регистра символов.

class fre : public scope {
public:
  static bool is_match( const char* re, const char* str );

private:
  static bool matchstar( const char* re, const char* str );
};

}; // namespace lwml

#endif // _FRE_
