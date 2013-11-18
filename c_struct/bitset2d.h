// ��������� ������ �����.
// lwml, (c) ltwood

#ifndef _BITSET2D_
#define _BITSET2D_

#include "defs.h"
#include "mdefs.h"
#include "bitset.h"

/*#lake:stop*/

namespace lwml {

class bitset2d : public value {
public:
  // ������� ��������� ������� ��������� �������� xsz �� ysz ���, ��������� ������ ������� � v.
  bitset2d( int xsz, int ysz, bool v = false )
    : _xsize(xsz), _ysize(ysz), _bits(xsz * ysz, v)
  {
    double sz = real_cast(xsz) * real_cast(ysz);
    if( sz > INT_MAX )
      fail_assert("too large bitset2d");
  }

  // ������������� �������� ��������� �� ������ ������.
  void char_read( const char* buf ) { _bits.char_read(buf); }

  // �������������� �������� ��������� � ������ ������.
  void char_write( char* buf ) const { _bits.char_write(buf); }

  // ���������� ����� ������ � �������������� ��������� ������ ������.
  int char_num() const { return _bits.len(); }

  // ��������� �������������� �������� ���������.
  int lx() const { return _xsize; }
  int ly() const { return _ysize; }

  // �������� ��� ���� ��������� � ����.
  void set_zero() { _bits.set_zero(); }

  // ���������� ��� ���� ��������� � �������.
  void set_one() { _bits.set_one(); }

  // ���������� ��� � ������� (x, y).
  void set( int ix, int iy ) {
    TEST_INDEX2(ix, _xsize, iy, _ysize);
    _bits.set(idx2lidx(ix, iy));
  }

  // ���������� ���� � ��������������.
  void set( int ax, int bx, int ay, int by );

  // �������� ��� � ������� (x, y).
  void clear( int ix, int iy ) {
    TEST_INDEX2(ix, _xsize, iy, _ysize);
    _bits.clear(idx2lidx(ix, iy));
  }

  // �������� ���� � ��������������.
  void clear( int ax, int bx, int ay, int by );

  // ������������� ��� � ������� (x, y).
  void flip( int ix, int iy ) {
    TEST_INDEX2(ix, _xsize, iy, _ysize);
    _bits.flip(idx2lidx(ix, iy));
  }

  // ������������� ���� � ��������������.
  void flip( int ax, int bx, int ay, int by );

  // �������� �������� ���� � ������� (x, y).
  bool test( int ix, int iy ) const {
    TEST_INDEX2(ix, _xsize, iy, _ysize);
    return _bits.test(idx2lidx(ix, iy));
  }

  // ��������� ��������� �� �����������.
  // ���������� true ���� ��������� ������������ � ���������� ls.
  bool is_meet( const bitset2d& ls ) const {
    TEST_INDEX2(ls.lx(), _xsize, ls.ly(), _ysize);
    return _bits.is_meet(ls._bits);
  }

  // ���������� ����������� ��������.
  // ��������� ���������-�������� � �������� ��������� �.�.
  // ������������� � ��������� ��� ����, ������������� � ��������� ls.
  // ���������� ������ �� ���������� �������� ���������.
  bitset2d& join( const bitset2d& ls ) {
    TEST_INDEX2(ls.lx(), _xsize, ls.ly(), _ysize);
    _bits.join(ls._bits);
    return *this;
  }

  // ���������� ����������� ��������.
  // ���������� ������� ��������� � ����������-���������� �.�.
  // ��������� �������������� ������ ����, ������������� � ��������� ls.
  // ���������� ������ �� ���������� �������� ���������.
  bitset2d& meet( const bitset2d& ls ) {
    TEST_INDEX2(ls.lx(), _xsize, ls.ly(), _ysize);
    _bits.meet(ls._bits);
    return *this;
  }

  // ���������� true, ���� ��� ���� ��������� � ��������� false.
  bool is_empty() const {
     return _bits.is_empty();
  }

private:
  int idx2lidx( int32 x, int32 y ) const { return x * _ysize + y; }

  int _xsize, _ysize;
  bitset _bits;
};

}; // namespace lwml

#endif // _BITSET2D_
