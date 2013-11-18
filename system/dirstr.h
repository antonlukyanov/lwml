// �����, ��������� � �����������
// lwml, (c) ltwood

#ifndef _DIRSTR_
#define _DIRSTR_

#include "defs.h"
#include "cstrng.h"
#include "stdmem.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// ���������� ��� ������������� ������� ����������

DEF_EX_TYPE_MSG(ex_base, ex_dirstr, "can't open dirstr");

// ����� ������������� ����� ������ ����������� ���������� (dirent).
// ��� �������� ������ ����� ���������� ������ ������ ������,
// � � ���������� �������������� ������������ ������, � �������
// ������������ ������ ���������� �������� ����������.
// �������� ������������� �������� �����:
//   for( dirstr dir(path); !dir.is_end(); dir.next() )
//     process(dir.get());

class dirstr : public refcount {
private:
  dirstr( const char* dn );
  virtual ~dirstr();

public:
  // ��������� ����� ������ ���������� dn.
  static referer<dirstr> create( const char* dn ){
    return referer<dirstr>(new(lwml_alloc) dirstr(dn));
  }

  // ������� �� ������ ������.
  void rewind();

  // ������� � ���������� �������� ������.
  // ���������� ���� ���������� ������.
  bool next() { return read(); }

  // ��������� �������� �������� ������.
  // ���������� ������ � ������ ��������� �������������.
  strng get() const;

  // �������� �� ����� ������.
  // ���������� true ���� ������ ������������� ��������.
  bool is_end() const;

private:
  void* _dir;    // DIR*
  void* _dirent; // struct dirent *

  bool read();
};

}; // namespace lwml

#endif // _DIRSTR_
