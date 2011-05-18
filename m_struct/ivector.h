// ������������� ������
// lwml, (c) ltwood

#ifndef _IVECTOR_
#define _IVECTOR_

#include "defs.h"
#include "basex.h"
#include "lwmlconf.h"
#include "stream.h"

/*#lake:stop*/

namespace lwml {

// ���������� ��� ������ ������������� ����������� �����

DEF_EX_TYPE_MSG(ex_integrity, ex_ivector, "error reading int_vector");

class int_vector : public value {
public:
  // ��������������� �� �����.
  explicit int_vector( int len = 0 );

  // ��������������� �� ����� � ���������� �������� ���������.
  int_vector( int len, int val );

  // ��������������� �� ����� � ������������ �������, �� ��������
  // ����� ����� �������� �������� �������.
  int_vector( int len, const int* );

  // ��������������� �� �����, �� �������� ������������ ������ �������.
  // ���� ������ ��������� ��������� ���� LVECT.
  explicit int_vector( referer<stream> file );

  // ��������������� �� ����� �����, �� �������� ������������ ������ �������.
  // ��� ���������� ��������� ���� LVECT ������������ ������ "������" ���������� �����.
  explicit int_vector( const char* );

  int_vector( const int_vector& );
  virtual ~int_vector();

  int_vector& operator=( const int_vector& src );

  int len() const { return _len; }              // ��������� ����� �������.

  void resize( int len ) { realloc(len); }

  int& operator[]( int j ) {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[j];
  }
  const int& operator[]( int j ) const {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[j];
  }

  // ���������� ������� � ��������� �����.
  void save( referer<stream> file ) const;

  // ���������� ������� � ��������� ����� � �������� ������.
  void save( const char *name ) const;

  // ���������� ������� � ��������� ����� � �������� ������ ��� ���������.
  void export_as_text( const char *name ) const;

  // ����������� � �������� ���������� idx0 ������� �������
  // ���������� ������� ������� ������� �����.
  // ��� ���� ���������� ������� ������� ����������� ����������,
  // ��������� � ��������� val.
  void copy( const int_vector& src, int val = -1, int idx0 = 0 );

  // �������� �� ����������� ������� ������� ����� len().
  // �������� idx ������ ��������� ������ ����������� �������.
  void copy_sub( const int_vector& src, int idx );

  void set_val( int val );     // ��������� ���� �������� �������� val
  void set_zero();             // �������� ��� ��������
  void swap( int k, int j );   // �������� k-� � j-� �������� �������
  void revers();               // �������� ������� ��������� �� ��������

  int min() const;             // ����� ����������� �������
  int max() const;             // ����� ������������ �������
  int min_idx() const;         // ����� ������ ������������ ��������
  int max_idx() const;         // ����� ������ ������������� ��������
  int min_abs() const;         // ����������� �� ������ �������
  int max_abs() const;         // ������������ �� ������ �������
  int min_abs_idx() const;     // ������ ������������ �� ������ ��������
  int max_abs_idx() const;     // ������ ������������� �� ������ ��������
  int inner_sum() const;       // ����� ���������

  int comp( int i, int j ) const 
  {
    return _data[i] - _data[j];
  }

private:
  int _len;
  int *_data;

  void alloc( int );
  void dealloc();
  void realloc( int len );

  void read_raw_data( const char* fname );
  bool read_data( referer<stream> file, const char* fname );
};

}; // namespace lwml

#endif // _IVECTOR_
