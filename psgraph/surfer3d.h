// Рисование поверхностей
// lwml, (c) ltwood

//!! TODO: переделать

#ifndef _SURFER3D_
#define _SURFER3D_

#include "defs.h"
#include "mdefs.h"
#include "vp3d.h"
#include "matrix.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// Класс colorer3d - вспомогательный для класса matrix_as_surface.
// Он служит для преобразования значения и освещенности элемента матрицы в цвет.
// Оператор operator() получает значение освещенности в модели Гуро
// и значение элемента матрицы, отнормированное на диапазон [0,1]
// и преобразует их в цвет в модели HSB.
// Получаемое значение освещенности будет отрицательным для тех элементов
// поверхности, для которых видна их нижняя поверхность.

class i_colorer3d : public interface {
public:
  // преобразование освещенности и значения в цвет
  virtual hsbcolor operator()( real brt, real val ) const = 0;
};

// Простейший colorer, преобразующий значения в цвет.
// При конструировании задается тон и насыщенность получаемого на выходе цвета,
// а для яркости задается диапазон, в который линейно преобразуется значение освещенности.
// Флаг revers позволяет обратить яркости: если этот флаг включен,
// то большим значениям соответствуют темные цвета.
// Пергрузка функции mk_color() позволяет задать зависимость тона и насыщенности
// от получаемого значения элемента матрицы.

const real std_simple_colorer3d_blackcolor = 0.1;
const real std_simple_colorer3d_whitecolor = 0.9;

const real COL_UPPOW = 2.0;
const real COL_DNPOW = 3.0;

class simple_colorer3d : public i_colorer3d, public value {
public:
  enum color_mode { POSITIVE, NEGATIVE };

  // для цвета задается тон, насыщенность и диапазон изменения яркости
  // дополнительно задается флаг реверсирования яркостей
  simple_colorer3d(
    real hue = 0.0,
    real satur = 0.0,
    real bc = std_simple_colorer3d_blackcolor,
    real wc = std_simple_colorer3d_whitecolor,
    color_mode cm = POSITIVE
  ){
    _hue = hue;
    _satur = satur;
    _bc = bc;
    _wc = wc;
    _revers = (cm == NEGATIVE);
  }

  virtual hsbcolor operator()( real brt, real val ) const {
    real cb = (brt < 0.0) ? pow(-brt, COL_DNPOW) : pow(brt, COL_UPPOW);
    hsbcolor res = mk_color(val);
    if( _revers )
      res.set_brightness(scale::coord(cb, 0.0, 1.0, _wc, _bc));
    else
      res.set_brightness(scale::coord(cb, 0.0, 1.0, _bc, _wc));
    return res;
  }

  virtual hsbcolor mk_color( real val ) const {
    return hsbcolor(_hue, _satur, 0.0);
  }

private:
  real _hue, _satur;
  real _bc, _wc;
  bool _revers;
};

// Класс реализует рисование поверхностей
// Поверхность задается матрицей значений в равноотстоящих узлах
// Считается, что столбцам соответствует ось Ox, а строкам - Oy,
// причем первой строке соответствует минимальное значение координаты y.
// При конструировании указывается канва, в которой происходит рисование,
// 3-мерная сцена, вмещающая поверхность (см. vp3d.h),
// вектор наблюдения (см. vp3d.h) и флаг, определяющий стиль рисования
// поверхности - линиями (false) или закрашенными гранями (true)
// Функция vp() позволяет получить сконструированный в процессе построения
// поверхности объект класса vp3d.

class matrix_as_surface : public value {
public:
  enum coloring_type { GRAYSCALE, COLOR };

  matrix_as_surface( referer<i_canvas> cv,
    const matrix&,
    fpoint3d viewbox,
    viewpoint3d& viewpoint,
    coloring_type ct = COLOR,
    const i_colorer3d& col = simple_colorer3d()
  );

  referer<viewport3d> vp() { return _vp; }

private:
  referer<viewport3d> _vp;
  viewpoint3d _viewpoint;
  bool _showcolor;

  scene3d calcscene( const matrix&, fpoint3d viewbox );

  // индексация треугольников
  int trnum( const matrix& src ) const { return (src.str()-1)*(src.col()-1)*2; }
  int idx2it( int idx ) const { return idx % 2; }
  int idx2ix( int idx, const matrix& m ) const { return (idx/2) % (m.col() - 1); }
  int idx2iy( int idx, const matrix& m ) const { return (idx/2) / (m.col() - 1); }

  // отрисовка поверхности
  void draw( const matrix& m, const i_colorer3d& col );
  void plottr( const matrix& m, int x, int y, int k, const i_colorer3d& col );

  // окраска поверхности
  real calccol( fpoint3d p1, fpoint3d p2, fpoint3d p3 );

  // сортировка в z-пордке
  class triangler {
  public:
    triangler( const matrix_as_surface&, const matrix&, const scene3d& );

    int len() const { return _len; }
    int comp( int i, int j ) const {
      real vi = _val[i];
      real vj = _val[j];
      return (vi < vj) ? -1 : (vi > vj) ? 1 : 0;
    }

  private:
    int _len;
    vector _val;
    const matrix *_matr;
    const matrix_as_surface *_base;
    const scene3d *_scene;

    real val( int ) const;
  };
  friend class matrix_as_surface::triangler;
};

}; // namespace lwml

#endif // _SURFER3D_
