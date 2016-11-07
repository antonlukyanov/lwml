// Intel Hardware RNG
// lwml, (c) ltwood

#ifndef _IHRNG_
#define _IHRNG_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/base/basex.h"
#include "lwml/base/refcount.h"
#include "lwml/memory/stdmem.h"

/*#lake:stop*/

namespace lwml {

// Ошибка инициализации Hardware Random Number Generator

DEF_EX_TYPE(ex_base, ex_ihrng, "intel hardware RNG error");

// Реализация доступа к Intel Hardware Random Number Generator через win32 CryptoAPI.
// Сам Intel HRNG входит в Firmware Hub, который присутствует в чипсетах
// Intel(R) 810, 815, 820, 840 Chipset family.
// Драйвер этого датчика называется Intel(R) Security Driver и при установке
// добавляет Intel(R) Hardware Cryptographic Service Provider (CSP).

class ihrng : public refcount {
private:
  ihrng();
  ~ihrng();

public:
  static referer<ihrng> create() {
    return referer<ihrng>(new(lwml_alloc) ihrng());
  }

  void get_data( void* buf, int len );

private:
  uint32 _prov;
};

}; // namespace lwml

#endif // _IHRNG_
