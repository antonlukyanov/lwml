// Абстрактный интерфейс потока вывода
// lwml, (c) ltwood

#ifndef _I_STREAM_
#define _I_STREAM_

#include "defs.h"
#include "cstrng.h"

/*#lake:stop*/

namespace lwml {

// Класс определяет интерфейс текстового потока вывода.
// В конце файла описаны основные проектные решения, связанные с этим интерфейсом.

class i_stream : public interface {
public:
  virtual void out( const strng& ) = 0;     // вывод строки
  virtual void flush() = 0;                 // выталкивание потока

  // вывод строки
  i_stream& operator<<( const cstrng& s ) { 
    out(s);
    return *this;
  }
};

// Реализация потока вывода в строку

class sstream : public i_stream, public value {
public:
  // вывод строки
  virtual void out( const strng& s ){ _str.concat(s); }

  // выталкивание потока
  virtual void flush() {}

private:
  cstrng _str;
};

}; // namespace lwml

#endif // _I_STREAM_
