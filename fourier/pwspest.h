// ���������� ������� ��������
// lwml, (c) ltwood

#ifndef _PWSPEST_
#define _PWSPEST_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"
#include "sliding.h"

/*#lake:stop*/

namespace lwml {

// ������������� ������������ ������ �������� ������������� �������.
// ��� ��������������� ����������� ���������� ����� �������������� ������.
// �������������� ���������� ������� put().
// ������ ����������� ������ �� ��������� ������ � �����, ���������
// ��� ���������������, ����� ������� ������.
// ��� ���� ��� ���� vector ���������� �������������� ����� ����������� �������,
// � ��� ���� sliding ���������� �������������� ���������� ����.
// ����� �������������� ������ �������� ������.

class pwspest : public value {
public:
  pwspest( int len );

  // ���������� ��������� ������.
  void put( const vector& );
  void put( const sliding& src, int idx );

  // �������� ���������� ������������ ��������.
  int len() const { return _splen / 2 + 1; }

  // ���������� �������� ������������ ������������.
  real operator[]( int idx ) const;

  // �������� ��� ������������ ������� ������� ��������
  // � ������ ���������� ������� �����.
  void get( vector& ) const;

  // ���������� �������� �� ������� ���������.
  real error() const { return mag(_xr[_splen/2], _xi[_splen/2]); }

  // ���������� ������ �������� �������.
  real sum() const;

  // ���������� ��������������� �������, ���������������
  // ���������� ������������� �������.
  real freq( int idx )   const { return real_cast(idx) / real_cast(_splen); }

  // ���������� ��������������� ������, ���������������
  // ���������� ������������� �������.
  real period( int idx ) const { return real_cast(_splen) / real_cast(idx); }

  // �������� ���� �� ��������������� �������.
  real delta_freq()      const { return 1.0 / real_cast(_splen); }

private:
  int _dlen;     // ������������ ����� ������
  int _splen;    // ����� �������
  vector _xr, _xi;
};

inline real pwspest::operator[]( int idx ) const {
  test_index(idx, len());
  if( idx == 0 || idx == len()-1 )
    return mag(_xr[idx], _xi[idx]);
  else
    return 2.0 * mag(_xr[idx], _xi[idx]);
}

}; // namespace lwml

#endif // _PWSPEST_
