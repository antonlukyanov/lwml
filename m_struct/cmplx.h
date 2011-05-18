// ����������� ����������
// lwml, (c) ltwood

#ifndef _CMPLX_
#define _CMPLX_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

class cmplx : public value {
public:
  // ������������� ������������ �����
  cmplx( real re = 0.0, real im = 0.0 ) : _re(re), _im(im) {}

  // ��������� ��������
  void set( real re, real im = 0.0 ){
    _re = re;
    _im = im;
  }

  // ��������� ������������ �����
  const real& re() const { return _re; }

  // ��������� ������ �����
  const real& im() const { return _im; }

  // ��������� ������������ �����
  // ��������� ������������ ��������� ���������� ���� c.re() = 6.0;
  // ���������� ������ �� ������������ �����
  real& re() { return _re; }

  // ��������� ������ �����
  // ��������� ������������ ��������� ���������� ���� c.im() = 6.0;
  // ���������� ������ �� ������ �����
  real& im() { return _im; }

  real norm() const;     // ����� (������)
  real mag() const;      // ������� �����
  cmplx conj() const;    // ����������� �����
  real arg() const;      // ��������

  cmplx& operator+=( const cmplx& ); // ���������� ������������ �����
  cmplx& operator-=( const cmplx& ); // ��������� ������������ �����
  cmplx& operator*=( const cmplx& ); // ���������� �� ����������� �����
  cmplx& operator/=( const cmplx& ); // ������� �� ����������� �����
  cmplx& operator*=( real );         // ���������� �� ������������ �����
  cmplx& operator/=( real );         // �������� �� ������������ �����

  // ������� ����������� �����
  // c - �������, d - ��������
  // ���������� ��������� ������� ����� c �� ����� d
  static cmplx div( const cmplx& c, const cmplx& d );

  // ������� �������, ���������������� ������������ �����
  // pnt - �������������� �����
  // angle - ���� ��������
  // cnt - �����, ����� ������� ����������� �������
  // ���������� ����������� �����, ���������� ����������� �������
  static cmplx rot( const cmplx& pnt, real angle, const cmplx& cnt );

  // ���������� ����� �� ������������ �����
  static cmplx sqrt( const cmplx& z );

  // ��������� ������������ ����� ��������� ������������ �����
  static real  re( const cmplx& c );
  // ��������� ������ ����� ��������� ������������ �����
  static real  im( const cmplx& c );
  // ���������� ����� (������) ��������� ������������ �����
  static real  norm( const cmplx& c );
  // ���������� �������� ����� ��������� ������������ �����
  static real  mag( const cmplx& c );
  // ��������� ������������ ����� ��� ��������� ������������ �����
  static cmplx conj( const cmplx& c );
  // ���������� ��������� ��������� ������������ �����
  static real  arg( const cmplx& c );

  // ���������� ���������� ����� ������������ �������
  static real  dist( const cmplx& c1, const cmplx& c2 );

  // ���������� ������������ ����� �� ������ � ���������
  // r - ������, t - ��������
  static cmplx mkpolar( real r, real t );

  // ���������� ������������ ����� � ��������� ������ � �������� ����������
  // t - ��������
  static cmplx iexp( real t );

  // ���������� ����������� ����������
  static cmplx exp( const cmplx& c );

  // ���������� ������������ ������� ������������ �����.
  // c - ����������� �����, ���������� � �������
  // p - ������������ ���������� �������
  static cmplx pow( const cmplx& c, real p );

private:
  real _re, _im;
};

// "�������" ����������� ����������

// ��������� ���������������� �� ����� ������������ �����
cmplx operator-( const cmplx& c );

// �������� ����������� �����
cmplx operator+( const cmplx& c1, const cmplx& c2 );

// ��������� ����������� �����
cmplx operator-( const cmplx& c1, const cmplx& c2 );

// ��������� ����������� �����
cmplx operator*( const cmplx& c1, const cmplx& c2 );

// ������� ����������� �����
cmplx operator/( const cmplx& c1, const cmplx& c2 );

// ��������� ������������ ����� �� ������������
cmplx operator*( const cmplx& c, real r );

// ��������� ������������� ����� �� �����������
cmplx operator*( real r, const cmplx& c );

// ������� ������������ ����� �� ������������
cmplx operator/( const cmplx& c, real r );

// implementation

inline real cmplx::norm() const {
  return hypot(_re, _im);
}

inline real cmplx::mag() const {
  return fsqr(hypot(_re, _im));
}

inline cmplx cmplx::conj() const {
  return cmplx(_re, -_im);
}

inline real cmplx::arg() const {
  return atan2(_im, _re);
}

inline cmplx& cmplx::operator+=( const cmplx& c ){
  _re += c._re;
  _im += c._im;
  return *this;
}

inline cmplx& cmplx::operator-=( const cmplx& c ){
  _re -= c._re;
  _im -= c._im;
  return *this;
}

inline cmplx& cmplx::operator*=( const cmplx& c ){
  real nr = _re * c._re - _im * c._im;
  _im = _re * c._im + _im * c._re;
  _re = nr;
  return *this;
}

inline cmplx& cmplx::operator/=( const cmplx& c ){
  cmplx res = cmplx::div( *this, c );
  _re = res._re;
  _im = res._im;
  return *this;
}

inline cmplx& cmplx::operator*=( real x ){
  _re *= x;
  _im *= x;
  return *this;
}

inline cmplx& cmplx::operator/=( real x ){
  _re /= x;
  _im /= x;
  return *this;
}

inline real cmplx::re( const cmplx& c ){
  return c._re;
}

inline real cmplx::im( const cmplx& c ){
  return c._im;
}

inline real cmplx::norm( const cmplx& c ){
  return hypot(c._re, c._im);
}

inline real cmplx::mag( const cmplx& c ){
  return fsqr(hypot(c._re, c._im));
}

inline cmplx cmplx::conj( const cmplx& c ){
  return cmplx(c._re, -c._im);
}

inline real cmplx::arg( const cmplx& c ){
  return atan2(c._im, c._re);
}

inline real cmplx::dist( const cmplx& c1, const cmplx& c2 ){
  return hypot(c1._re - c2._re, c1._im - c2._im);
}

inline cmplx cmplx::mkpolar( real r, real t ){
  return cmplx( r * cos(t), r * sin(t) );
}

inline cmplx cmplx::iexp( real t ){
  return cmplx( cos(t), sin(t) );
}

inline cmplx cmplx::exp( const cmplx& c ){
  real rexp = ::exp(c._re);
  return cmplx( rexp * cos(c._im), rexp * sin(c._im) );
}

inline cmplx cmplx::pow( const cmplx& c, real p ){
  return cmplx::mkpolar(::pow( norm(c), p), p * arg(c));
}

inline cmplx operator-( const cmplx& c ){
  return cmplx(-c.re(), -c.im());
}

inline cmplx operator+( const cmplx& c1, const cmplx& c2 ){
  return cmplx(c1.re() + c2.re(), c1.im() + c2.im());
}

inline cmplx operator-( const cmplx& c1, const cmplx& c2 ){
  return cmplx(c1.re() - c2.re(), c1.im() - c2.im());
}

inline cmplx operator*( const cmplx& c1, const cmplx& c2 ){
  return cmplx( c1.re() * c2.re() - c1.im() * c2.im(),
                c1.re() * c2.im() + c1.im() * c2.re() );
}

inline cmplx operator/( const cmplx& c1, const cmplx& c2 ){
  return cmplx::div(c1, c2);
}

inline cmplx operator*( const cmplx& c, real r ){
  return cmplx(c.re() * r, c.im() * r);
}

inline cmplx operator*( real r, const cmplx& c ){
  return cmplx(r * c.re(), r * c.im());
}

inline cmplx operator/( const cmplx& c, real r ){
  return cmplx(c.re() / r, c.im() / r);
}

}; // namespace lwml

#endif // _CMPLX_
