#include "lwml/m_base/statfn.h"
#include "lwml/base/basex.h"
#include "lwml/m_base/spfunc.h"

/*#lake:stop*/

namespace lwml {

real statfn::beta( real x, real a, real b )
{
  if( a <= 0.0 || b <= 0.0 )
    fail_assert("inadmissible negative argument");
  if( x <= 0.0 )
    return 0.0;             // НЕ ошибка!
  else if( x >= 1.0 )
    return 1.0;             // НЕ ошибка!
  if( x < (a + 1) / (a + b + 2) )
    return beta_frac(a, b, x);
  else
    return 1 - beta_frac(b, a, 1 - x);
}

real statfn::beta_inv( real p, real a, real b )
{
  if( a <= 0.0 || b <= 0.0 )
    fail_assert("inadmissible negative argument");

  if( p < 0.0 || p > 1.0 )
    fail_assert("argument out of bounds");
  if( p == 0.0 || p == 1.0 ) return p;

  real l = 0.0, r = 1.0, x = 0.5;
  do{
    real fx = beta(x, a, b);
    if( fx > p ) r = x;
    else if( fx < p ) l = x;
    else return x;
    x = l + (r - l)/2.0;
  }while( fabs(r-l) > 2.0 * REAL_EPS );
  return x;
}

// М.Абрамовиц, И.Стиган
// Справочник по специальным функциям (М: Мир, 1979)
// Неполная бета-функция вычисляется с помощью разложения в цепную дробь
//   i_beta(a,b,x) = x^{a}*(1-x)^{b}*fraction / a * beta(a,b),
// где
//               1    d1   d2   d3   d4
//   fraction = ---  ---- ---- ---- ---- ....
//               1+   1+   1+   1+   1+
// Подходящие дроби: A(n) / B(n) где
//   A(n) = (s(n) * A(n-1) + r(n) * A(n-2)) * factor
//   B(n) = (s(n) * B(n-1) + r(n) * B(n-2)) * factor
// и
//   A(-1) = 1, B(-1) = 0, A(0) = s(0), B(0) = 1.
// Здесь s(0) = 0 и при n >= 1 s(n) = 1,
// а r(1) = 1 и при i >= 2
//   r(i) =  m(b-m)x / (a+i-1)(a+i)       когда i = 2m,
//   r(i) = -(a+m)(a+b+m)x / (a+i-1)(a+i) когда i = 2m+1,
// factor - шкалирующий множитель, позволяющий избежать переполнения.
// Итак,
//   A(0) = 0 , B(0) = 1,
//   r(1) = -(a+b)*x / (a+1)
//   A(1) = A(0) + r(1)*A(-1) = r(1) = 1
//   B(1) = B(0) + r(1)*B(-1) = 1

real statfn::beta_frac( real a, real b, real x )
{
  real old_bta = 0.0, factor = 1.0;
  real A0 = 0.0, A1 = 1.0, B0 = 1.0, B1 = 1.0;
  real bta = 1.0, am = a, ai = a;
  real iter = 0.0, r;

  do{
    // часть цикла, вычисляющая нечетные подходящие дроби
    // начинаем с i = 1, iter = 0
    ai += 1.0; // = a+i
    r = -am * (am + b) * x / ((ai - 1.0) * ai);
    // пересчет A и B в два шага
    A0 = (A1 + r * A0) * factor; // i НЕчетно
    B0 = (B1 + r * B0) * factor;
    // часть цикла, вычисляющая нечетные подходящие дроби
    // начинаем с i = 2, iter = 1
    am += 1.0;
    iter += 1.0;
    ai += 1.0;
    r = iter * (b - iter) * x * factor / ((ai - 1.0) * ai);
    A1 = A0 + r * A1; // i четно, A0 и B0 уже шкалированы
    B1 = B0 + r * B1;
    old_bta = bta;
    factor = 1.0 / B1;
    bta = A1 * factor;
  }while( fabs(old_bta - bta) > 2.0*REAL_EPS*(1.0 + t_max<real>(fabs(old_bta), fabs(bta))) );
  return bta * exp(a * log(x) + b * log(1.0 - x) - spfunc::log_beta(a, b)) / a;
}

real statfn::gamma( real x, real shape, real scale )
{
  if( shape <= 0.0 || scale <= 0.0 )
    fail_assert("inadmissible negative argument");

  x *= scale;
  if( x <= 0.0 )
    return 0.0; // НЕ ошибка!
  else if( x < (shape + 1.0) )
    return gamma_ser(x, shape, scale);
  else
    return 1.0 - gamma_frac(x, shape, scale);
}

real statfn::gamma_inv( real p, real shape, real scale )
{
  if( shape <= 0.0 || scale <= 0.0 )
    fail_assert("inadmissible negative argument");

  if( p < 0.0 || p > 1.0 )
    fail_assert("argument out of bounds");
  if( p == 0.0 || p == 1.0 ) return p;

  real l = 0.0, r = shape/2.0;
  while( gamma(r, shape, scale) < p ){
    l = r;
    r += shape/2.0;
  }
  real x = l + (r - l)/2.0;

  do{
    real fx = gamma(x, shape, scale);
    if( fx > p ) r = x;
    else if( fx < p ) l = x;
    else return x;
    x = l + (r - l)/2.0;
  }while( fabs(r-l) > 2.0 * REAL_EPS );
  return x;
}

// М.Абрамовиц, И.Стиган
// Справочник по специальным функциям (М: Мир, 1979)
// Для вычисления неполной гамма функции P(a,x)
// используется ее разложение в ряд Тейлора.

real statfn::gamma_ser( real x, real shape, real scale )
{
  real sum, prev_sum, term, aa = shape;
  long i = 0;

  term = sum = 1.0 / shape;
  do{
    aa += 1.0;
    term *= x / aa;
    prev_sum = sum; sum += term;
    i++;
  }while( fabs(prev_sum - sum) > 2.0*REAL_EPS*(1.0 + t_max<real>(fabs(prev_sum), fabs(sum))) );

  return sum *= exp(-x + shape * log(x) - spfunc::log_gamma(shape));
}

// М.Абрамовиц, И.Стиган
// Справочник по специальным функциям (М: Мир, 1979)
// Для вычисления неполной гамма функции P(a,x)
// используется ее разложение в цепную дробь Лежандра
//   P(a,x)=exp(-x +x*ln(a))*CF/logGamma(a),
// где
//         1    1-a   1   2-a   2
//   CF = ---  ----- --- ----- --- ....
//        x+    1+   x+   1+   x+
// Используются подходящие дроби CF(n) = A(n) / B(n) где
//   A(n) = (s(n) * A(n-1) + r(n) * A(n-2)) * factor
//   B(n) = (s(n) * B(n-1) + r(n) * B(n-2)) * factor
// причем
//   A(-1) = 1, B(-1) = 0, A(0) = s(0), B(0) = 1.
// Здесь
//   s(0) = 0, s(1) = x, r(0) = 0, r(1) = 1,
// так что
//   A(1) = 1.0 * factor, B(1) = r * factor
// и, следовательно,
//   r(i) = k - a  if i = 2k,   k > 0
//   r(i) = k      if i = 2k+1,
//   s(i) = 1      if i = 2k,
//   s(i) = x      if i = 2k+1
// factor - шкалирующий множитель

real statfn::gamma_frac( real x, real shape, real scale )
{
  real old_sum=0.0, factor=1.0;
  real A0=0.0, A1=1.0, B0=1.0, B1=x;
  real sum=1.0/x, z=0.0, ma=0.0-shape, rfact;

  do{
    z += 1.0;
    ma += 1.0;
    // два шага рекуррентной замены A и B
    A0 = (A1 + ma * A0) * factor; // i четно
    B0 = (B1 + ma * B0) * factor;
    rfact = z * factor;
    A1 = x * A0 + rfact * A1; // i НЕчетно, A0 всегда отмасштабировано
    B1 = x * B0 + rfact * B1;
    if( B1 ){ //!! TODO: правильно ли?
      factor = 1.0 / B1;
      old_sum = sum;
      sum = A1 * factor;
    }
  }while( fabs(old_sum - sum) > 2.0*REAL_EPS*(1.0 + t_max<real>(fabs(old_sum), fabs(sum))) );
  return exp(-x + shape * log(x) - spfunc::log_gamma(shape)) * sum;
}

real statfn::gauss( real x )
{
  if( x == 0.0 )
    return 0.5;
  real dfg = gamma(x * x / 2.0, 0.5) / 2.0;
  return 0.5 + (x > 0.0 ? dfg : -dfg);
}

real statfn::gauss_inv( real level )
{
  if( level <= 0.0 && level >= 1.0 )
    fail_assert("argument out of bounds");

  real t = level < 0.5 ? level : 1.0 - level;
  t = sqrt(-2 * log(t));
  real q = t - ((0.010328 * t + 0.802853) * t + 2.515517) /
           (((0.001308 * t + 0.189269) * t + 1.432788) * t + 1);
  return level > 0.5 ? q : -q;
}

real statfn::student( real x, real n )
{
  if( n <= 0.0 )
    fail_assert("inadmissible negative argument");

  if( x == 0.0 )
    return 0.5;
  real z = 0.5 * beta(n / (n + x * x), 0.5 * n, 0.5);
  return (x > 0.0) ? 1.0 - z : z;
}

real statfn::student_inv( real p, real n )
{
  if( p == 0.5 )
    return 0.0;
  real z = 1.0 - 2.0 * p;
  z = beta_inv(fabs(z), 0.5, 0.5*n);
  real q = sqrt(n*z/(1.0-z));
  return (p < 0.5) ? -q : q;
}

real statfn::chi2( real x, real f )
{
  if( f <= 0.0 )
    fail_assert("inadmissible negative argument");
  return gamma(x/2.0, f/2.0);
}

real statfn::chi2_inv( real q, real f )
{
  if( f <= 0.0 )
    fail_assert("inadmissible negative argument");
  return gamma_inv(q, f/2.0);
}

real statfn::fisher( real x, real n1, real n2 )
{
  if( x < 0.0 )
    fail_assert("inadmissible negative argument");
  return 1.0 - beta(n2 / (n2 + n1 * x), 0.5 * n2, 0.5 * n1);
}

real statfn::fisher_inv( real p, real n1, real n2 )
{
  real y = beta_inv(p, 0.5 * n1, 0.5 * n2);
  return n2*y/n1/(1-y);
}

}; // namespace lwml
