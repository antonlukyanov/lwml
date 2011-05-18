// ����� �������������� ����������� � �������
// lwml, (c) ltwood

#ifndef _MDEFS_
#define _MDEFS_

#include "defs.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

// �������������� ����������

DEF_EX_CLASS(ex_base, ex_math);

DEF_EX_TYPE(ex_math, ex_sing, "numerical singularity");
DEF_EX_TYPE(ex_math, ex_noconv, "no convergence");
DEF_EX_TYPE(ex_math, ex_loss, "loss of significance");

// ������� ������������ ���.

typedef double real;
typedef long double long_real;

// ��������� �.�. ������ (quiet) ������� (NaN)

real real_nan();
bool is_nan( real );

void nan_filling( real*, int sz );

// �������������� ���������, �����������
// ����������� �� ����� float.h

// STDEPS    - ����������� �������� "������" �����,
//             ������������� ��� ��������, ����������� � ��.
// REAL_DIGS - ����� ���������� ���� ����� �������
// REAL_BITS - ����� ����� � ��������
// REAL_EPS  - ����������� ������������� �����, �������� �� ����
// REAL_MIN  - ����������� �������� ������������ ����������
// REAL_MAX  - ������������ �������� ������������ ����������

const real STDEPS    = 1e-12;
const int  REAL_DIGS = DBL_DIG;         // 15
const int  REAL_BITS = DBL_MANT_DIG;    // 53
const real REAL_EPS  = DBL_EPSILON;     // 2.22E-16
const real REAL_MIN  = DBL_MIN;         // 2.23E-308
const real REAL_MAX  = DBL_MAX;         // _max_dble

// �������� �������������� ���������

#undef M_E
#undef M_LOG2E
#undef M_LOG10E
#undef M_LN2
#undef M_LN10
#undef M_PI
#undef M_PI_2
#undef M_PI_4
#undef M_1_PI
#undef M_2_PI
#undef M_2_SQRTPI
#undef M_SQRT2
#undef M_1_SQRTPI
#undef M_SQRT1_2

#define M_E         2.71828182845904523536   // $e$
#define M_LOG2E     1.44269504088896340736   // $\log_2(e)$
#define M_LOG10E    0.434294481903251827651  // $\log_10(e)$
#define M_LN2       0.693147180559945309417  // $\log_e(2)$
#define M_LN10      2.30258509299404568402   // $\log_e(10)$
#define M_PI        3.14159265358979323846   // $\pi$
#define M_PI_2      1.57079632679489661923   // $\pi/2$
#define M_PI_4      0.785398163397448309616  // $\pi/4$
#define M_1_PI      0.318309886183790671538  // $1/\pi$
#define M_2_PI      0.636619772367581343076  // $2/\pi$
#define M_2_SQRTPI  1.12837916709551257390   // $2/\pi^{1/2}$
#define M_SQRT2     1.41421356237309504880   // $2^{1/2}$
#define M_1_SQRTPI  0.564189583547756286948  // $1/\pi^{1/2}$
#define M_SQRT1_2   0.707106781186547524401  // $1/2^{1/2}$

// �������������� �������������� ���������

const real M_SQRTPI   = 1.77245385090551603; // $\pi^{1/2}$
const real M_SQRT2PI  = 2.50662827463100050; // $(2\pi)^{1/2}$
const real M_SQRT3    = 1.73205080756887729; // $3^{1/2}$
const real M_SQRT5    = 2.23606797749978970; // $5^{1/2}$
const real M_GOLDSECT = 0.61803398874989485; // $(5^{1/2}-1)/2$

// ������� ����������� ���������� �����.
// ��� ������� ������������ ��� ����� ������� ���������
// ������ ����������� static_cast �� int � real
// � ������������ ����������� ������ static_cast �� real � int.

// ���������� �� ���� real � ���� int
inline int int_cast( real x ) { return static_cast<int>(x); }

// ���������� �� ���� real � ���� uint
inline uint uint_cast( real x ){ return static_cast<uint>(x); }

// ���������� �� ���� int � ���� real
inline real real_cast( int x ) { return static_cast<real>(x); }

// ���������� �� ���� uint � ���� real
inline real real_cast( uint x ){ return static_cast<real>(x); }

// ���� �����

inline real fsgn( real x ){
  return x < 0.0 ? -1.0 : (x > 0.0 ? 1.0 : 0.0);
}

// (-1)^n � ������������ � ������������� ������

inline int  loddsgn( int n ) { return (n&1) ? -1 : 1; }
inline real foddsgn( int n ) { return (n&1) ? -1.0 : 1.0; }

// ���������� ����� �� ������ �����

inline real fsqrt( int x ){ return sqrt(real_cast(x)); }
inline real fsqrt( real x ){ return sqrt(x); }

// ���������� ����� � ����� ������� n

real intpow( real x, int n );

// ���������� �������������� ��������� ���������

int intlog2( int );

// ���������� ����� �� ��������� ����� ������� ������

int up2pow2( int );

// �������� �������� �� ����� ����� �������� ������

inline bool ispow2( int x ){ return !((~(~0u>>1)|x) & (x - 1)); }

// ���������� � �������

inline real fsqr( real t ){ return t * t; }
inline real fsqr( int t ){ return t * t; }

// ���������� ����� ��������� ����������

inline real mag( real x, real y ){
  return fsqr(hypot(x, y));
}

// ������ �� ����� ��������� ��� ���� ����������

inline real hypot3( real x, real y, real z ){
  return hypot(x, hypot(y, z));
}

// ������� � ��������������� ������� ��������� (���������)

inline real log_dba( real x, real min = -120.0 ){
  real xmin = pow(10.0, min/20.0);
  return (x > xmin) ? 20.0 * log10(x) : min;
}

// ������� � ��������������� ������� �������� (���������)

inline real log_dbp( real x, real min = -60.0 ){
  real xmin = pow(10.0, min/10.0);
  return (x > xmin) ? 10.0 * log10(x) : min;
}

#if defined(__BORLANDC__) || defined(_MSC_VER)
  inline real log2( real x ){ return log(x) / M_LN2; }
#endif

// ������� x*log(x)  � ������ ��������� ���������
// � �������������� �� ������������� � ����� 0.0

inline real log2c( real x ){
  if( x == 0.0 )
    return -0.0;
  else
    return x * log2(x);
}

// ������� sin(x)/x � �������������� �� ������������� � ����� 0.0

inline real sinc( real x ){
  if( x == 0.0 )
    return 1.0;
  else
    return sin(x) / x;
}

// ����������� ��������
// � �������� �������� [0,1]

inline real sigm( real x ){
  return 1.0 / (1.0 + exp(-x));
}

// ������� ������ ��������� �������,
// ���������� ���������� ����������� �������������.

inline real gauss( real x, real sigma = 1.0 ){
  return exp(-fsqr(x/sigma)/2.0) / (sigma * M_SQRT2PI);
}

// ������� ������� ������.
// � ����� x = 0 ��������� ��������� ��������,
// � � ����� x0 �������� ����������� � d ���.
// ��� d = 2 �������� x0 ��������� � ��������� ��������� FWHM.

inline real gauss_weight( real x, real x0, real d ){
  return exp(-fsqr(x/x0) * log(d));
}

// ������� ���� �� �������� � �������

inline real deg2rad( real gr ){
  return gr * (M_PI / 180.0);
}

// ������� ���� �� �������� � �������

inline real rad2deg( real rd ){
  return rd * (180.0 / M_PI);
}

// ���������� ��� ������������ �����.
// ����������� ������������ ������� ����� � ����������.
// ��� ������� ������������ � C99, �� �� � C++.
// � ���� �� � C99 round() �� ������������� ����������
// ����������� ��������������� ����������.

namespace fpr {
  inline real trunc( real x ) { return (x < 0.0) ? ceil(x) : floor(x); }
  inline real round( real x ) { return (x < 0.0) ? ceil(x - 0.5) : floor(x + 0.5); }

  // ���������� ����� �� d ������ ����� �������
  inline real fround10( real x, int d ){
    real a = pow(10.0, d);
    return round(x * a) / a;
  }

  inline int ltrunc( real x ) { return int_cast(trunc(x)); }
  inline int lround( real x ) { return int_cast(round(x)); }
};

// ��������������� �����

class scale : public scope {
public:
  // ���������������� ������������� ����� x �� ��������� [a,b] � �������� [c,d]
  static real coord( real x, real a, real b, real c, real d ){
    return c + (d-c) * (x-a)/(b-a);
  }

  // ����������� ���� ��� �������� ����� �������
  // ��� ��������������� �� ��������� [a,b] � �������� [c,d]
  static real length( real l, real a, real b, real c, real d ){
    return (d-c) * l / (b-a);
  }

  // �������������� ������������� ����� x �� ��������� [a,b]
  // � ����� ����� �� ��������� [c,d]
  static int interval( real x, real a, real b, int c, int d ){
    int res = int_cast(scale::coord(x, a, b, c, d));
    return (res < c) ? c : ((res > d) ? d : res);
  }
};

// ������ � ���������� ������� �� ������� ����������
// ����� �������� ������ ����������� ������
// ������ ������ ��������� �������� ����������

class intervals : public scope {
public:
  // ������������� ����� x �� ��������� [ax,bx]
  // � ����� ��������� �� ��������� [0, len-1]
  static int val2idx( real x, real ax, real bx, int len );

  // ������������� ����� ��������� �� ��������� [0, len-1]
  // � �����, ��������������� ����� ������� ���������
  static real idx2val( int idx, real ax, real bx, int len );

  // ������������� ����� ��������� �� ��������� [0, len-1]
  // � �����, ��������������� �������� ���������
  static real idx2cnt( int idx, real ax, real bx, int len );
};

// �����, �������������� IEEE-������������� ������������ �����.
// | type   bytes bits mant exp exp.limits  shift interval  digs  eps
// | float  4     32   24   8   -126  +127  127   10e+-38   6     1.19E-07f
// | double 8     64   53   11  -1022 +1023 1023  10e+-308  15    2.22E-16
// ����������� ������� isexactint() ��������� ������������ �����
// �� ����������� ��� ������� ������������� ��� ������.

class ieeereal : public value {
public:
  // ���������������� IEEE-������������� �� �����
  ieeereal( real x = 0.0 ){ set(x); }

  // ���������������� IEEE-������������� �� ����������� �������������
  ieeereal( real mnt, int exp ){ set(mnt, exp); }

  // ���������� ��������
  void set( real x );

  // ���������� ���������� �������������
  void set( real mnt, int exp );

  // �������� ������������ �����
  real get();

  // �������� ��������
  real mant() const { return _mant; }

  // �������� ����������
  int expn() const { return _expn; }

  // ��������� ����� �� ����������� ��� ������� ������������� ��� ������
  static bool is_exactint( real n ){
    real d = n + 1.0;
    return n + 3.0 == d + 2.0;
  }

private:
  real _mant;
  int  _expn;
};

// �������� � ������� �� ���������� �����������.
// �������� ������.

class summer : public value {
public:
  summer( real v = 0.0 ) : _sum(v), _cur(0.0) {}

  void reset( real v = 0.0 ) {
    _sum = v;
    _cur = 0.0;
  }

  real get() const { return _sum; }

  real add( real v ) {
    double y = v - _cur;
    double t = _sum + y;
    _cur = (t - _sum) - y;
    return _sum = t;
  }

private:
  real _sum;
  real _cur;
};

// ���������� �������� � ��������� ��� ��������������� �������������

class minmax : public value {
public:
  minmax() : _is_first(true), _min(0.0), _max(0.0) {}

  void put( real x ){
    if( _is_first ){
      _min = _max = x;
      _is_first = false;
    } else {
      t_minize(_min, x);
      t_maxize(_max, x);
    }
  }

  void reset() { _is_first = true; }

  real min() const { return _min; }
  real max() const { return _max; }

  // ���� �� � ������ �������� ���� ���� ��������
  bool is_ok() const { return !_is_first; }

private:
  bool _is_first;
  real _min, _max;
};

// ���������� �������� ������� �� ���� ������

class line_func : public value {
public:
  line_func( real x1, real y1, real x2, real y2 )
  {
    reset(x1, y1, x2, y2);
  }

  void reset( real x1, real y1, real x2, real y2 ){
    if( x2 == x1 )
      throw ex_sing();

    _x1 = x1;
    _y1 = y1;
    _k = (y2 - y1) / (x2 - x1);
  }

  real operator()( real x ) const { return _y1 + _k * (x - _x1); }

  real inv( real y ){
    if( _k == 0.0 )
      throw ex_sing();

    return _x1 + (y - _y1) / _k;
  }

private:
  real _x1, _y1, _k;
};

// ���������� ������������ ������� �� ���� ������
// ��������������, ��� ����� ������������� ��������
// ��������� x=0.0, x=1.0 � x=2.0.

class quad_func : public value {
public:
  quad_func( real y0, real y1, real y2 )
  {
    reset(y0, y1, y2);
  }

  void reset( real y0, real y1, real y2 ){
    _y0 = y0;
    _d1 = y1 - y0;
    _d2 = 0.5 * (y0 - 2*y1 + y2);
  }

  real operator()( real x ) const { return _y0 + x * (_d2*x + (_d1-_d2)); }

private:
  real _y0, _d1, _d2;
};

// �������-����.
// ������������� ��� ���������� �������-�������� �������.
// �� ���� ���������� �� ������� [0, 1].
// �� ������ ������� ��������� �������� 0.0 � 1.0 ��������������.
// ����������� ������� ��������� �� ������ �������� d1 � d2.

class glue_func : public value {
public:
  glue_func( real d0, real d1 ){
    _a = d0 + d1 - 2.0;
    _b = 3.0 - 2.0 * d0 - d1;
    _c = d0;
  }

  real operator()( real x ) const { return x * (x * (_a * x + _b) + _c); }

private:
  real _a, _b, _c;
};

}; // namespace lwml

#endif // _MDEFS_
