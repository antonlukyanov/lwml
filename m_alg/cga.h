// �������������� ����������� �������
// lwml, (c) ltwood

#ifndef _CGA_
#define _CGA_

#include "mdefs.h"
#include "vector.h"
#include "matrix.h"
#include "frand.h"
#include "i_vfunc.h"

/*#lake:stop*/

namespace lwml {

// �������������� ����������� (�����������) ������� ������ ����������.
// ���������� ������ ������������� ��������� ��� ������ ������������ ����������.
// Differential Evolution (DE) for Continuous Function Optimization.
// An algorithm by Kenneth Price and Rainer Storn.
// http://www.ICSI.Berkeley.edu/~storn/code.html

// ��� �������� ��������� ��������� �������� ������� ��������� ��������� �� ���������.
// ��� ������� �� ������������ ��� ����������� �����������, �������
// ���������� ������� ����� ��������� ����� ������ ������� �� �����.
// ��� ������� ����� � ������������� ������� �������� �������� �������.
// ������ �� ��������� ���������� ��������� �������������� �������,
// ������� �������� ������� ����� ���� �����������.

// ������� �������������� ����������� ����������� �����������
// len (������ ���������), prtr (����������� �������) � alpha (���� �������).
// ��� ������� ��������� len ����� ������������� ��������
// A * dim, ��� 5 <= A <= 10.
// ��� ���� ������� alpha �������� �������� ���������� �� ������� [0.4, 1.0],
// ������ ������� ��������� ��������� ����� 0.5, �� ��� �������
// ���������� ��������� ����� �� ������� ������� ��������� �������� alpha.
// ����������� ������� prtr ���������� �� 0.0 �� 1.0, ������ �������� �������
// � ������������ ������� �������� (0.9 ��� ���� 1.0), ����� ���������
// ����������� �������� ��������� ������� ��������� �������.
// ����� ������� ��������� �������� ��������� ������ �� 0.1 ��� ���� 0.0,
// ����� � ��������� ����������� �� �������� ������������ � �����
// ����������� �������������.
// �� ���������� ������� ����� ������ ������ ��������� � ���� �������,
// ����� ��� ����������� ������� ������ ��� ������ ���������.

class real_ga : public value {
public:
  // ������������� ��������������� ������������.
  // ���������� ����������� ������������ ��������� dim, ����������� ��������� len
  // � �������������� ��������� �������� ��� ���������� ��������� �����.
  real_ga( int dim, int len, real alpha = 0.6, real prtr = 0.8, uint seed = 1 );

  // ��������� ����������� ������.
  int dim() const { return _dim; }

  // ��������� ������� ���������.
  int len() const { return _len; }

  // ����� �������� �����������.
  // ��� ������ ��������� ��������� ������� ���������
  // ���� ��������� �������� ������������.
  void start( const i_vfunction& fn, real a, real b );

  // ����� �������� �����������.
  // ��� ������ ��������� ��������� ������� ���������
  // �������� �������� ��� ������ ����������.
  void start( const i_vfunction& fn, const vector& a, const vector& b );

  // ������� ���� ��� (����� ���������) �����������.
  // ���������� ����������� ����������� ��������.
  real step( const i_vfunction& fn );

  // ������� max_gen ����� (���� ���������) �����������.
  // ���������� ����������� ����������� ��������.
  real steps( const i_vfunction& fn, int max_gen );

  // ��������� j-�� ���������� �������� ������������ �������.
  real get( int j ) const { return get_crnt()(_min_score_idx, j); }

  // ��������� �������� ������������ �������.
  void get( vector& dst ) const { get_crnt().get_str(dst, _min_score_idx); }

  // ��������� ������������ ������������ ��������.
  real val() const { return _min_score; }

  // ��������� ����� ������������� ������ �� ������� ����.
  // ��� ������������� ������� steps() ������������ �����
  // ������������� �� ��������� ����.
  real penalty() const { return _penalty_count; }

  // ��������� ����� �������� ������.
  real sur() const { return _sur_count; }

  // ��������� ��������������� ������� ���������.
  void diam( vector& ) const;

private:
  int _dim;             // ����������� ��������
  int _len;             // ������ ���������
  real _alpha;          // ���� �������
  real _prtr;           // ����������� �������
  frand _rnd;           // �������� ��������
  matrix _pop1, _pop2;  // ������� ���������
  vector _cost;         // ��������� ���������
  vector _child;        // �������-���������
  real _min_score;      // ������� �������
  int _min_score_idx;   // ����� ������������ ��������

  bool _state;

  matrix& get_crnt() { return _state ? _pop1 : _pop2; }
  const matrix& get_crnt() const { return _state ? _pop1 : _pop2; }
  matrix& get_next() { return _state ? _pop2 : _pop1; }

  int _penalty_count;   // �������� ������� � ��������
  int _sur_count;

  void select3( int k, int& a, int& b, int& c );
  void update_opt();
};

}; // namespace lwml

#endif // _CGA_
