#include "fftutl.h"

/*#lake:stop*/

namespace lwml {

void fftutl::copy_cnt_spectrum( matrix& dst, const matrix& src )
{
  int lx1 = src.col();
  int ly1 = src.str();
  int lx2 = dst.col();
  int ly2 = dst.str();

  if( lx1 >= lx2 || ly1 >= ly2 )
    runtime("index sizes in spectrum copy function");

  int dy = (ly2 - ly1) / 2;
  int dx = (lx2 - lx1) / 2;

  dst.set_zero();

  for( int y = 1; y < ly1; y++ ){
    for( int x = 1; x < lx1; x++ )
      dst(dy+y, dx+x) = src(y, x);
  }

  for( int x = 1; x < lx1; x++ )
    dst(dy, dx+x) = dst(dy+ly1, dx+x) = src(0, x)/2.0;

  for( int y = 1; y < ly1; y++ )
    dst(dy+y, dx) = dst(dy+y, dx+lx1) = src(y, 0)/2.0;

  dst(dy, dx) = dst(dy+ly1, dx) = dst(dy, dx+lx1) = dst(dy+ly1, dx+lx1) = src(0, 0);
}

}; // namespace lwml
