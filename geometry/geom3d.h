// Простая 3-мерная вещественная геометрия
// lwml, (c) ltwood

#ifndef _GEOM3D_
#define _GEOM3D_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/geometry/geom.h"

/*#lake:stop*/

namespace lwml {

// Класс реализует 3-мерный вектор
// При конструировании указываются значения координат
// При конструировании по умолчанию получается нулевой вектор
// Функция project() возвращает двумерную проекцию точки на
// плоскость, заданную углами своей нормали
// Класс содержит статические функции norm() и mag() для
// вычисления нормы и магнитуды вектора, переданного как аргумент
// Статическая функция dist() вычисляет расстояние между концами векторов
// Статическая функция polar() создает вектор по его длине и
// углам, которые он образует с осями
// Статические функции rot_x(), rot_y() и rot_z() поворачивает точку
// на угол angle около точки cnt в плоскости, перпендикулярной
// указанной в названии функции оси
// Статическая функция inner_mul() вычисляет скалярное произведение векторов
// Статическая функция vector_mul() вычисляет векторное произведение векторов

typedef class real_point3d fpoint3d;

class real_point3d : public value {
public:
  real_point3d()
    : _x(0.0), _y(0.0), _z(0.0) {}
  real_point3d( real x, real y, real z )
    : _x(x), _y(y), _z(z) {}

  // получение координат
  real x() const { return _x; }
  real y() const { return _y; }
  real z() const { return _z; }

  // установка значений отдельных координат
  real& x() { return _x; }
  real& y() { return _y; }
  real& z() { return _z; }

  // установка значений всех координат
  void set( real x, real y, real z ){
    _x = x; _y = y; _z = z;
  }

  real norm() const;    // норма (длина) вектора
  real mag() const;     // квадрат нормы вектора

  // поворот конца вектора как точки на угол angle около точки cnt
  // в плоскости, перпендикулярной указанной в названии функции оси
  real_point3d& rot_x( real al, const real_point3d& c3 = real_point3d(0.0, 0.0, 0.0) );
  real_point3d& rot_x( const sincos& sc, const real_point3d& c3 = real_point3d(0.0, 0.0, 0.0) );
  real_point3d& rot_y( real al, const real_point3d& c3 = real_point3d(0.0, 0.0, 0.0) );
  real_point3d& rot_y( const sincos& sc, const real_point3d& c3 = real_point3d(0.0, 0.0, 0.0) );
  real_point3d& rot_z( real al, const real_point3d& c3 = real_point3d(0.0, 0.0, 0.0) );
  real_point3d& rot_z( const sincos& sc, const real_point3d& c3 = real_point3d(0.0, 0.0, 0.0) );

  // проектирование вектора на плоскость
  // theta = angle(Pxy(v),Ox); phi = angle(v,Oz)
  real_point project( real theta, real phi );
  real_point project( const sincos& sc_theta, const sincos& sc_phi );

  // простая векторная арифметика
  real_point3d& operator+=( const real_point3d& );
  real_point3d& operator-=( const real_point3d& );
  real_point3d& operator*=( real );
  real_point3d& operator/=( real );

  static real norm( const real_point3d& v );
  static real mag( const real_point3d& v );
  static real dist( const real_point3d& v1, const real_point3d& v2 );

  // theta = angle(Pxy(v),Ox), phi = angle(v,Oz)
  static real_point3d polar( real theta, real phi, real rho );
  static real_point3d polar( const sincos& sc_theta, const sincos& sc_phi, real rho );

  static real_point3d rot_x( const real_point3d& v, real al, const real_point3d c3 = real_point3d(0.0, 0.0, 0.0) );
  static real_point3d rot_x( const real_point3d& v, const sincos& sc, const real_point3d c3 = real_point3d(0.0, 0.0, 0.0) );
  static real_point3d rot_y( const real_point3d& v, real al, const real_point3d c3 = real_point3d(0.0, 0.0, 0.0) );
  static real_point3d rot_y( const real_point3d& v, const sincos& sc, const real_point3d c3 = real_point3d(0.0, 0.0, 0.0) );
  static real_point3d rot_z( const real_point3d& v, real al, const real_point3d c3 = real_point3d(0.0, 0.0, 0.0) );
  static real_point3d rot_z( const real_point3d& v, const sincos& sc, const real_point3d c3 = real_point3d(0.0, 0.0, 0.0) );

  static real inner_mul( const real_point3d& a, const real_point3d& b );

  static real_point3d vector_mul( const real_point3d& a, const real_point3d& b );

  static real_point3d conv_hull( const real_point3d& a, const real_point3d& b, real al );

  static real_point3d sub( const real_point3d& a, const real_point3d& b ); // a - b

private:
  real _x, _y, _z;
};

// implementation

// real_point3d

inline real real_point3d::norm() const {
  return hypot3( _x, _y, _z );
}

inline real real_point3d::mag() const {
  return fsqr(hypot3(_x, _y, _z));
}

inline real_point3d& real_point3d::rot_x( real al, const real_point3d& c3 ){
  real_point yz(_y, _z);
  real_point c2(c3.y(), c3.z());
  yz.rot(al, c2);
  _y = yz.x();
  _z = yz.y();
  return *this;
}

inline real_point3d& real_point3d::rot_x( const sincos& sc, const real_point3d& c3 ){
  real_point yz(_y, _z);
  real_point c2(c3.y(), c3.z());
  yz.rot(sc, c2);
  _y = yz.x();
  _z = yz.y();
  return *this;
}

inline real_point3d& real_point3d::rot_y( real al, const real_point3d& c3 ){
  real_point xz(_x, _z);
  real_point c2(c3.x(), c3.z());
  xz.rot(-al, c2);
  _x = xz.x();
  _z = xz.y();
  return *this;
}

inline real_point3d& real_point3d::rot_y( const sincos& sc, const real_point3d& c3 ){
  real_point xz(_x, _z);
  real_point c2(c3.x(), c3.z());
  xz.rot(sincos(-sc.sin(), sc.cos()), c2);
  _x = xz.x();
  _z = xz.y();
  return *this;
}

inline real_point3d& real_point3d::rot_z( real al, const real_point3d& c3 ){
  real_point xy(_x, _y);
  real_point c2(c3.x(), c3.y());
  xy.rot(al, c2);
  _x = xy.x();
  _y = xy.y();
  return *this;
}

inline real_point3d& real_point3d::rot_z( const sincos& sc, const real_point3d& c3 ){
  real_point xy(_x, _y);
  real_point c2(c3.x(), c3.y());
  xy.rot(sc, c2);
  _x = xy.x();
  _y = xy.y();
  return *this;
}

inline real_point real_point3d::project( real theta, real phi ){
  real_point3d res(*this);
  res.rot_z(M_PI/2.0 - theta);
  res.rot_x(-(M_PI - phi));
  return real_point(-res.x(), res.y());
}

inline real_point real_point3d::project( const sincos& sc_theta, const sincos& sc_phi ){
  real_point3d res(*this);
  res.rot_z(sincos(sc_theta.cos(), sc_theta.sin())); // M_PI/2.0 - theta
  res.rot_x(sincos(-sc_phi.sin(), -sc_phi.cos())); // -(M_PI - phi)
  return real_point(-res.x(), res.y());
}

inline real_point3d& real_point3d::operator+=( const real_point3d& v ){
  _x += v.x();  _y += v.y();  _z += v.z();
  return *this;
}

inline real_point3d& real_point3d::operator-=( const real_point3d& v ){
  _x -= v.x();  _y -= v.y();  _z -= v.z();
  return *this;
}

inline real_point3d& real_point3d::operator*=( real k ){
  _x *= k;  _y *= k;  _z *= k;
  return *this;
}

inline real_point3d& real_point3d::operator/=( real k ){
  _x /= k;  _y /= k;  _z /= k;
  return *this;
}

inline real real_point3d::norm( const real_point3d& v ){
  return v.norm();
}

inline real real_point3d::mag( const real_point3d& v ){
  return v.mag();
}

inline real real_point3d::dist( const real_point3d& v1, const real_point3d& v2 ){
  return hypot3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

inline real_point3d real_point3d::polar( real theta, real phi, real rho ){
  return real_point3d(rho * sin(phi) * cos(theta), rho * sin(phi) * sin(theta), rho * cos(phi));
}

inline real_point3d real_point3d::polar( const sincos& sc_theta, const sincos& sc_phi, real rho ){
  return real_point3d(rho * sc_phi.sin() * sc_theta.cos(), rho * sc_phi.sin() * sc_theta.sin(), rho * sc_phi.cos());
}

inline real_point3d real_point3d::rot_x( const real_point3d& v, real al, const real_point3d c3 ){
  real_point3d res(v);
  res.rot_x(al, c3);
  return res;
}

inline real_point3d real_point3d::rot_x( const real_point3d& v, const sincos& sc, const real_point3d c3 ){
  real_point3d res(v);
  res.rot_x(sc, c3);
  return res;
}

inline real_point3d real_point3d::rot_y( const real_point3d& v, real al, const real_point3d c3 ){
  real_point3d res(v);
  res.rot_y(al, c3);
  return res;
}

inline real_point3d real_point3d::rot_y( const real_point3d& v, const sincos& sc, const real_point3d c3 ){
  real_point3d res(v);
  res.rot_y(sc, c3);
  return res;
}

inline real_point3d real_point3d::rot_z( const real_point3d& v, real al, const real_point3d c3 ){
  real_point3d res(v);
  res.rot_z(al, c3);
  return res;
}

inline real_point3d real_point3d::rot_z( const real_point3d& v, const sincos& sc, const real_point3d c3 ){
  real_point3d res(v);
  res.rot_z(sc, c3);
  return res;
}

inline real real_point3d::inner_mul( const real_point3d& a, const real_point3d& b ){
  return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

inline real_point3d real_point3d::vector_mul( const real_point3d& a, const real_point3d& b ){
  return real_point3d( a.y() * b.z() - a.z() * b.y(),
                       a.z() * b.x() - a.x() * b.z(),
                       a.x() * b.y() - a.y() * b.x() );
}

inline real_point3d real_point3d::conv_hull( const real_point3d& a, const real_point3d& b, real al ){
  real_point3d d(b.x() - a.x(), b.y() - a.y(), b.z() - a.z());
  return real_point3d(a.x() + al * d.x(), a.y() + al * d.y(), a.z() + al * d.z());
}

inline real_point3d real_point3d::sub( const real_point3d& a, const real_point3d& b ){
  return real_point3d(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}

}; // namespace lwml

#endif // _GEOM3D_
