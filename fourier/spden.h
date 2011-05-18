#ifndef _SPDEN_
#define _SPDEN_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"

/*#lake:stop*/

namespace lwml {

// ���������� ���������� ������ �������������� �������.
// ��� ��������������� ����������� ����� ����, ����� ������ �� ����� � ����� ����.
// �� ��������� ����� ������ �� ����� ����� ����, � ����� ����
// ����� �������� ����� ���� (����������� ������ ����������� ���������).
// �������������� ���������� ��� ���������� � ������ ������ put().
// ����� �������������� ������ �������� ������.

class spdenest : public value {
  friend class spacorr;

public:
  spdenest( int winlen, int uplen = 0, int winshift = 0 );

  // ���������� ������.
  void put( const vector& src );

  // �������� ����� ����, �� ������� ��� �������� ������.
  int win_num() const { return _win_num; }

  // �������� ���������� ������������ ��������.
  int len() const { return _len; }

  // ���������� �������� ������������ ������������.
  real operator[]( int idx ) const;

  // �������� ��� ������������ ������� �������
  // �������� � ������ ���������� ������� �����.
  void get( vector& ) const;

  // �������� �������� �� ������� ���������.
  real error() const { return _p[_len-1]; }

  // ���������� ������ �������� �������.
  real sum() const   { return _p.inner_sum(); }

  // ���������� ��������������� �������, ���������������
  // ���������� ������������� �������.
  real freq( int idx )   const { return real_cast(idx) / real_cast(_splen); }

  // ���������� ��������������� ������, ���������������
  // ���������� ������������� �������.
  real period( int idx ) const { return real_cast(_splen) / real_cast(idx); }

  // ���� �������� ���� �� ��������������� �������.
  real delta_freq()      const { return 1.0 / real_cast(_splen); }

private:
  int _winlen;    // ����� ����
  int _uplen;     // ����� ������ �� �����
  int _splen;     // ����� �������
  int _winshift;  // ����� ����
  int _len;       // ����� ������� ��������
  vector _p;      // ������ ��������
  int _win_num;
};

inline real spdenest::operator[]( int idx ) const {
  test_index(idx, _len);
  return _p[idx];
}

class spacorr : public value {
public:
  spacorr( const spdenest& );

  void operator()( const spdenest& );

  int len() const { return _clen; }

  real operator[]( int idx ) const;

private:
  int _splen;
  int _clen;
  vector _xr, _xi;
};

inline real spacorr::operator[]( int idx ) const {
  test_index(idx, len());
  return _xr[idx];
}

}; // namespace lwml

#endif // _SPDEN_
