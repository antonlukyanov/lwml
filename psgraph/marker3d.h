// Разметка осей на 3-мерной канве
// lwml, (c) ltwood

#ifndef _MARKER3D_
#define _MARKER3D_

#include "defs.h"
#include "vp3d.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// Класс, инкапсулирующий отрисовку осей, засечек и надписей на 3-мерной канве
// При конструировании указывается канва, в которой происходит рисование
// и длина засечек на осях в единицах высоты текущего шрифта
// Функция show_viewbox() отрисовывает оси координат
// Оси Ox и Oz отрисовываются обычным образом, но ось Oy отрисовывается
// как прямая, параллельная настоящей оси Oy, но при x = max_x
// Функции *grid() и *subscript() работают аналогично соответствующим
// функциям в двумерном случае

const real STD_GRIDSIZE = 0.35;

class vp3d_marker : public value {
public:
  vp3d_marker( referer<viewport3d>, real gsize = STD_GRIDSIZE );

  void show_viewbox();

  void xgrid( real, const char *subs = 0 );
  void ygrid( real, const char *subs = 0 );
  void zgrid( real, const char *subs = 0 );

  void xsubscript(
    const char*, const char*,
    const align& lal = align().left().top(),
    const align& ral = align().right().top()
  );
  void ysubscript(
    const char*, const char*,
    const align& lal = align().left().top(),
    const align& ral = align().right().top()
  );
  void zsubscript(
    const char*, const char*,
    const align& lal = align().left().bot(),
    const align& ral = align().right().bot()
  );

private:
  referer<viewport3d> _vp;
  real _gridsize;

  void gridline( fpoint3d pos, fpoint3d dir, real sz );
};

}; // namespace lwml

#endif // _MARKER3D_
