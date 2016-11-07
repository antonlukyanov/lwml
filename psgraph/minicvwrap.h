// simple wraper for minicv
// lwml, (c) ltwood

#ifndef _MINICVWRAP_
#define _MINICVWRAP_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/memory/stdmem.h"
#include "lwml/base/refcount.h"

#include "lwml/psgraph/minicv.h"
#include "lwml/psgraph/canvas.h"

/*#lake:stop*/

namespace lwml {

class minicvwrap : public i_canvas {
public:
  minicvwrap( minicv* mcv ) : _mcv(mcv) {}

public:
  static referer<minicvwrap> create( minicv* mcv ){
    return referer<minicvwrap>(new(lwml_alloc) minicvwrap(mcv));
  }

  virtual frect boundrect() const {
    real ax, bx, ay, by;
    _mcv->boundrect(ax, bx, ay, by);
    return frect(ax, bx, ay, by);
  }

  virtual real xunit() const { return _mcv->xunit(); }
  virtual real yunit() const { return _mcv->yunit(); }

  virtual void pixel( real x, real y )    { _mcv->pixel(x, y); }
  virtual void marker( real x, real y )   { _mcv->marker(x, y); }

  virtual void line( real x1, real y1, real x2, real y2 )      { _mcv->line(x1, y1, x2, y2); }
  virtual void box( real x1, real y1, real x2, real y2 )       { _mcv->box(x1, y1, x2, y2); }
  virtual void rectangle( real x1, real y1, real x2, real y2 ) { _mcv->rectangle(x1, y1, x2, y2); }

  virtual void triangle( real x1, real y1, real x2, real y2, real x3, real y3 ){
    _mcv->triangle(x1, y1, x2, y2, x3, y3);
  }
  virtual void facet( real x1, real y1, real x2, real y2, real x3, real y3 ){
    _mcv->facet(x1, y1, x2, y2, x3, y3);
  }

  virtual void set_gray( real graycol )     { _mcv->set_gray(graycol); }
  virtual void set_rgb( rgbcolor rgbcol )   { _mcv->set_rgb(rgbcol.red(), rgbcol.green(), rgbcol.blue()); }
  virtual void set_hsb( hsbcolor hsbcol );

  virtual void puts( real x, real y, const char* str, const align& algn, real alpha );

  virtual void special( real x, real y, int id, const char* cmd ) { _mcv->special(x, y, id, cmd); }

private:
  minicv* _mcv;
};

}; // namespace lwml

#endif // _MINICVWRAP_
