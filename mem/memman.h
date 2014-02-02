// ��������� ������
// lwml, (c) ltwood

#ifndef _MEMMAN_
#define _MEMMAN_

#include "defs.h"
#include "stdmem.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// ����������� ��������� ����������� ��������� ������
// ���� ��������� ����������� ����������� �����������
// ������� strdup() �� �������� ����������� � �������� ����������.

class i_memman : public interface, public refcount {
public:
  void* alloc( uint el_num, uint el_sz ){
    return alloc(el_num * el_sz);
  }

  virtual void* alloc( uint size ) = 0;
  virtual void  free( void* ) = 0;

  char* strdup( const char* );
};

// ������� ���������� ������� ����� ����������� ��������
// ����� �����, ����������� ��������� ���� ��������

#define MM_NEW( mm, type )                                      \
  static_cast<type*>((mm)->alloc(sizeof(type)))                 //
#define MM_ARRAY( mm, type, size )                              \
  static_cast<type*>((mm)->alloc((size) * sizeof(type)))        //
#define MM_FREE( mm, ptr )                                      \
  (mm)->free( ptr )                                             //

// ���������� �������� ������, ����������� �����������
// ��������� i_memman ����������� ������� �� ������ mem

class std_memman : public i_memman { // public refcount
private:
  std_memman() {}

public:
  static referer<std_memman> create() {
    return referer<std_memman>(new(lwml_alloc) std_memman());
  }

  virtual void* alloc( uint size ) { return mem::alloc(size); }
  virtual void  free( void *ptr )  { mem::free(ptr); }
};

}; // namespace lwml

#endif // _MEMMAN_
