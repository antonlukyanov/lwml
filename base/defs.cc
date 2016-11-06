#include "defs.h"

/*#lake:stop*/

namespace lwml {

// Функция strncpy(dst, src, num) копирует не более num символов,
// заполняя нулями остаток строки dst.
// Если исходная строка длиннее num, то будет скопировано ровно
// num символов и не будет записан завершающий нуль.

char* prot_strcpy( char* dst, const char* src, int buflen )
{
  if( src ){
    strncpy(dst, src, buflen-1);
    dst[buflen-1] = 0;  // дописываем завершающий нуль
  } else
    dst[0] = 0; // формируем строку нулевой длины

  return dst;
}

// Функция snprintf может записать ровно buflen символов и вернуть число buflen.
// В этом случае считается, что произошло переполнение буфера и следует использовать
// буфер большей длины.
// В функции prot_vsprintf() при переполнении происходит простое обрезание результата
// и возвращается признак ошибки.
int prot_vsprintf( char* buf, int buflen, const char* fmt, va_list va )
{
  int numch;
#if defined(_MSC_VER)  
  numch = _vsnprintf(buf, buflen, fmt, va);
#else
  numch = vsnprintf(buf, buflen, fmt, va);
#endif

  if( numch >= 0 && numch < buflen )
    return numch;
  // при переполнении буфера
  buf[buflen-1] = 0;  // записываем завершающий нуль
  return -1;          // возвращаем признак ошибки
}

int prot_sprintf( char* buf, int buflen, const char* fmt, ... )
{
  va_list va;
  va_start(va, fmt);
  //!! SIC: prot_vsprintf() не генерирует исключений!
  int numch = prot_vsprintf(buf, buflen, fmt, va);
  va_end(va);
  return numch;
}

// lwml version

namespace {
  volatile char _lwml_copy[] =
    "\rlwml, ver. 8.02, (c) ltwood, 1990--2005, mailto:ltwood@gmail.com\r";

  const char _lwml_ver[] = "8.02";
};

const char* lwml_ver()
{
  _lwml_copy[0] = '\n';
  return _lwml_ver;
}

}; // namespace lwml
