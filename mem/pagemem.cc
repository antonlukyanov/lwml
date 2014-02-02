#include "pagemem.h"
#include "stream.h"
/*#lake:stop*/

namespace lwml {

// page memman

typedef char* LINK;

#define PAGELINK(pageptr) (*reinterpret_cast<LINK*>(pageptr))
#define PAGEDATA(pageptr) (static_cast<char*>(pageptr)+sizeof(LINK))

void page_memman::create_newpage()
{
  if( _headpage != 0 )
    _leaksize += _pagesize - _pageused;
  char* newpage = static_cast<char*>(::malloc(_pagesize + sizeof(LINK)));
  if( newpage == 0 )
    throw ex_pagemem();
  PAGELINK(newpage) = _headpage;
  _headpage = newpage;
  _pagenum++;
  _pageused = 0;
}

page_memman::page_memman( uint el_in_pg, uint el_sz )
  : _pagesize(el_in_pg * el_sz)
{
  _leaksize = 0;
  _headpage = 0;
  _pagenum = 0;
  create_newpage();
}

page_memman::~page_memman()
{
  char* page = _headpage;
  while( page ){
    char* next = PAGELINK(page);
    mem::free(page);
    page = next;
  }
}

void* page_memman::alloc( uint size )
{
  if( size > _pagesize )
    runtime("too long block for current page size (%d/%d)", size, _pagesize);

  uint freesize = _pagesize - _pageused;
  if( size > freesize )
    create_newpage();
  char* res = PAGEDATA(_headpage) + _pageused;
  _pageused += size;
  return res;
}

void page_memman::dump( const char *fn ) const
{
  referer<stream> f = stream::create(fn, fmWRITE, fmBINARY);
  for( char * page = _headpage; page; page = PAGELINK(page) )
    f->write(PAGEDATA(page), _pagesize);
}


}; // namespace lwml
