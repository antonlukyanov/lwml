// ������ �����-������
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

// ���������� ��� ������ �������� �����

DEF_EX_TYPE_MSG(ex_base, ex_fopen, "can't open file");

// ���������� ��� ������ ������ ����������� �����

DEF_EX_TYPE(ex_base, ex_fread, "can't read data from file");

// ������ �������� �����

const int fmREAD    = 0x0000;  // ������ ������������� �����
const int fmWRITE   = 0x0001;  // ������ (������������ ���� ����������������)
const int fmAPPEND  = 0x0002;  // ����������� � ����� (��� ���������� ���� ���������)
const int fmRDWR_EX = 0x0010;  // ������-������ ������������� �����
const int fmRDWR_CR = 0x0011;  // �������� ��� ������-������ (������������ ���� ����������������)
const int fmRDWR_UP = 0x0012;

const int fmTEXT    = 0x0100;  // ��������� �����
const int fmBINARY  = 0x0200;  // ��������� �����

// ������� ������ FILE*.

class stream : public refcount {
private:
  stream();
  stream( const char* name, int wrmode, int btmode );

  virtual ~stream();

public:
  // �������� ���������� ��������� �����
  static referer<stream> create() {
    return referer<stream>(new(lwml_alloc) stream());
  }

  // �������� ����� � ������ name � ������ mode
  static referer<stream> create( const char* name, int wrmode, int btmode ){
    return referer<stream>(new(lwml_alloc) stream(name, wrmode, btmode));
  }

  FILE* file() { return _file; }        // �������� FILE* ��� �����
  int id();                             // �������� ���������� �����

  int size();                           // �������� ������ �����
  int tell();                           // �������� ������� � �����
  void rewind();                        // ������ �� ������ �����
  void seek( int pos );                 // ������� � ������� pos
  void skip( int ofs );                 // ���������� ofs ������
  void flush();                         // �������� ���� �� ����

  // ��������� �� ����� len ���� � �������� �� �� ��������� ptr
  void read( void* ptr, int len );

  // �������� � ���� len ���� �� ������ �� ��������� ptr
  void write( const void* ptr, int len );

  // ��������� �����.
  // �������� ������ � ���������� �������.
  void printf( const char* fmt, ... ) PRINTF(2,3);

  // ������ � �������� ����� "�����" ������ � ����������� �����.
  // ���������� ���������� ��� ������� ������ � ��������� �����.
  void put_str( const char* );

  // ��������� ������ �� ������.
  // �� ��������� ������ ������ ������, ������������ ����� ��� ������ �����,
  // ��� ���������� -- ������������ ��������� ������ ��� ������ �����.
  // ��� ������������� �������� ������ ���������� false.
  bool get_str( cstrng& );

  // ��������� ����� �����
  int count_lines();

private:
  FILE *_file;
  int _is_text;

  strng mkmode( int mode ) const;
};

// ��������� ����� ������

class fstream : public i_stream, public value {
public:
  fstream( const char* name ){
    _str = stream::create(name, fmWRITE, fmTEXT);
  }

  fstream( referer<stream> str ) : _str(str) {}

  // ����� ������
  virtual void out( const strng& s ) { _str->printf("%s", s.ascstr()); }

  // ������������ ������
  virtual void flush() { _str->flush(); }

private:
  referer<stream> _str;
};

}; // namespace lwml

#endif // _STREAM_
