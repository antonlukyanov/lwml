// Потоки ввода-вывода
// lwml, (c) ltwood

#ifndef _STREAM_
#define _STREAM_

#include "defs.h"
#include "mdefs.h"
#include "cstrng.h"
#include "stdmem.h"
#include "refcount.h"
#include "i_stream.h"
#include "platforms.h"

/*#lake:stop*/

namespace lwml {

// Исключение при ошибке открытия файла

DEF_EX_TYPE_MSG(ex_base, ex_fopen, "can't open file");

// Исключение при ошибке чтения содержимого файла

DEF_EX_TYPE(ex_base, ex_fread, "can't read data from file");

// режимы открытия файла

const int fmREAD    = 0x0000;  // чтение существующего файла
const int fmWRITE   = 0x0001;  // запись (существующий файл перезаписывается)
const int fmAPPEND  = 0x0002;  // дописывание в конец (при отсутствии файл создается)
const int fmRDWR_EX = 0x0010;  // чтение-запись существующего файла
const int fmRDWR_CR = 0x0011;  // создание для чтения-записи (существующий файл перезаписывается)
const int fmRDWR_UP = 0x0012;

const int fmTEXT    = 0x0100;  // текстовый режим
const int fmBINARY  = 0x0200;  // текстовый режим

// Обертка вокруг FILE*.

class stream : public refcount {
private:
  stream();
  stream( const char* name, int wrmode, int btmode );

  virtual ~stream();

public:
  // Создание временного бинарного файла
  static referer<stream> create() {
    return referer<stream>(new(lwml_alloc) stream());
  }

  // Открытие файла с именем name в режиме mode
  static referer<stream> create( const char* name, int wrmode, int btmode ){
    return referer<stream>(new(lwml_alloc) stream(name, wrmode, btmode));
  }

  FILE* file() { return _file; }        // получить FILE* для файла
  int id();                             // получить дескриптор файла

  int size();                           // получить размер файла
  int tell();                           // получить позицию в файле
  void rewind();                        // встать на начало файла
  void seek( int pos );                 // перейти в позицию pos
  void skip( int ofs );                 // пропустить ofs байтов
  void flush();                         // сбросить файл на диск

  // прочитать из файла len байт и записать их по указателю ptr
  void read( void* ptr, int len );

  // записать в файл len байт из памяти по указателю ptr
  void write( const void* ptr, int len );

  // Форматный вывод.
  // Работает только с текстовыми файлами.
  void printf( const char* fmt, ... ) PRINTF(2,3);

  // Запись в двоичный поток "сырой" строки с завершающим нулем.
  // Генерирует исключение при попытке записи в текстовый поток.
  void put_str( const char* );

  // Получение строки из потока.
  // Из бинарного потока читает строку, ограниченную нулем или концом файла,
  // для текстового -- ограниченную переводом строки или концом файла.
  // При невозможности получить строку возвращает false.
  bool get_str( cstrng& );

  // посчитать число строк
  int count_lines();

private:
  FILE *_file;
  int _is_text;

  strng mkmode( int mode ) const;
};

// Текстовый поток вывода

class fstream : public i_stream, public value {
public:
  fstream( const char* name ){
    _str = stream::create(name, fmWRITE, fmTEXT);
  }

  fstream( referer<stream> str ) : _str(str) {}

  // вывод строки
  virtual void out( const strng& s ) { _str->printf("%s", s.ascstr()); }

  // выталкивание потока
  virtual void flush() { _str->flush(); }

private:
  referer<stream> _str;
};

}; // namespace lwml

#endif // _STREAM_
