// ������������� �������
// lwml, (c) ltwood

#ifndef _IMATRIX_
#define _IMATRIX_

#include "defs.h"
#include "ivector.h"
#include "basex.h"
#include "lwmlconf.h"
#include "stream.h"

/*#lake:stop*/

namespace lwml {

// ���������� ��� ������ ������������� ����������� �����

DEF_EX_TYPE_MSG(ex_integrity, ex_imatrix, "error reading int_matrix");

class int_matrix : public value {
public:
  // ��������������� ������ �������.
  int_matrix();

  // ��������������� �� ��������.
  int_matrix( int, int );

  // ��������������� �� �������� � ���������� �������� ���������.
  int_matrix( int, int, int val );

  // ��������������� �� �����, �� �������� ������������ ������ �������.
  // ���� ������ ��������� ��������� ���� LMATR.
  explicit int_matrix( referer<stream> file );

  // ��������������� �� ����� �����, �� �������� ������������ ������ �������.
  // ���� ������ ��������� ��������� ���� LMATR.
  explicit int_matrix( const char* );

  int_matrix( const int_matrix& );

  virtual ~int_matrix();

  void resize( int m, int n );
  void resize( int m, int n, int val );

  int_matrix& operator=( const int_matrix& src );

  // ��������� �������� �������.
  int str() const { return _str; }
  int col() const { return _col; }

  int& operator()( int i, int j ) {
    TEST_INDEX2(i, _str, j, _col);
    return _data[i][j];
  }
  const int& operator()( int i, int j ) const {
    TEST_INDEX2(i, _str, j, _col);
    return _data[i][j];
  }

  // �������������� ��� �������� ��������.
  // ��� ���������� � �������� ����������.
        int& at( int i, int j )       { return _data[i][j]; }
  const int& at( int i, int j ) const { return _data[i][j]; }

  // ���������� ������� � ��������� �����.
  void save( referer<stream> file ) const;

  // ���������� ������� � ��������� ����� � �������� ������.
  void save( const char *name ) const;

  // ����������� � �������� ����������� i0, j0 ������� �������
  // ���������� ������ �������, �������� �������.
  // ���������� �������� ����������� ����������, ��������� � ��������� val.
  void copy( const int_matrix& src, int val = -1, int i0 = 0, int j0 = 0 );

  // �������� �� ���������� ������� ������������� �������, ������� ��������
  // ��������� � ��������� ������ �������.
  // ��������� i0, j0 ������ ��������� ������ �������� ���� ����������� �������.
  void copy_sub( const int_matrix& src, int i0, int j0 );

  void set_val( int val );   // ��������� ���� ��������� �������� val
  void set_zero();           // ��������� ���� ��������� ������� ��������
  void set_diag( int x );    // ������� ������� ������������ � ���������� x �� ���������
  void swap( int k, int j ); // �������� ������� ������
  void tran();               // ��������������� �������

  int min() const;           // ����������� �������
  int max() const;           // ������������ �������
  int min_abs() const;       // ����������� �� ������ �������
  int max_abs() const;       // ������������ �� ������ �������
  int inner_sum() const;     // ����� ���������

  // ����������� ������� ��� ������ ������� � ���������� ������.
  void get_col( int_vector&, int c ) const;
  void get_str( int_vector&, int s ) const;

  // ����������� ���������� ����������� ������� � �������� ������� ��� ������ �������.
  void put_col( const int_vector&, int c );
  void put_str( const int_vector&, int s );

private:
  int _str, _col;
  int **_data;
  int *_repr;

  void alloc( int m, int n );
  void dealloc();

  void read_data( referer<stream> file, const char* fname );
};

}; // namespace lwml

#endif // _IMATRIX_
