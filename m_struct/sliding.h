// ����, ���������� �� ������� ������
// lwml, (c) ltwood

#ifndef _SLIDING_
#define _SLIDING_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"
#include "i_vset.h"

/*#lake:stop*/

namespace lwml {

// ����� sliding ����������� ����� ��������� ���� ������ len
// �� �������, ������ ������ ���������� ���� ���������� �� ���������
// � ������ ����������� �� shift ��������
// ��� ��������������� ����������� ������ � ��������� ������� �
// ����� ������ ��������� �� ���� ������
// ����� dim() ��������� �������� ������ ����
// ����� num() ���� ����� ���������� ����
// �������� () ��������� �������� ������� ���������� ����

class sliding : public i_vector_set, public hardcore {
public:
  // ��������� �� ������ ���� ����� len � ����� shift.
  sliding( const vector&, int len, int shift = 1 );

  virtual int len() const { return _win_num; }  // ����� ����
  virtual int dim() const { return _win_len; }  // ����� ����

  // ��������� ������� � ������� d �� ���� � ������� j.
  virtual real get( int j, int d ) const {
    test_index2(j, _win_num, d, _win_len);
    return (*_src)[j * _shift + d];
  }

  // ��������� ���� � ������� j.
  virtual void get( vector& v, int j ) const {
    test_size(v.len(), _win_len);
    v.copy_sub(*_src, j * _shift);
  }

private:
  const vector* _src;
  int _win_len;         // ������ ����
  int _shift;           // ����� ����
  int _win_num;         // ����� ����

  // ����� ���� ����� winlen, ������� ����� �������� �� ������ ����� len
  // ��� ������ ���������� ���� �� shift �������� �� ��������� � �����������
  int calc_win_num( int len, int winlen, int shift ){
    return (len - winlen) / shift + 1;
  }
};

}; // namespace lwml

#endif // _SLIDING_
