#include "lwml/psgraph/font.h"
#include "lwml/system/chrfont.h"

/*#lake:stop*/

namespace lwml {

font::font()
{
  _top = chrfont::uppermargin;
  _bot = -chrfont::descender;
  _x0 = _y0 = 0.0;
  _cx = _cy = 0.0;
}

real font::mult( real height ) const
{
  return height / (_top + _bot);
}

real font::width( char ch, real height ) const
{
  return mult(height) * chrfont::width[(uchar)ch];
}

real font::width( const char *str, real height ) const
{
  if( str == 0 )
    return 0.0;
  real res = 0.0;
  for( size_t j = 0; j < strlen(str); j++ )
    res += width(str[j], height);
  return res;
}

void font::moveto( real x, real y )
{
  _cx = x;
  _cy = y;
}

void font::lineto( referer<i_canvas> pcv, real x, real y )
{
  pcv->line(_x0 + _cx, _y0 + _cy, _x0 + x, _y0 + y);
  _cx = x;
  _cy = y;
}

void font::put_char( referer<i_canvas> pcv, char ch, real height, real alpha )
{
  real ml = mult(height);
  real cs = cos(alpha);
  real sn = sin(alpha);
  int ofs = chrfont::offset[(uchar)ch];
  for( int j = ofs; chrfont::stroke[j].t != 0; j++ ){
    real x = ml * chrfont::stroke[j].x;
    real y = ml * chrfont::stroke[j].y;
    real rx = x * cs - y * sn;
    real ry = x * sn + y * cs;
    switch( chrfont::stroke[j].t ){
      case 1:
        moveto(rx, ry);
        break;
      case 2:
        lineto(pcv, rx, ry);
        break;
    }
  }
  _x0 += _cx;
  _y0 += _cy;
  _cx = _cy = 0.0;
}

void font::puts(
  referer<i_canvas> pcv,
  real x, real y, const char *str, real height, const align& ta, real alpha
)
{
  if( str == 0 ) return;

  real ml = mult(height);

  real tx = cos(alpha);  // вектор вдоль строки
  real ty = sin(alpha);
  real nx = -ty;         // вектор вверх от строки
  real ny = tx;

  real hl = 0.0;
  if( ta.is_top() )
    hl = -ml * _top;
  else if( ta.is_vcenter() )
    hl = -ml * (_top - (_top+_bot)/2.0);
  else if( ta.is_bot() )
    hl = ml * _bot;
  x += hl * nx;
  y += hl * ny;

  real wd = width(str, height);
  real vl = 0.0;
  if( ta.is_right() )
    vl = -wd;
  else if( ta.is_hcenter() )
    vl = -wd / 2.0;
  x += vl * tx;
  y += vl * ty;

  _x0 = x;
  _y0 = y;

  _cx = _cy = 0.0;
  for( size_t j = 0; j < strlen(str); j++ )
    put_char(pcv, str[j], height, alpha);
}

}; // namespace lwml
