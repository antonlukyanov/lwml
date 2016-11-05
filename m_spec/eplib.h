// ������ � ���� ������ ��������� �����������
// ��� ���������� ������������� � ����������� procep
// lwml, (c) ltwood

#ifndef _EPLIB_
#define _EPLIB_

#include "defs.h"
#include "vector.h"
#include "matrix.h"
#include "i_vset.h"
#include "cstrng.h"
#include "t_array.h"
#include "t_membuf.h"

/*#lake:stop*/

namespace lwml {

// ���������� ��� ������ ������������� ����������� �����

DEF_EX_TYPE_MSG(ex_integrity, ex_eplib, "error reading eplib");

// epinfo - ������ ���� ������ ��������� ����������� (��)
// ������� �� ������������� ��� ����� � ������� (fname),
// ��� ����������� (name), ���� �������� ���������� (color),
// ����� ��������� (line), ������� ������� ������� � ����� (start)
// � ����� ������ �� (length)
// ������� is_name(), is_color(), is_line() ��������� ���������
// ���������������� ���� ����������� ��������
// ������� is() ��������� ��������� ���� ����� ���������� ���������
// ������� init() �������������� ��� ���� ������ ����������� ����������

class epinfo : public value {
public:
  void init( const char *fnm, const char *nm, const char *col, int ln, int st, int len );

  strng fname() const { return _fname; }
  strng name() const { return _name; }
  strng color() const { return _color; }
  int line() const { return _line; }
  int start() const { return _start; }
  int len() const { return _length; }

  bool is_name( const char* ) const;
  bool is_color( const char* ) const;
  bool is_line( int ) const;

  bool is( const char*, const char*, int ) const;

private:
  strng _fname;
  strng _name, _color;
  int _line;
  int _start, _length;
};

// ����� ��������� ��������� ���� ������ ��������� �����������
// ��� ��������������� ����������� ���� ���� ������
// size() ���� ��������� ����� �� � ���� ������
// ������� min_len() � max_len() ���� ����� ����� ��������
// � ����� ������� ������ �� ���� ������
// ������� name(), color(), line() � len() ���������� ��������������
// ���, ����, ��������� � ����� ������ ��� �� � ���������� �������
// ������� vget() �������� � ���������� ������ ������ � ��������� �������
// ����� ����������� ������� ������ ���� �����������
// ������� count() ������� ����� ��, �������������� "�������"
// ������� mget() �������� ��, �������������� "�������" � ������ �������
//   ������ ��������� ��� �����������, ���� ������� � ����� ���������
//   � ����� ����������� � ����� ������� ����������� '*' �������������
//   ������� ���������
//   ��� ������ ��������� ����������� '*' ������������� �������� -1

class eplib : public value {
public:
  eplib( const char *libname );

  int size() const { return _size; }
  int min_len() const { return _minlen; }
  int max_len() const { return _maxlen; }

  strng name( int k ) const {
    test_index(k, _size);
    return _info[k].name();
  }
  strng color( int k ) const {
    test_index(k, _size);
    return _info[k].color();
  }

  int line( int k ) const {
    test_index(k, _size);
    return _info[k].line();
  }
  int len( int k ) const {
    test_index(k, _size);
    return _info[k].len();
  }

  void vget( vector& dst, int num );

  int  count( const char *name, const char* color, int line );
  void mget( matrix& dst, const char *name, const char *color, int line );

private:
  int                   _size;
  t_array<epinfo>       _info;
  int                   _maxlen, _minlen;
  t_membuf<int16>       _idata; // internal data representation

  void setminmaxlen();
  void seek( int num );
};

// ����� ��������� ��������� i_vector_set
// ����� ��������� �������������� ����, ����������� �� �������
// � ������� ������� ��������� ���������� ������ ��
// � ���� ��������� �������� �� ���� �� ������� � ����������
// ������������� �� ��������� � �������
// ��� ��������������� ���������� �������, � ������� �������
// ��������� �� � ����� ����
// ����� seek() ���������� ���� � ��������� �������

class epwindow : public i_vector_set, public hardcore {
public:
  epwindow( const matrix& src, int len );

  virtual int len() const { return _src->str(); }
  virtual int dim() const { return _len; }
  virtual real get( int j, int d ) const {
    return (*_src)(j, _pos + d);
  }
  virtual void get( vector& v, int j ) const {
    _src->get_str(v, j);
  }

  void seek( int pos );

private:
  int _len;             // ����� ����
  const matrix *_src;   // �������-��������
  int _pos;             // ������� ����
};

}; // namespace lwml

#endif // _EPLIB_
