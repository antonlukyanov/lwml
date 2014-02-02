// Intel Hardware RNG
// lwml, (c) ltwood

#ifndef _IHRNG_
#define _IHRNG_

#include "defs.h"
#include "mdefs.h"
#include "basex.h"
#include "refcount.h"
#include "stdmem.h"

/*#lake:stop*/

namespace lwml {

// ������ ������������� Hardware Random Number Generator

DEF_EX_TYPE(ex_base, ex_ihrng, "intel hardware RNG error");

// ���������� ������� � Intel Hardware Random Number Generator ����� win32 CryptoAPI.
// ��� Intel HRNG ������ � Firmware Hub, ������� ������������ � ��������
// Intel(R) 810, 815, 820, 840 Chipset family.
// ������� ����� ������� ���������� Intel(R) Security Driver � ��� ���������
// ��������� Intel(R) Hardware Cryptographic Service Provider (CSP).

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
