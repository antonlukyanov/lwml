// ������������ �������
// lwml, (c) ltwood

#ifndef _MATRIX_
#define _MATRIX_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"
#include "basex.h"
#include "i_vset.h"
#include "lwmlconf.h"
#include "stream.h"

/*#lake:stop*/

namespace lwml {

// ���������� ��� ������ ������������� ����������� �����

DEF_EX_TYPE_MSG(ex_integrity, ex_matrix, "error reading matrix");

// ������� ������������ �����.

typedef class real_matrix matrix;

class real_matrix : public value {
public:
  // ��������������� ������ �������.
  real_matrix();

  // ��������������� �� ��������.
  real_matrix( int, int );

  // ��������������� �� �������� � ���������� ��������.
  real_matrix( int, int, real val );

  // ��������������� �� �����, �� �������� ������������ ������ �������.
  // ���� ������ ��������� ��������� ���� LMATR ��� RMATR.
  explicit real_matrix( referer<stream> file );

  // ��������������� �� ����� �����, �� �������� ������������ ������ �������.
  // ���� ������ ��������� ��������� ���� LMATR ��� RMATR.
  explicit real_matrix( const char* );

  real_matrix( const real_matrix& );

  virtual ~real_matrix();

  void resize( int m, int n );
  void resize( int m, int n, real val );

  real_matrix& operator=( const real_matrix& src );

  // ��������� �������� �������.
  int str() const { return _str; }
  int col() const { return _col; }

  // �������������� ��������� �������.
  real& operator()( int i, int j ) {
    TEST_INDEX2(i, _str, j, _col);
    return _data[i][j];
  }
  const real& operator()( int i, int j ) const {
    TEST_INDEX2(i, _str, j, _col);
    return _data[i][j];
  }

  // �������������� ��� �������� ��������.
  // ��� ���������� � �������� ����������.
        real& at( int i, int j )       { return _data[i][j]; }
  const real& at( int i, int j ) const { return _data[i][j]; }

  // ���������� ������� � ��������� �����.
  // � ������ ������ ����������� ����� ����� � ��������,
  // � ����� - �������� ��������� ������� �� �������.
  // ����� ������ �������� ������������� ����� (prec).
  // �������� nf ������ ����� ������ ����� ��� ������.
  // ��� ������ � ������� nfSCNT �������� ���������� ��� ����� �������� ����,
  // � ��� ������ � ������� nfFIXED -- ��� ����� ���� ����� �������.

  // ���������� ������� � ��������� �����.
  void save( referer<stream> file, int prec = 6, real_format nf = nfSCNT ) const;

  // ���������� ������� � ��������� ����� � �������� ������.
  void save( const char *name, int prec = 6, real_format nf = nfSCNT ) const;

  // ����������� � �������� ����������� i0, j0 ������� �������
  // ���������� ������ �������, �������� �������.
  // ���������� �������� ����������� ����������, ��������� � ��������� val.
  void copy( const real_matrix& src, real val = 0.0, int i0 = 0, int j0 = 0 );

  // �������� �� ���������� ������� ������������� �������,
  // ������� �������� ��������� � ��������� ������ �������.
  // ��������� ������� ������ ��������� ������ �������� ���� ����������� �������.
  void copy_sub( const real_matrix& src, int, int );

  void set_val( real val );  // ��������� ���� ��������� �������� val
  void set_zero();           // ��������� ���� ��������� ������� ��������
  void set_diag( real x );   // ������� ������� ������������ � ���������� x �� ���������
  void swap( int k, int j ); // �������� ������� ������
  void tran();               // ��������������� �������
  void center();             // ������������ �������� �������

  real mag() const;          // ����� ��������� ���������
  real norm() const;         // ������ �� ����� ��������� ���������
  real norm_manh() const;    // �����, ������������� � ������������� ��� ��������
  real trace() const;        // ���� ������� (����� ������������ ���������)
  real min() const;          // ����������� �������
  real max() const;          // ������������ �������
  real min_abs() const;      // ����������� �� ������ �������
  real max_abs() const;      // ������������ �� ������ �������
  real inner_sum() const;    // ����� ���������
  real mid() const;          // ������� �������� ���������

  // ��������� ���������� ����� ����� ���������.
  static real dist( const real_matrix&, const real_matrix& );

  // ������� �������������� ��������� ����������
  real_matrix& operator+=( const real_matrix& );
  real_matrix& operator-=( const real_matrix& );
  real_matrix& operator*=( real );
  real_matrix& operator/=( real );

  // ����������� �������� � ����� ����� ��������� � ���������.
  void get_col( vector&, int c ) const;
  void get_str( vector&, int s ) const;
  void put_col( const vector&, int c );
  void put_str( const vector&, int s );

  // ���������� ������� �� �������-��������.
  void inc_mul( const real_matrix& );

  // ���������� ������� �� ����������������� �������-��������.
  void inc_mul_tran( const real_matrix& );

  // ������������ ���� ������.
  static void mul( real_matrix& dst, const real_matrix& src1, const real_matrix& src2 );

  // ��������� ������� �� ����������������� �������.
  static void mul_tran( real_matrix& dst, const real_matrix& src1, const real_matrix& src2 );

  // ���������� ��������� ������������ �������� ����� �������.
  real inner_mul( int j1, int j2 );

  // ��������������� ����� �������.
  static void gramm( real_matrix& a );

private:
  int _str, _col;
  real **_data;
  real *_repr;

  void alloc( int, int );
  void dealloc();

  void read_data( referer<stream> file, const char* fname );
};

// ���������� ���������� i_vector_set �� ���� �������
// ���������, ��� ������ ������ ������� ������ ���� ������

class matrix_vector_set : public i_vector_set, public hardcore {
public:
  matrix_vector_set( const matrix& lbl ){
    _data = &lbl;
  }

  virtual int len() const { return _data->str(); }
  virtual int dim() const { return _data->col(); }
  virtual real get( int j, int d ) const { return (*_data)(j, d); }
  virtual void get( vector& v, int j ) const { _data->get_str(v, j); }

private:
  const matrix *_data;
};

//!! TODO: ������������

// void minittakens( real_matrix*, vector&, int winlen, int shift );

// real_matrix::gramm():
// ��������� ������� ��������������� ������-������
// k- ������ �������� ������� ����������� k-� ������
//
// e_k = f_k - sum_{j=1}^{n-1} L_j e_j
// L_j = (f_k,e_j)/(e_j,e_j)

}; // namespace lwml

#endif // _MATRIX_
