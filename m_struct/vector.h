// ������������ ������
// lwml, (c) ltwood

#ifndef _VECTOR_
#define _VECTOR_

#include "defs.h"
#include "mdefs.h"
#include "basex.h"
#include "lwmlconf.h"
#include "stream.h"

/*#lake:stop*/

namespace lwml {

// ���������� ��� ������ ������������� ����������� �����

DEF_EX_TYPE_MSG(ex_integrity, ex_vector, "error reading vector");

typedef class real_vector vector;

class real_vector : public value {
public:
  // ��������������� �� �����.
  explicit real_vector( int len = 0 );

  // ��������������� �� ����� � ���������� �������� ���������.
  real_vector( int len, real val );

  // ��������������� �� ����� � ������������ �������, �� ��������
  // ����� ����� �������� �������� �������.
  real_vector( int len, const real* );

  // ��������������� �� �����, �� �������� ������������ ������ �������.
  // ���� ������ ��������� ��������� ���� RVECT ��� LVECT.
  explicit real_vector( referer<stream> file );

  // ��������������� �� ����� �����, �� �������� ������������ ������ �������.
  // ��� ���������� ��������� ���� RVECT ��� LVECT ������������ ������ "������" ���������� �����.
  explicit real_vector( const char* );

  real_vector( const real_vector& src );
  virtual ~real_vector();

  real_vector& operator=( const real_vector& src );

  int len() const { return _len; }              // ��������� ����� �������.

  void resize( int len ) { realloc(len); }

  real& operator[]( int j ) {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[j];
  }
  const real& operator[]( int j ) const {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[j];
  }

  // �������������� ��� �������� �������.
  // ��� ���������� � �������� ����������.
        real& at( int j )       { return _data[j]; }
  const real& at( int j ) const { return _data[j]; }

  // ���������� �������� ������� � ��������� �����.
  // ����� ������ �������� prec ������������� �����.
  // �������� nf ������ ����� ������ ����� ��� ������.
  // ��� ������ � ������� nfSCNT �������� ���������� ��� ����� �������� ����,
  // � ��� ������ � ������� nfFIXED -- ��� ����� ���� ����� �������.

  // ���������� ������� � ��������� �����.
  void save( referer<stream> file, int prec = 6, real_format nf = nfSCNT ) const;

  // ���������� ������� � ��������� ����� � �������� ������.
  void save( const char *name, int prec = 6, real_format nf = nfSCNT ) const;

  // ���������� ������� � ��������� ����� � �������� ������ ��� ���������.
  void export_as_text( const char *name, int prec = 6, real_format nf = nfSCNT ) const;

  // ����������� � �������� ���������� idx0 ������� �������
  // ���������� ������� ������� ������� �����.
  // ��� ���� ���������� ������� ������� ����������� ����������,
  // ��������� � ��������� val.
  void copy( const real_vector& src, real val = 0.0, int idx0 = 0 );

  // �������� �� ����������� ������� ������� ����� len().
  // �������� idx ������ ��������� ������ ����������� �������.
  void copy_sub( const real_vector& src, int idx );

  void set_val( real val );      // ��������� ���� �������� �������� val
  void set_zero();               // �������� ��� �������
  void swap( int k, int j );     // �������� k-� � j-� �������� �������
  void revers();                 // �������� ������� ��������� �� ��������
  void center();                 // ������������ ������� �������

  real mag() const;              // ����� ��������� ���������
  real norm() const;             // ������ �� ����� ��������� ���������
  real norm_manh() const;        // ������������� �����
  real min() const;              // ����������� �������
  real max() const;              // ������������ �������
  int  min_idx() const;          // ������ ������������ ��������
  int  max_idx() const;          // ������ ������������� ��������
  real min_abs() const;          // ����������� �� ������ �������
  real max_abs() const;          // ������������ �� ������ �������
  int  min_abs_idx() const;      // ������ ������������ �� ������ ��������
  int  max_abs_idx() const;      // ������ ������������� �� ������ ��������
  real inner_sum() const;        // ����� ���������
  real mid() const;              // ������� �������� ���������
  real disp() const;             // ��������� ���������

  // ���������� k-�� ������� ������������������ �������, �������
  // �������� �������� ������������ �������������� ��������,
  // �������� ���� �� ����� �� k ��������.
  // ��� ���� ��� ���� k ����������� ��� ���������� ����� len().
  real acorr( int k ) const;

  // ���������� �������� �������-�������� ������������.
  // �������� �������, ��������������� ��� ������� ������� � ������������� ������
  // � ��������� �������� ���������� ������� � ����� t.
  // ��� ���� �������� t ������ ������ �� ������� [0, len()-1].
  real interpol( real t ) const; // �������� � ����� t ��� ������������

  // ��������� ���������� ����� ����� ���������.
  static real dist( const real_vector&, const real_vector& );

  // ��������� ������������ ���� ��������.
  static real inner_mul( const real_vector&, const real_vector& );

  // ����������� ���������� ��� ���� ��������.
  static real corr( const real_vector&, const real_vector& );

  // k-� ������ ��������������� ���� ��������.
  // ��� ���������� ��������������� ��������� ����� ������������� ��������������
  // �������� �� ��������, ��� ��������, �������� ���� �� ����� �� k ��������.
  static real ccorr( const real_vector&, const real_vector&, int );

  // ����� �������� ������� ������� (src) �� ������ (dst).
  static real project( const real_vector& dst, const real_vector& src );

  // ������� �������������� ��������� ����������
  real_vector& operator+=( const real_vector& );
  real_vector& operator-=( const real_vector& );
  real_vector& operator*=( real );
  real_vector& operator/=( real );

  int comp( int i, int j ) const {
    return (_data[i] < _data[j]) ? -1 : (_data[i] > _data[j]) ? 1 : 0;
  }

private:
  int   _len;
  real *_data;

  void dealloc();
  void alloc( int len );
  void realloc( int len );

  void read_raw_data( const char* fname );
  bool read_data( referer<stream> file, const char* fname );
};

}; // namespace lwml

#endif // _VECTOR_
