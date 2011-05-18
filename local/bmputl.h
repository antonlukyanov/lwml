// ������� ��������� �����������
// lwml, (c) ltwood

#ifndef _BMPUTL_
#define _BMPUTL_

#include "defs.h"
#include "mdefs.h"
#include "matrix.h"
#include "imatrix.h"

/*#lake:stop*/

namespace lwml {

// ����� ��������� ������� ����������� ��������� ��������� �������� �����������.
// ��� ������ ��������� � "������������" ������� "�����������" ���������
// ����������� �����������, �������������� ��������� ��������.

// ����������� �������� ��� ����� ���������� �����������
// ������������ ��� ������������ ����������� ������������ �����������.

const int STD_BMP_HST_LEN = 64;

class bmputil : public scope {
public:
  // ������������ ����������� �����������
  // hstlen - ����� ���������� �����������
  static void equalization( matrix& dst, const matrix& src, int hstlen );
  static void equalization( matrix& src, int hstlen );

  // ���������� ����������� � ��������� ����� ������� �������
  // lev - ����� ������� � �������������� �����������
  static void leveling( int_matrix& dst, const matrix& src, int lev );

  // ������������ �����������.
  // ������� ������� src ������ ������ �������� ��
  // ��������������� ������� ������� dst.
  static void decimation( matrix& dst, const matrix& src );

  // �������������� ������� � ��������� ���� ����� ��������� �� ������.
  // ����� �������� �� ������� [0,1].
  static void thresholding( int_matrix& dst, const matrix& src, real val );

  // ����������� ������� � ������� ��������������.
  static void clust_thresholding( int_matrix& dst, const matrix& src, int seed = 1 );

  // ���������� ����������� �� ������.
  static void sobel( matrix& dst, const matrix& src );

  // ������� ���������� �� ����������� 3x3.
  static void filter( matrix& dst, const matrix& src, real cnt, real cross, real diag );

  // ��������� �������� �����������.
  static void diff( matrix& dst, const matrix& src1, const matrix& src2 );

  // ���������� �����������.
  static void hist( vector& dst, const matrix& src, int hst_len );

  // ���������� ������� ��������� ��������� ������
  // ������� �������������� ������� ����� 0,1,2 � ����������� �� ����,
  // ������� �� ��������� ������� ��������� (����� '����' = '�������'):
  // - ���� ����� ����� c1 � ���� �� ������� ������ ����� c2
  // - ���� ����� ����� c1 � ���� �� ������� ������ ����� c2
  // c1 - ���� ����������� �����
  // c2 - ���� �������� �����
  static void cooccuring( int_matrix& dst, const int_matrix& src, int c1, int c2 );

  // ���������� ������������ ��������� ������� ��������� ��������� ������
  // ������� �������������� ������� ����� 0,1,2 � ����������� �� ����,
  // ������� �� ��������� ������� ���������:
  // - ������� ������� ��� �������� � ������� ������ ����� ��������� �����
  // - ������� ������� ��� �������� � ������� ������ ����� ��������� �����
  // dc - ������� �������
  static void cooccuring( int_matrix& dst, const int_matrix& src, int dc );

  // C����������� �� ����������� ��������� ������� ��� ������������ �������
  // ������� �������������� ������� ����� ����� ���������
  // �������� �������, ������������� � �������� �����������
  // nbh_size - ������ �����������
  // prg - ��������-���������
  static void counting( matrix& dst, const matrix& src, int nbh_size, tick_mode tick = tmOFF );

  // C����������� �� ����������� ��������� ������� ��� ������������� �������
  // ������� �������������� ������� ����� ����� ���������
  // �������� �������, ������������� � �������� �����������
  // nbh_size - ������ �����������
  // prg - ��������-���������
  static void counting( int_matrix& dst, const int_matrix& src, int nbh_size, tick_mode tick = tmOFF );

  // �������� ����������� ������ ������
  static void rot( int_matrix& dst, const int_matrix& src, real angle );
};

}; // namespace lwml

#endif // _BMPUTL_
