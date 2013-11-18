// ��������� �������� ��������� �����
// ������ ��� ������������!
// � �������� ������� ������� ������������ ������ frand (��. frand.h)
// lwml, (c) ltwood

#ifndef _RANDSET_
#define _RANDSET_

#include "defs.h"
#include "mdefs.h"
#include "frand.h"

/*#lake:stop*/

namespace lwml {

// ��������� �������� ��������� �������:
// i_rand   - ����� ��������� ���� �������� �� ���� ���������
// lcg32    - ���������� 32-������ �������� ������������ ���
// randu    - ���������� ������ RANDU �� SSP
// vrand    - ������ �.�.������������
// brand    - ������ rand() �� C/C++ ������������ ����� Borland
// wrand    - ������ rand() �� C/C++ ������������ ����� Watcom
// krand    - ������ �.�.�������
// lcg10    - ���������� ��������� 10-������ �������� ������������ ���
// drand1   - ������� ��������� 10-������ ���
// drand2   - ��� ���� ��������� 10-������ ���
// tfrand   - ������� ��� ������� �������� frand

// �������������� ������������ ������ 32-�������
// ����� � ������������ ����� �� ��������� [0,1]

inline real UINT2FLOAT( uint x )
{
  return static_cast<double>((x >> 1) & 0x7FFFFFFF) / static_cast<double>(0x80000000);
}

// �������������� ������ ����� � ��������� �������� ����� �����

inline int ODD( int x )
{
  return (x & 0x01) ? x : x + 1;
}

// ��������� ��� �� ���������

class i_rand : public interface {
public:
  // �������� ��������� ��������� �����
  virtual real get() = 0;
};

// ���������� 32-������ �������� ������������ ���
// ��� ��������������� ����������� ��������� �����
// � �������� �������� ��������� ������������� ���������

class lcg32 : public i_rand, public value {
public:
  lcg32( uint seed, uint a, uint c ){
    _x = seed;
    _a = a;
    _c = c;
  }

  virtual real get(){
    return UINT2FLOAT(next());
  }

private:
  uint _x; // MUST be 32-bit unsigned int
  uint _a, _c;

  uint next(){
    return _x = _a * _x + _c;
  }
};

// ��� RANDU �� ������ SSP ����� IBM.
// X(k+2) = 6 * X(k+1) - 9 * X(k) mod m

class randu : public lcg32 {
public:
  randu( uint seed = 1 ) : lcg32(ODD(seed), 0x10003, 0) {}
};

// ��� �.�.������������

class vrand : public lcg32 {
public:
  vrand( uint seed = 1 ) : lcg32(ODD(seed), 0xCAB3, 0) {}
};

// ������� rand() �� Borland C/C++
// ������������ ������� ���������� 16-������ �����,
// ����������� �� ������� (X >> 16) & 0x7fff

class brand : public lcg32 {
public:
  brand( uint seed = 1 ) : lcg32(seed, 0x15A4E35, 1) {}
};

// ������� rand() �� Watcom C/C++
// ������������ ������� ���������� 16-������ �����,
// ����������� �� ������� (X >> 16) & 0x7fff

class wrand : public lcg32 {
public:
  wrand( uint seed = 1 ) : lcg32(seed, 0x41C64E6D, 0x3039) {}
};

// ��� �.�.�������

class krand : public i_rand, public value {
public:
  krand(){
    _a = sqrt(2.0);
    _b = sqrt(3.0);
  }

  virtual real get();

private:
  double _a, _b;

  double frac( double x ) const { return x - floor(x); }
};

// ���������� ��������� 10-������ �������� ������������ ���

class lcg10 : public i_rand, public value {
public:
  lcg10( uint seed, uint a, uint c ){
    _x = seed;
    _a = a;
    _c = c;
  }

  virtual real get() { return real_cast(step()) / 1024.0; }

private:
  uint _x;
  uint _a, _c;

  uint step(){
    return _x = (_a * _x + _c) % 1024;
  }
};

// ������� ��������� 10-������ ���

class drand1 : public lcg10 {
public:
  drand1( uint seed = 1 ) : lcg10(seed, 1021, 21) {}
};

// ��� ���� ��������� 10-������ ���

class drand2 : public lcg10 {
public:
  drand2( uint seed = 1 ) : lcg10(seed, 997 /*77*/, 0 /*21*/ /*1*/) {}
};

// ������� ��� ������� �������� frand

class tfrand : public i_rand, public value {
public:
  tfrand( uint seed = 1 ) : _rnd(seed) {}

  virtual real get() { return _rnd.get_real(); }

private:
  frand _rnd;
};

}; // namespace lwml

#endif // _RANDSET_
