// Простая целочисленная геометрия
// lwml, (c) ltwood

#ifndef _IGEOM_
#define _IGEOM_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

// Класс реализует двумерный вектор с целочисленными координатами
// При конструировании указываются значения координат
// При конструировании по умолчанию получается нулевой вектор

class int_point : public value {
public:
  int_point() : _x(0), _y(0) {}
  int_point( int x, int y ) : _x(x), _y(y) {}

  // получение координат
  int x() const { return _x; }
  int y() const { return _y; }

  // установка значений отдельных координат
  int& x() { return _x; }
  int& y() { return _y; }

  // установка значений всех координат
  void set( int x, int y ){ _x = x; _y = y; }

private:
  int _x, _y;
};

// Класс реализует сегмент [a,b) на целочисленной прямой
// При конструировании указываются границы сегмента
// При конструировании по умолчанию получается пустой сегмент

class int_segment : public value {
public:
  int_segment() : _a(0), _b(-1) {}
  int_segment( int a, int b ) : _a(a), _b(b) {}

  // получение координат концов
  int a() const { return _a; }
  int b() const { return _b; }

  // установка координат отдельных концов
  int& a() { return _a; }
  int& b() { return _b; }

  // установка координат всех концов
  void set( int a, int b ){ _a = a; _b = b; }

  // получение длины сегмента
  int len() const { return is_empty() ? 0 : _b - _a; }

  // сделать сегмент пустым
  void set_empty() { _a = 0; _b = -1; }

  // проверить сегмент на пустоту
  bool is_empty() const { return _a >= _b; }

  // проверить точку на принадлежность сегменту
  bool is_in( int x ) const { return _a <= x && x < _b; }

  // проверить сегмент-аргумент на то, что он лежит
  // целиком внутри данного сегмента
  bool is_subset( const int_segment& arg ) const;

  // проверить сегмент-аргумент на то, что он пересекается с данным
  bool is_cross( const int_segment& arg ) const;

  // пересечь сегмент с сегментом-аргументом
  int_segment& operator*=( const int_segment& arg );

  // добавить сегмент-аргумент к данному сегменту
  // при этом получается минимальный сегмент, включающий оба сегмента
  int_segment& operator+=( const int_segment& arg );

  // вычесть из данного сегмента сегмент-аргумент
  // в результате может получится 2 разных сегмента
  int_segment sub_left( const int_segment& ) const;   // левый сегмент-разность
  int_segment sub_right( const int_segment& ) const;  // правый сегмент-разность

  // растянуть сегмент так, чтобы он включал точку
  int_segment& stretch( int x );

private:
  int _a, _b;
};

// Класс реализует прямоугольник [ax,bx)x[ay,by) на целочисленной плоскости
// При конструировании указываются границы прямоугольника
// При конструировании по умолчанию получается пустой прямоугольник

class int_rect : public value {
public:
  int_rect()
    : _xseg(), _yseg() {}
  int_rect( int ax, int bx, int ay, int by )
    : _xseg(ax, bx), _yseg(ay, by) {}

  // полученить сегменты, соответствующие осям для чтения
  int_segment x() const { return _xseg; }
  int_segment y() const { return _yseg; }

  // полученить сегменты, соответствующие осям для записи
  int_segment& x() { return _xseg; }
  int_segment& y() { return _yseg; }

  // установить все границы как сегменты, соответствующие осям
  void set( const int_segment& x, const int_segment& y ){ _xseg = x; _yseg = y; }

  // получение координат отдельных границ
  int ax() const { return _xseg.a(); }
  int bx() const { return _xseg.b(); }
  int ay() const { return _yseg.a(); }
  int by() const { return _yseg.b(); }

  // установка координат отдельных границ
  int& ax() { return _xseg.a(); }
  int& bx() { return _xseg.b(); }
  int& ay() { return _yseg.a(); }
  int& by() { return _yseg.b(); }

  // покоординатная установка координат всех границ
  void set( int ax, int bx, int ay, int by ){
    _xseg.set(ax, bx);
    _yseg.set(ay, by);
  }

  // получение длины прямоугольника вдоль осей
  int lx() const { return _xseg.len(); }
  int ly() const { return _yseg.len(); }

  // сделать прямоугольник пустым
  void set_empty(){ _xseg.set_empty(); _yseg.set_empty(); }

  // проверить прямоугольник на пустоту
  bool is_empty() const { return _xseg.is_empty() || _yseg.is_empty(); }

  // проверить точку на принадлежность прямоугольнику
  bool is_in( const int_point& p ) const {
    return _xseg.is_in(p.x()) && _yseg.is_in(p.y());
  }

  // проверить точку на принадлежность прямоугольнику
  bool is_in( int x, int y ) const { return _xseg.is_in(x) && _yseg.is_in(y); }

  // проверить прямоугольник-аргумент на то, что он
  // лежит целиком внутри данного прямоугольника
  bool is_subset( const int_rect& r ) const {
    return _xseg.is_subset(r.x()) && _yseg.is_subset(r.y());
  }

  // проверить прямоугольник-аргумент на то, что он
  // пересекается с данным прямоугольником
  bool is_cross( const int_rect& r ) const {
    return _xseg.is_cross(r.x()) && _yseg.is_cross(r.y());
  }

  // пересечь прямоугольник с прямоугольником-аргументом
  int_rect& operator*=( const int_rect& arg );

  // добавить прямоугольник-аргумент к данному прямоугольнику
  // при этом получается минимальный прямоугольник, включающий оба прямоугольника
  int_rect& operator+=( const int_rect&  arg );

  // растянуть прямоугольник так, чтобы он включал точку
  // используется при построении ограничивающего прямоугольника для множества точек
  int_rect& stretch( const int_point& arg );

private:
  int_segment _xseg, _yseg;
};

// implementation

// int_segment

inline bool int_segment::is_subset( const int_segment& arg ) const
{
  return arg.is_empty() || (_a <= arg.a() && arg.b() <= _b );
}

inline bool int_segment::is_cross( const int_segment& arg ) const
{
  return !is_empty() && !arg.is_empty() && t_max<int>(_a, arg.a()) < t_min<int>(_b, arg.b());
}

inline int_segment& int_segment::operator*=( const int_segment& arg )
{
  if( !is_empty() ){
    if( arg.is_empty() )
      set_empty();
    else{
      _a = t_max<int>(_a, arg.a());
      _b = t_min<int>(_b, arg.b());
    }
  }
  return *this;
}

inline int_segment& int_segment::operator+=( const int_segment& arg )
{
  if( !arg.is_empty() ){
    if( is_empty() )
      set(arg.a(), arg.b());
    else{
      _a = t_min<int>(_a, arg.a());
      _b = t_max<int>(_b, arg.b());
    }
  }
  return *this;
}

inline int_segment int_segment::sub_left( const int_segment& arg ) const
{
  if( is_empty() )
    return int_segment();
  if( arg.is_empty() )
    return *this;
  return int_segment(a(), t_min<int>(b(), arg.a()));
}

inline int_segment int_segment::sub_right( const int_segment& arg ) const
{
  if( is_empty() )
    return int_segment();
  if( arg.is_empty() )
    return int_segment();
  return int_segment(t_max<int>(a(), arg.b()), b());
}

inline int_segment& int_segment::stretch( int x )
{
  if( is_empty() ){
    set(x, x + 1);
  } else {
    if( x < a() )
      _a = x;
    if( x >= b() )
      _b = x + 1;
  }
  return *this;
}

// int_rect

inline int_rect& int_rect::operator*=( const int_rect& arg )
{
  _xseg *= arg.x();
  _yseg *= arg.y();
  return *this;
}

inline int_rect& int_rect::operator+=( const int_rect&  arg )
{
  _xseg += arg.x();
  _yseg += arg.y();
  return *this;
}

inline int_rect& int_rect::stretch( const int_point& arg )
{
  _xseg.stretch(arg.x());
  _yseg.stretch(arg.y());
  return *this;
}

}; // namespace lwml

#endif // _IGEOM_
