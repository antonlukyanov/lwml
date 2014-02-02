// ���������� ������� � ������������ �������

#ifndef _CONVOLV_
#define _CONVOLV_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"

/*#lake:stop*/

namespace lwml {

// ����� ��������� ������������ �������� ���������� �������
// ��� ��������������� ����������� ������������ ����� ��������������� �������
// � �������� ����� ���������� ��������������, ���������������
// ��������������� � ������������� ��������� �������
// ������ ����������� ��������� �������� ���������� ��������������,
// ��������������� ������ ��������������� ��������� �������
// � ������������� �� ��������������
// ������� ������������ �������� (), ������� ����������� ������ src
// � ��������� ��� ��������������� �� � ������ ��������� � dst
// ������� src � dst ������ ���� ����������� �������

class convolver : public value {
public:
  convolver( int len, const vector& hp, const vector& hn );
  convolver( int len, const vector& hp );

  void operator()( vector& dst, const vector& src );

private:
  int _uplen;   // ����� ������ �����������
  int _dlen;    // ������������ ����� ������
  int _splen;   // ������ ������ ��������
  vector _hr, _hi;
  vector _xr, _xi;
};

// ����� ��������� ������������ �������� ���������� ���������������
// ����������� ��������������� ���������� �� ������������� �����������
// ����������� ������������ ��������� (1/N)
// ��� ��������������� ����������� ������������ ����� ��������������� �������
// � ����� clen ����������� ������ ���
// ��� ���� ����������� ������� ��� ��� -clen <= j <= clen
// ����� len() ���������� ����� ������ �� ��������������� �������� ���
// ���������� ��� ��������� �������� (), ���������� �� ���� ������� �������� ������
// ������� ��� ����� ��������, ������ ����� []

class crosscorr : public value {
public:
  crosscorr( int len, int clen );

  int len() const { return _clen + 1; }

  void operator()( const vector& src1, const vector& src2 );

  real operator[]( int idx ) const;

private:
  int _dlen;  // ������������ ����� ������
  int _clen;  // ������������ ����� �������������� �������
  int _splen; // ������ ������ ��������
  vector _xr, _xi;
  vector _xxr, _xxi;
};

inline real crosscorr::operator[]( int idx ) const {
  test_index(int_cast(labs(idx)), len());
  return (idx >= 0) ? _xr[idx] : _xr[_splen+idx];
}

// ����� ��������� ������������ �������� ���������� ��������������
// ��� ��������������� ����������� ������������ ����� ��������������� �������
// � ����� clen ����������� ������ ���
// ��� ���� ����������� ������� ��� ��� -clen <= j <= clen
// ����� len() ���������� ����� ������ �� ��������������� �������� ���
// ���������� ��� ��������� �������� (), ���������� �� ���� ������� �������� ������
// ������� ��� ����� ��������, ������ ����� []

class autocorr : public value {
public:
  autocorr( int len, int clen );

  int len() const { return _clen + 1; }

  void operator()( const vector& src );

  real operator[]( int idx ) const;

private:
  int _dlen;    // ������������ ����� ������                
  int _clen;    // ������������ ����� �������������� �������
  int _splen;   // ������ ������ ��������                   
  vector _xr, _xi;
};

inline real autocorr::operator[]( int idx ) const {
  test_index(int_cast(labs(idx)), len());
  return _xr[labs(idx)];
}

}; // namespace lwml

#endif // _CONVOLV_
