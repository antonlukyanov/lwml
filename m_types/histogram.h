// �����������
// lwml, (c) ltwood

//!! TODO: ����������� � herst

#ifndef _HISTOGRAM_
#define _HISTOGRAM_

#include "defs.h"
#include "ivector.h"

/*#lake:stop*/

namespace lwml {

// ����� histogram ������������ ���������� ����������.
// ��� ��������������� ����������� ����� �������� ����������� (len)
// � ������� [ax, bx], ��������������� �����������.

class histogram : public value {
public:
  histogram( int len, real ax, real bx );

  // �������� �����������
  void clear();

  real ax() const { return _ax; }  // �������� ����� ������� �����������
  real bx() const { return _bx; }  // �������� ������ ������� �����������

  int len() const { return _data.len(); }  // �������� ����� ��������
  int total() const { return _total; }    // �������� ����� ����� ���������� �����

  int  val2idx( real val ) const; // ������������� ����� � ����� �������
  real idx2val( int idx ) const;  // �������� �� ������ ������� ��� ����� �������
  real idx2cnt( int idx ) const;  // �������� �� ������ ������� ��� ��������

  histogram& put( real x );          // �������� ����� � �����������

  int max_idx() const; // �������� ����� ������� � ������������ ����������� �����
  int max_val() const; // �������� ����� ����� � ����� ���������� �������

  int operator[]( int j ) const { return _data[j]; } // �������� ����� ����� � j-� �������

  // ��������� ������� �������������
  // ������ ������ ����� �����, ������ ����� �������� �����������
  // � j-� ������� ������� ��������� ��������� ����� �����
  // � �������� � �������� k <= j
  void set_integral( int_vector& );

  // ���������� ���������� ��-������� ��� �������� � ����������� �������������
  real chisq_val();  // �������� ���������� ��-������� S
  real chisq_prob(); // ���������� ����, ��� s <= S
  int  chisq_susp(); // ������ ���������������� S (0..3)

private:
  real _ax, _bx;
  int_vector _data;
  int _total;
};

}; // namespace lwml

#endif // _HISTOGRAM_
