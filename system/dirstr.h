// Поток, связанный с директорией
// lwml, (c) ltwood

#ifndef _DIRSTR_
#define _DIRSTR_

#include "lwml/base/defs.h"
#include "lwml/types/cstrng.h"
#include "lwml/memory/stdmem.h"
#include "lwml/base/refcount.h"

/*#lake:stop*/

namespace lwml {

// Исключение при невозможности открыть директорию

DEF_EX_TYPE_MSG(ex_base, ex_dirstr, "can't open dirstr");

// Класс инкапсулирует поток чтения содержимого директории (dirent).
// При создании потока сразу происходит чтение первой записи,
// и в дальнейшем поддерживается актуальность буфера, в который
// производится чтение очередного элемента директории.
// Сценарий использования примерно такой:
//   for( dirstr dir(path); !dir.is_end(); dir.next() )
//     process(dir.get());

class dirstr : public refcount {
private:
  dirstr( const char* dn );
  virtual ~dirstr();

public:
  // Открывает поток чтения директории dn.
  static referer<dirstr> create( const char* dn ){
    return referer<dirstr>(new(lwml_alloc) dirstr(dn));
  }

  // Переход на начало потока.
  void rewind();

  // Переход к следующему элементу потока.
  // Возвращает флаг успешности чтения.
  bool next() { return read(); }

  // Получение текущего элемента потока.
  // Возвращает строку с именем очередной поддиректории.
  strng get() const;

  // Проверка на конец потока.
  // Возвращает true если список поддиректорий исчерпан.
  bool is_end() const;

private:
  void* _dir;    // DIR*
  void* _dirent; // struct dirent *

  bool read();
};

}; // namespace lwml

#endif // _DIRSTR_
