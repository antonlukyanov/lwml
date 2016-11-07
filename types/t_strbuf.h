// Simple string buffer
// lwml, (c) ltwood

#ifndef _STRBUF_
#define _STRBUF_

#include "lwml/base/defs.h"
#include "lwml/platforms.h"

/*#lake:stop*/

namespace lwml {

// Простой строковый буфер фиксированной длины.
// Позволяет безопасно (с контролем переполнения) дописывать в конец строки.
// В основном используется для накопления сообщений об ошибках.
// Гарантируется, что операции с этим объектом никогда не генерируют исключений.

template<int LEN>
class strbuf : public value {
public:
  strbuf() { reset(); }

  void reset() { _buf[0] = 0; }

  strbuf& put( const char* );

  int vprintf( const char* fmt, va_list va );
  int printf( const char* fmt, ... ) PRINTF(2, 3);

  char* get() const { return _buf; }

private:
  mutable char _buf[LEN];
};

template<int LEN>
strbuf<LEN>& strbuf<LEN>::put( const char* str )
{
  int sz = strlen(_buf);
  int len = LEN - sz;
  prot_strcpy(_buf + sz, str, len);
  return *this;
}

template<int LEN>
int strbuf<LEN>::vprintf( const char* fmt, va_list va )
{
  int sz = strlen(_buf);
  int len = LEN - sz;
  return prot_vsprintf(_buf + sz, len, fmt, va);
}

template<int LEN>
int strbuf<LEN>::printf( const char* fmt, ... )
{
  va_list va;
  va_start(va, fmt);
  int sz = strlen(_buf);
  int len = LEN - sz;
  int res = prot_vsprintf(_buf + sz, len, fmt, va);
  va_end(va);
  return res;
}

}; // namespace lwml

#endif // _STRBUF_
