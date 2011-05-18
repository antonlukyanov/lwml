// Обобщенный автоматический хранитель указателя
// (c) ltwood

#ifndef _REFOWNER_
#define _REFOWNER_

#include "defs.h"

/*#lake:stop*/

namespace lwml {

// Автоматический хранитель указателя гарантирует вызов
// деструктора при выходе из области видимости.
// Указатель передается хранителю при конструировании или с помощью функции reset().
// В отличие от auto_ptr не поддерживает передачу собственности
// путем определения конструктора копирования и оператора копирования.
// Вместо этого конструктор и оператор копирования сделаны приватными.

template <typename T>
class refowner : public hardcore {
public:
  explicit refowner( T* ptr = 0 ) : _ptr(ptr) {}

  ~refowner() { delete _ptr; }

  // обеспечиваем обычную семантику указателей
  //!! SIC: эти операторы возвращают неконстантный объект, будучи сами константными!
  T& operator*()  const { return *_ptr; }
  T* operator->() const { return _ptr; }

  // даем явный доступ к указателю
  T* get() const { return _ptr; }

  // убить хранимый объект и принять новый
  void reset( T* ptr = 0 ) {
    if( ptr != _ptr ){
      delete _ptr;
      _ptr = ptr;
    }
  }

  // отдать хранимый объект (передает владение)
  T* release() {
    T* tmp = _ptr;
    _ptr = 0;
    return tmp;
  }

  // Проверка инициализированности указателя.
  bool is_ok() const { return _ptr != 0; }

private:
  T* _ptr;
};

}; // namespace lwml

#endif // _REFOWNER_
