// Страничный менеджер памяти
// lwml, (c) ltwood

#ifndef _PAGEMEM_
#define _PAGEMEM_

//!! TODO: добавить метод free

#include "lwml/base/defs.h"
#include "lwml/base/basex.h"
#include "lwml/memory/memman.h"
#include "lwml/memory/stdmem.h"

/*#lake:stop*/

namespace lwml {

// Исключение при проблемах с памятью

DEF_EX_TYPE(ex_base, ex_pagemem, "can't allocate memory page");

// Страничный менеджер памяти, реализующий абстрактный
// интерфейс i_memman посредством стекового выделения
// памяти из заранее распределенного блока сырой памяти
// При конструировании указывается размер страницы в "элементах"
// и размер "элемента" в байтах.
// Поддерживает список блоков, но не поддерживает операции
// освобождение выделенного участка.
// Используется при массовом выделении памяти под мелкие
// объекты, если всю эту память можно удалять одновременно
// Функция size() возвращает полный размер памяти, занятой
// менеджером, включая утечки на концах страниц и нераспределенное
// пространство на текущей странице
// Функция leak() возвращает суммарный размер памяти, потерянной
// за счет утечек на концах страниц

class page_memman : public i_memman { // public refcount
private:
  page_memman( uint el_in_pg, uint el_sz );

public:
  static referer<page_memman> create( uint el_in_pg, uint el_sz ) {
    return referer<page_memman>(new(lwml_alloc) page_memman(el_in_pg, el_sz));
  }

  virtual ~page_memman();

  virtual void* alloc( uint size );

  virtual void free( void* ) {}

  int pages() const { return _pagenum; }
  int size()  const { return _pagesize * _pagenum; }
  int leak()  const { return _leaksize; }

  void dump( const char *fn ) const;

private:
  uint _pagesize;         // размер страницы
  char *_headpage;        // указатель на первую страницу
  uint _pagenum;          // общее число страниц
  uint _pageused;         // использовано в первой странице
  uint _leaksize;         // суммарный размер хвостов

  void create_newpage();
};

}; // namespace lwml

#endif // _PAGEMEM_
