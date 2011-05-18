// 3-мерная графика
// lwml, (c) ltwood

//!! TODO: переделать

#ifndef _VP3D_
#define _VP3D_

#include "defs.h"
#include "mdefs.h"
#include "viewport.h"
#include "geom.h"
#include "geom3d.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// Класс scene3d представляет 3-мерную сцену, которая реализует
// преобразование из предметных координат в координаты сцены
// Предметные координаты могут иметь диапазон изменения,
// естественный для отрисовываемого объекта
// Границы изменения предметных координат определяются точками
// ax, bx т.е. например координата x изменяется от ax.x() до bx.x()
// Диапазон изменения координат сцены определяет соотношения
// сторон отображаемого объекта
// Координаты сцены изменяются от 0 до координат точки vb
// При конструировании указываются границы предметных координат (ax, bx)
// и координат сцены (vb)

class scene3d : public value {
public:
  scene3d( fpoint3d ax, fpoint3d bx, fpoint3d vb )
    : _ax(ax), _bx(bx), _vb(vb) {}

  // оператор преобразования предметных координат 
  // в координаты сцены
  fpoint3d operator()( fpoint3d ) const;

  // получение границ изменения координат
  fpoint3d viewbox() const { return _vb; }  // сцены
  fpoint3d ax() const { return _ax; }       // предметных
  fpoint3d bx() const { return _bx; }

private:
  fpoint3d _ax, _bx;
  fpoint3d _vb;
};

inline fpoint3d scene3d::operator()( fpoint3d pnt ) const {
  real sx = scale::coord(pnt.x(), _ax.x(), _bx.x(), 0.0, _vb.x());
  real sy = scale::coord(pnt.y(), _ax.y(), _bx.y(), 0.0, _vb.y());
  real sz = scale::coord(pnt.z(), _ax.z(), _bx.z(), 0.0, _vb.z());
  return fpoint3d(sx, sy, sz);
}

// Класс viewpoint3d реализует проектирование 3-мерных точек
// в координатах сцены на заданную плоскость "экрана"
// При конструировании указывается расположение плоскости экрана
// путем задания нормали - направления "взгляда"
// Направление взгляда задается углами right и up
// Считается, что оси X и Y расположены в горизонтальной плоскости,
// а ось Z перпендикулярна им и направлена вертикально вверх
// Если right = up = 0, то взгляд направлен вдоль оси Y
// Угол right определяет поворот точки наблюдения вправо, так, что
// при этом сцена поворачивается по часовой стелке вокруг оси Z
// Угол up определяет подъем точки наблюдения вверх
// Все углы измеряются в долях прямого угла
// Проектор умеет также вычислять "глубину" точки - расстояние
// от точки до плоскости экрана в предположении, что плоскость
// экрана проходит через ноль и если точка расположена за экраном,
// то расстояние от нее до экрана отрицательно

class viewpoint3d : public value {
public:
  viewpoint3d( real right, real up ){
    _theta = M_PI_2 * (-1.0 + right);
    _phi = M_PI_2 * (1.0 - up);
    _pv = fpoint3d::polar(_theta, _phi, 1.0);
  }

  // оператор проектирования на экран
  fpoint operator()( fpoint3d ) const;

  // вычисление "глубины" точки
  real depth( fpoint3d ) const;

  // получение вектора проектирования (направление взгляда)
  fpoint3d prj_vect() const { return _pv; }

private:
  real _theta, _phi;
  fpoint3d _pv;
};

inline fpoint viewpoint3d::operator()( fpoint3d pnt ) const {
  return pnt.project(_theta, _phi);
}

inline real viewpoint3d::depth( fpoint3d pnt ) const {
  return fpoint3d::inner_mul(prj_vect(), pnt);
}

// Канва для отрисовки 3-мерной графики
// При конструировании задается обычная канва назначения i_canvas,
// сцена scene3d для преобразования предметных координат в
// координаты сцены, класс-проектор viewpoint3d и ширина
// защитной полосы (border) в единицах высоты шрифта
// Большинство методов рисования аналогичны методам класса i_canvas,
// но работают с 3-мерными координатами точек, спрятанными в fpoint3d
// Здесь описаны только существенные отличия
// 1. Функции scene() и viewpoint() позволяют получить ссылки на сцену и проектор
// 2. Функция prj() производит проектирование 3-мерной точки на плоскость
// 3. Функция cv() позволяет получить ссылку на 2-мерную канву рисования
// 4. При выводе строки указывается 3-мерное направление, которое
// проектируется на плоскость для получения направления вывода текста

class viewport3d : public refcount {
private:
  viewport3d( referer<i_canvas>, scene3d sc, viewpoint3d, real border );

public:
  static referer<viewport3d> create( referer<i_canvas> cv, scene3d sc, viewpoint3d vpnt, real border = 1.0 ){
    return referer<viewport3d>(new(lwml_alloc) viewport3d(cv, sc, vpnt, border));
  }

  void set_gray( real g )      { _cv->set_gray(g); }
  void set_rgb( rgbcolor rgb ) { _cv->set_rgb(rgb); }
  void set_hsb( hsbcolor hsb ) { _cv->set_hsb(hsb); }

  void pixel( fpoint3d );
  void marker( fpoint3d );
  void line( fpoint3d, fpoint3d );
  void box( fpoint3d, fpoint3d );
  void rectangle( fpoint3d, fpoint3d );
  void triangle( fpoint3d, fpoint3d, fpoint3d );
  void facet( fpoint3d, fpoint3d, fpoint3d );
  void puts( fpoint3d, const char *str, const align&, fpoint3d );

  void reset();
  void point( fpoint3d );

  const scene3d& scene() const { return _sc; }

  const viewpoint3d& viewpoint() const { return _vp; }

  referer <i_canvas> cv() { return _cv; }

  fpoint prj( fpoint3d ) const;

private:
  scene3d _sc;
  viewpoint3d _vp;
  referer<viewport> _cv;

  frect calcbound( scene3d sc, viewpoint3d vp );
  frect calcrect( referer<i_canvas> cv, real gsize );
};

inline fpoint viewport3d::prj( fpoint3d pnt ) const {
  return _vp(_sc(pnt));
}

}; // namespace lwml

#endif // _VP3D_
