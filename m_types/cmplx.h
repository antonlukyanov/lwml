// Комплексная арифметика
// lwml, (c) ltwood

#ifndef _CMPLX_
#define _CMPLX_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"

/*#lake:stop*/

namespace lwml {

class cmplx : public value {
public:
  // Инициализация комплексного числа
  cmplx( real re = 0.0, real im = 0.0 ) : _re(re), _im(im) {}

  // Установка значения
  void set( real re, real im = 0.0 ){
    _re = re;
    _im = im;
  }

  // Получение вещественной части
  const real& re() const { return _re; }

  // Получение мнимой части
  const real& im() const { return _im; }

  // Изменение вещественной части
  // Позволяет использовать операторы присвоения вида c.re() = 6.0;
  // Возвращает ссылку на вещественную часть
  real& re() { return _re; }

  // Изменение мнимой части
  // Позволяет использовать операторы присвоения вида c.im() = 6.0;
  // Возвращает ссылку на мнимую часть
  real& im() { return _im; }

  real norm() const;     // Норма (модуль)
  real mag() const;      // Квадрат нормы
  cmplx conj() const;    // Сопряженное число
  real arg() const;      // Аргумент

  cmplx& operator+=( const cmplx& ); // Добавление комплексного числа
  cmplx& operator-=( const cmplx& ); // Вычитание комплексного числа
  cmplx& operator*=( const cmplx& ); // Домножение на комплексное число
  cmplx& operator/=( const cmplx& ); // Деление на комплексное число
  cmplx& operator*=( real );         // Домножение на вещественное число
  cmplx& operator/=( real );         // Деленеие на вещественное число

  // Деление комплексных чисел
  // c - делимое, d - делитель
  // Возвращает результат деления числа c на число d
  static cmplx div( const cmplx& c, const cmplx& d );

  // Поворот вектора, соответствующего комплексному числу
  // pnt - поворачиваемая точка
  // angle - угол поворота
  // cnt - точка, около которой выполняется поворот
  // Возвращает комплексное число, отвечающее повернутому вектору
  static cmplx rot( const cmplx& pnt, real angle, const cmplx& cnt );

  // Вычисление корня из комплексного числа
  static cmplx sqrt( const cmplx& z );

  // Получение вещественной части заданного комплексного числа
  static real  re( const cmplx& c );
  // Получение мнимой части заданного комплексного числа
  static real  im( const cmplx& c );
  // Вычисление нормы (модуля) заданного комплексного числа
  static real  norm( const cmplx& c );
  // Вычисление квадрата нормы заданного комплексного числа
  static real  mag( const cmplx& c );
  // Получение сопряженного числа для заданного комплексного числа
  static cmplx conj( const cmplx& c );
  // Вычисление аргумента заданного комплексного числа
  static real  arg( const cmplx& c );

  // Вычисление расстояния между комплексными числами
  static real  dist( const cmplx& c1, const cmplx& c2 );

  // Построение комплексного числа по модулю и аргументу
  // r - модуль, t - аргумент
  static cmplx mkpolar( real r, real t );

  // Построение комплексного число с единичной нормой и заданным аргументом
  // t - аргумент
  static cmplx iexp( real t );

  // Вычисление комплексной экспоненты
  static cmplx exp( const cmplx& c );

  // Вычисление вещественной степени комплексного числа.
  // c - комплексное число, возводимое в степень
  // p - вещественный показатель степени
  static cmplx pow( const cmplx& c, real p );

private:
  real _re, _im;
};

// "внешняя" комплексная арифметика

// Получение противоположного по знаку комплексного числа
cmplx operator-( const cmplx& c );

// Сложение комплексных чисел
cmplx operator+( const cmplx& c1, const cmplx& c2 );

// Вычитание комплексных чисел
cmplx operator-( const cmplx& c1, const cmplx& c2 );

// Умножение комплексных чисел
cmplx operator*( const cmplx& c1, const cmplx& c2 );

// Деление комплексных чисел
cmplx operator/( const cmplx& c1, const cmplx& c2 );

// Умножение комплексного числа на вещественное
cmplx operator*( const cmplx& c, real r );

// Умножение вещественного числа на комплексное
cmplx operator*( real r, const cmplx& c );

// Деление комплексного числа на вещественное
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
