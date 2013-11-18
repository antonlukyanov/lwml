#include "ihrng.h"

/*#lake:stop*/

namespace lwml {

#include <windows.h>
#include <wincrypt.h>

// Константы из файла icsp4ms.h из поставки Intel(R) Security Driver

//// const int PROV_INTEL_SEC = 22; //!! NB: Эта константа уже присутствует в wincrypt.h

const char INTEL_DEF_PROV[] = "Intel Hardware Cryptographic Service Provider";

ihrng::ihrng()
{
//  if( CryptAcquireContext(&_prov, NULL, NULL, PROV_RSA_FULL, 0) == 0 )
//    throw ex_ihrng();
  if( CryptAcquireContext(&_prov, 0, INTEL_DEF_PROV, PROV_INTEL_SEC, 0) == 0 )
    throw ex_ihrng();
}

ihrng::~ihrng()
{
  if( CryptReleaseContext(_prov, 0) == 0 )
    throw ex_ihrng();
}

void ihrng::get_data( void* buf, int len )
{
  if( CryptGenRandom(_prov, len, reinterpret_cast<BYTE*>(buf)) == 0 )
    throw ex_ihrng();
}

}; // namespace lwml
