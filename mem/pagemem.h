// ���������� �������� ������
// lwml, (c) ltwood

#ifndef _PAGEMEM_
#define _PAGEMEM_

//!! TODO: �������� ����� free

#include "defs.h"
#include "basex.h"
#include "memman.h"
#include "stdmem.h"

/*#lake:stop*/

namespace lwml {

// ���������� ��� ��������� � �������

DEF_EX_TYPE(ex_base, ex_pagemem, "can't allocate memory page");

// ���������� �������� ������, ����������� �����������
// ��������� i_memman ����������� ��������� ���������
// ������ �� ������� ��������������� ����� ����� ������
// ��� ��������������� ����������� ������ �������� � "���������"
// � ������ "��������" � ������.
// ������������ ������ ������, �� �� ������������ ��������
// ������������ ����������� �������.
// ������������ ��� �������� ��������� ������ ��� ������
// �������, ���� ��� ��� ������ ����� ������� ������������
// ������� size() ���������� ������ ������ ������, �������
// ����������, ������� ������ �� ������ ������� � ����������������
// ������������ �� ������� ��������
// ������� leak() ���������� ��������� ������ ������, ����������
// �� ���� ������ �� ������ �������

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
  uint _pagesize;         // ������ ��������
  char *_headpage;        // ��������� �� ������ ��������
  uint _pagenum;          // ����� ����� �������
  uint _pageused;         // ������������ � ������ ��������
  uint _leaksize;         // ��������� ������ �������

  void create_newpage();
};

}; // namespace lwml

#endif // _PAGEMEM_
