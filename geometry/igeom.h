// ������� ������������� ���������
// lwml, (c) ltwood

#ifndef _IGEOM_
#define _IGEOM_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

// ����� ��������� ��������� ������ � �������������� ������������
// ��� ��������������� ����������� �������� ���������
// ��� ��������������� �� ��������� ���������� ������� ������

class int_point : public value {
public:
  int_point() : _x(0), _y(0) {}
  int_point( int x, int y ) : _x(x), _y(y) {}

  // ��������� ���������
  int x() const { return _x; }
  int y() const { return _y; }

  // ��������� �������� ��������� ���������
  int& x() { return _x; }
  int& y() { return _y; }

  // ��������� �������� ���� ���������
  void set( int x, int y ){ _x = x; _y = y; }

private:
  int _x, _y;
};

// ����� ��������� ������� [a,b) �� ������������� ������
// ��� ��������������� ����������� ������� ��������
// ��� ��������������� �� ��������� ���������� ������ �������

class int_segment : public value {
public:
  int_segment() : _a(0), _b(-1) {}
  int_segment( int a, int b ) : _a(a), _b(b) {}

  // ��������� ��������� ������
  int a() const { return _a; }
  int b() const { return _b; }

  // ��������� ��������� ��������� ������
  int& a() { return _a; }
  int& b() { return _b; }

  // ��������� ��������� ���� ������
  void set( int a, int b ){ _a = a; _b = b; }

  // ��������� ����� ��������
  int len() const { return is_empty() ? 0 : _b - _a; }

  // ������� ������� ������
  void set_empty() { _a = 0; _b = -1; }

  // ��������� ������� �� �������
  bool is_empty() const { return _a >= _b; }

  // ��������� ����� �� �������������� ��������
  bool is_in( int x ) const { return _a <= x && x < _b; }

  // ��������� �������-�������� �� ��, ��� �� �����
  // ������� ������ ������� ��������
  bool is_subset( const int_segment& arg ) const;

  // ��������� �������-�������� �� ��, ��� �� ������������ � ������
  bool is_cross( const int_segment& arg ) const;

  // �������� ������� � ���������-����������
  int_segment& operator*=( const int_segment& arg );

  // �������� �������-�������� � ������� ��������
  // ��� ���� ���������� ����������� �������, ���������� ��� ��������
  int_segment& operator+=( const int_segment& arg );

  // ������� �� ������� �������� �������-��������
  // � ���������� ����� ��������� 2 ������ ��������
  int_segment sub_left( const int_segment& ) const;   // ����� �������-��������
  int_segment sub_right( const int_segment& ) const;  // ������ �������-��������

  // ��������� ������� ���, ����� �� ������� �����
  int_segment& stretch( int x );

private:
  int _a, _b;
};

// ����� ��������� ������������� [ax,bx)x[ay,by) �� ������������� ���������
// ��� ��������������� ����������� ������� ��������������
// ��� ��������������� �� ��������� ���������� ������ �������������

class int_rect : public value {
public:
  int_rect()
    : _xseg(), _yseg() {}
  int_rect( int ax, int bx, int ay, int by )
    : _xseg(ax, bx), _yseg(ay, by) {}

  // ���������� ��������, ��������������� ���� ��� ������
  int_segment x() const { return _xseg; }
  int_segment y() const { return _yseg; }

  // ���������� ��������, ��������������� ���� ��� ������
  int_segment& x() { return _xseg; }
  int_segment& y() { return _yseg; }

  // ���������� ��� ������� ��� ��������, ��������������� ����
  void set( const int_segment& x, const int_segment& y ){ _xseg = x; _yseg = y; }

  // ��������� ��������� ��������� ������
  int ax() const { return _xseg.a(); }
  int bx() const { return _xseg.b(); }
  int ay() const { return _yseg.a(); }
  int by() const { return _yseg.b(); }

  // ��������� ��������� ��������� ������
  int& ax() { return _xseg.a(); }
  int& bx() { return _xseg.b(); }
  int& ay() { return _yseg.a(); }
  int& by() { return _yseg.b(); }

  // �������������� ��������� ��������� ���� ������
  void set( int ax, int bx, int ay, int by ){
    _xseg.set(ax, bx);
    _yseg.set(ay, by);
  }

  // ��������� ����� �������������� ����� ����
  int lx() const { return _xseg.len(); }
  int ly() const { return _yseg.len(); }

  // ������� ������������� ������
  void set_empty(){ _xseg.set_empty(); _yseg.set_empty(); }

  // ��������� ������������� �� �������
  bool is_empty() const { return _xseg.is_empty() || _yseg.is_empty(); }

  // ��������� ����� �� �������������� ��������������
  bool is_in( const int_point& p ) const {
    return _xseg.is_in(p.x()) && _yseg.is_in(p.y());
  }

  // ��������� ����� �� �������������� ��������������
  bool is_in( int x, int y ) const { return _xseg.is_in(x) && _yseg.is_in(y); }

  // ��������� �������������-�������� �� ��, ��� ��
  // ����� ������� ������ ������� ��������������
  bool is_subset( const int_rect& r ) const {
    return _xseg.is_subset(r.x()) && _yseg.is_subset(r.y());
  }

  // ��������� �������������-�������� �� ��, ��� ��
  // ������������ � ������ ���������������
  bool is_cross( const int_rect& r ) const {
    return _xseg.is_cross(r.x()) && _yseg.is_cross(r.y());
  }

  // �������� ������������� � ���������������-����������
  int_rect& operator*=( const int_rect& arg );

  // �������� �������������-�������� � ������� ��������������
  // ��� ���� ���������� ����������� �������������, ���������� ��� ��������������
  int_rect& operator+=( const int_rect&  arg );

  // ��������� ������������� ���, ����� �� ������� �����
  // ������������ ��� ���������� ��������������� �������������� ��� ��������� �����
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
