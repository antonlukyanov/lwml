// Простая вещественная геометрия
// lwml, (c) ltwood

#ifndef _GEOM_
#define _GEOM_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

// Bandle for values of trigonometric functions of fixed angle.

class sincos {
public:
  sincos( real al = 0.0 ) : _sin(::sin(al)), _cos(::cos(al)) {}
  sincos( real s, real c ) : _sin(s), _cos(c) {}

  void reset( real al ) { _sin = ::sin(al); _cos = ::cos(al); }
  void reset( real s, real c ) { _sin = s; _cos = c; }

  real sin() const { return _sin; }
  real cos() const { return _cos; }

private:
  real _sin, _cos;
};

// Класс реализует двумерный вектор с вещественными координатами
// При конструировании указываются значения координат
// При конструировании по умолчанию получается нулевой вектор
// Класс содержит статические функции norm() и mag() для
// вычисления нормы и магнитуды вектора, переданного как аргумент
// Статическая функция dist() вычисляет расстояние между концами векторов
// Статическая функция polar() создает вектор по его длине и аргументу
// Статическая функция rot() поворачивает точку на угол angle
// около точки cnt
// Статическая функция inner_mul() вычисляет скалярное произведение векторов

typedef class real_point fpoint;

class real_point : public value {
public:
  real_point() : _x(0.0), _y(0.0) {}
  real_point( real x, real y ) : _x(x), _y(y) {}

  // получение координат
  real x() const { return _x; }
  real y() const { return _y; }

  // установка значений отдельных координат
  real& x() { return _x; }
  real& y() { return _y; }

  // установка значений всех координат
  void set( real x, real y ){ _x = x; _y = y; }

  real norm() const;    // норма (длина) вектора
  real mag() const;     // квадрат нормы вектора
  real arg() const;     // угол с осью абсцисс

  // поворот конца вектора как точки на угол angle около точки cnt
  real_point& rot( real angle, const real_point& cnt = real_point(0.0, 0.0) );
  real_point& rot( const sincos& sc, const real_point& cnt = real_point(0.0, 0.0) );

  // простая векторная арифметика
  real_point& operator+=( const real_point& );
  real_point& operator-=( const real_point& );
  real_point& operator*=( real );
  real_point& operator/=( real );

  static real norm( const real_point& v );
  static real mag( const real_point& v );
  static real arg( const real_point& v );
  static real dist( const real_point& v1, const real_point& v2 );

  static real_point polar( real r, real phi );
  static real_point polar( real r, const sincos& sc );

  static real_point rot(
    const real_point& pnt, real angle,
    const real_point& cnt = real_point(0.0, 0.0)
  );
  static real_point rot(
    const real_point& pnt, const sincos& sc,
    const real_point& cnt = real_point(0.0, 0.0)
  );

  static real inner_mul( const real_point& v1, const real_point& v2 );

private:
  real _x, _y;
};

// Класс реализует сегмент [a,b) на вещественной прямой
// При конструировании указываются границы сегмента
// При конструировании по умолчанию получается пустой сегмент

typedef class real_segment fsegment;

class real_segment : public value {
public:
  real_segment() : _a(0), _b(-1) {}
  real_segment( real a, real b ) : _a(a), _b(b) {}

  // получение координат концов
  real a() const { return _a; }
  real b() const { return _b; }

  // установка координат отдельных концов
  real& a() { return _a; }
  real& b() { return _b; }

  // установка координат всех концов
  void set( real a, real b ){ _a = a; _b = b; }

  // получение длины сегмента
  real len() const { return is_empty() ? 0 : _b - _a; }

  // сделать сегмент пустым
  void set_empty() { _a = 0; _b = -1; }

  // проверить сегмент на пустоту
  bool is_empty() const { return _a >= _b; }

  // проверить точку на принадлежность сегменту
  bool is_in( real x ) const { return _a <= x && x < _b; }

  // проверить сегмент-аргумент на то, что он лежит
  // целиком внутри данного сегмента
  bool is_subset( const real_segment& arg ) const;

  // проверить сегмент-аргумент на то, что он пересекается с данным
  bool is_cross( const real_segment& arg ) const;

  // пересечь сегмент с сегментом-аргументом
  real_segment& operator*=( const real_segment& arg );

  // добавить сегмент-аргумент к данному сегменту
  // при этом получается минимальный сегмент, включающий оба сегмента
  real_segment& operator+=( const real_segment& arg );

  // вычесть из данного сегмента сегмент-аргумент
  // в результате может получится 2 разных сегмента
  real_segment sub_left( const real_segment& ) const;   // левый сегмент-разность
  real_segment sub_right( const real_segment& ) const;  // правый сегмент-разность

private:
  real _a, _b;
};

// Класс реализует прямоугольник [ax,bx)x[ay,by) на вещественной плоскости
// При конструировании указываются границы прямоугольника
// При конструировании по умолчанию получается пустой прямоугольник

typedef class real_rect frect;

class real_rect : public value {
public:
  real_rect()
    : _xseg(), _yseg() {}
  real_rect( real ax, real bx, real ay, real by )
    : _xseg(ax, bx), _yseg(ay, by) {}

  // полученить сегменты, соответствующие осям для чтения
  real_segment x() const { return _xseg; }
  real_segment y() const { return _yseg; }

  // полученить сегменты, соответствующие осям для записи
  real_segment& x() { return _xseg; }
  real_segment& y() { return _yseg; }

  // установить все границы как сегменты, соответствующие осям
  void set( const real_segment& x, const real_segment& y ){ _xseg = x; _yseg = y; }

  // получение координат отдельных границ
  real ax() const { return _xseg.a(); }
  real bx() const { return _xseg.b(); }
  real ay() const { return _yseg.a(); }
  real by() const { return _yseg.b(); }

  // установка координат отдельных границ
  real& ax() { return _xseg.a(); }
  real& bx() { return _xseg.b(); }
  real& ay() { return _yseg.a(); }
  real& by() { return _yseg.b(); }

  // покоординатная установка координат всех границ
  void set( real ax, real bx, real ay, real by ){
    _xseg.set(ax, bx);
    _yseg.set(ay, by);
  }

  // получение длины прямоугольника вдоль осей
  real lx() const { return _xseg.len(); }
  real ly() const { return _yseg.len(); }

  // сделать прямоугольник пустым
  void set_empty(){ _xseg.set_empty(); _yseg.set_empty(); }

  // проверить прямоугольник на пустоту
  bool is_empty() const { return _xseg.is_empty() || _yseg.is_empty(); }

  // проверить точку на принадлежность прямоугольнику
  bool is_in( const real_point& p ) const {
    return _xseg.is_in(p.x()) && _yseg.is_in(p.y());
  }

  // проверить точку на принадлежность прямоугольнику
  bool is_in( real x, real y ) const { return _xseg.is_in(x) && _yseg.is_in(y); }

  // проверить прямоугольник-аргумент на то, что он
  // лежит целиком внутри данного прямоугольника
  bool is_subset( const real_rect& r ) const {
    return _xseg.is_subset(r.x()) && _yseg.is_subset(r.y());
  }

  // проверить прямоугольник-аргумент на то, что он
  // пересекается с данным прямоугольником
  bool is_cross( const real_rect& r ) const {
    return _xseg.is_cross(r.x()) && _yseg.is_cross(r.y());
  }

  // пересечь прямоугольник с прямоугольником-аргументом
  real_rect& operator*=( const real_rect& arg );

  // добавить прямоугольник-аргумент к данному прямоугольнику
  // при этом получается минимальный прямоугольник, включающий оба прямоугольника
  real_rect& operator+=( const real_rect&  arg );

private:
  real_segment _xseg, _yseg;
};

// implementation

// real_point

inline real real_point::norm() const {
  return hypot(_x, _y);
}

inline real real_point::mag() const {
  return fsqr(hypot(_x, _y));
}

inline real real_point::arg() const {
  return atan2(_y, _x);
}

inline real_point& real_point::rot( real angle, const real_point& cnt ){
  real_point dp(_x - cnt.x(), _y - cnt.y());
  real sn = sin(angle);
  real cs = cos(angle);
  _x = cnt.x() + dp.x() * cs - dp.y() * sn;
  _y = cnt.y() + dp.x() * sn + dp.y() * cs;
  return *this;
}

inline real_point& real_point::rot( const sincos& sc, const real_point& cnt ){
  real_point dp(_x - cnt.x(), _y - cnt.y());
  _x = cnt.x() + dp.x() * sc.cos() - dp.y() * sc.sin();
  _y = cnt.y() + dp.x() * sc.sin() + dp.y() * sc.cos();
  return *this;
}

inline real_point& real_point::operator+=( const real_point& v ){
  _x += v.x();  _y += v.y();
  return *this;
}

inline real_point& real_point::operator-=( const real_point& v ){
  _x -= v.x();  _y -= v.y();
  return *this;
}

inline real_point& real_point::operator*=( real k ){
  _x *= k;  _y *= k;
  return *this;
}

inline real_point& real_point::operator/=( real k ){
  _x /= k;  _y /= k;
  return *this;
}

inline real real_point::norm( const real_point& v ){
  return v.norm();
}

inline real real_point::mag( const real_point& v ){
  return v.mag();
}

inline real real_point::arg( const real_point& v ){
  return v.arg();
}

inline real real_point::dist( const real_point& v1, const real_point& v2 ){
  return hypot(v1.x() - v2.x(), v1.y() - v2.y());
}

inline real_point real_point::polar( real r, real phi ){
  return real_point(r * cos(phi), r * sin(phi));
}

inline real_point real_point::polar( real r, const sincos& sc ){
  return real_point(r * sc.cos(), r * sc.sin());
}

inline real_point real_point::rot( const real_point& pnt, real angle, const real_point& cnt ){
  real_point res(pnt);
  res.rot(angle, cnt);
  return res;
}

inline real_point real_point::rot( const real_point& pnt, const sincos& sc, const real_point& cnt ){
  real_point res(pnt);
  res.rot(sc, cnt);
  return res;
}

inline real real_point::inner_mul( const real_point& v1, const real_point& v2 ){
  return v1.x() * v2.x() + v1.y() * v2.y();
}

// real_segment

inline bool real_segment::is_subset( const real_segment& arg ) const {
  return arg.is_empty() || (_a <= arg.a() && arg.b() <= _b );
}

inline bool real_segment::is_cross( const real_segment& arg ) const {
  return !is_empty() && !arg.is_empty() && t_max<real>(a(), arg.a()) < t_min<real>(b(), arg.b());
}

inline real_segment& real_segment::operator*=( const real_segment& arg ) {
  if( !is_empty() ){
    if( arg.is_empty() )
      set_empty();
    else{
      _a = t_max<real>(_a, arg.a());
      _b = t_min<real>(_b, arg.b());
    }
  }
  return *this;
}

inline real_segment& real_segment::operator+=( const real_segment& arg ) {
  if( !arg.is_empty() ){
    if( is_empty() )
      set(arg.a(), arg.b());
    else{
      _a = t_min<real>(_a, arg.a());
      _b = t_max<real>(_b, arg.b());
    }
  }
  return *this;
}

inline real_segment real_segment::sub_left( const real_segment& arg ) const {
  if( is_empty() )
    return real_segment();
  if( arg.is_empty() )
    return *this;
  return real_segment(a(), t_min<real>(b(), arg.a()));
}

inline real_segment real_segment::sub_right( const real_segment& arg ) const {
  if( is_empty() )
    return real_segment();
  if( arg.is_empty() )
    return real_segment();
  return real_segment(t_max<real>(a(), arg.b()), b());
}

// real rect

inline real_rect& real_rect::operator*=( const real_rect& arg ) {
  _xseg *= arg.x();
  _yseg *= arg.y();
  return *this;
}

inline real_rect& real_rect::operator+=( const real_rect&  arg ) {
  _xseg += arg.x();
  _yseg += arg.y();
  return *this;
}

}; // namespace lwml

#endif // _GEOM_
