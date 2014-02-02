#include "cmap.h"

#include "bmp.h"

/*#lake:stop*/

namespace lwml {

void cmap::save( const char* fn, const matrix& src, const i_mapcolorer& map, direction dir )
{
  int ns = src.str();
  int nc = src.col();

  bmp_pal pal;
  bitmap dst(ns, nc, pal);

  for( int s = 0; s < ns; s++ ){
    for( int c = 0; c < nc; c++ ){
      real v = (dir == REVERSE) ? src(ns-1 - s, c) : src(s, c);
      hsbcolor hsb = map(v);
      rgbcolor rgb = color_conv::hsb2rgb(hsb);
      bmp_rgb col(rgb);
      dst.put(s, c, col);
    }
  }

  dst.save(fn);
}

void cmap::save( const char* fn, const int_matrix& src, const i_mapcolorer& map, direction dir )
{
  int ns = src.str();
  int nc = src.col();

  bmp_pal pal;
  bitmap dst(ns, nc, pal);

  for( int s = 0; s < ns; s++ ){
    for( int c = 0; c < nc; c++ ){
      real v = (dir == REVERSE) ? src(ns-1 - s, c) : src(s, c);
      hsbcolor hsb = map(v);
      rgbcolor rgb = color_conv::hsb2rgb(hsb);
      bmp_rgb col(rgb);
      dst.put(s, c, col);
    }
  }

  dst.save(fn);
}

}; // namespace lwml
