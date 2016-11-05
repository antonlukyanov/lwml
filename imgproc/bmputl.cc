#include "bmputl.h"

#include "ivector.h"
#include "histogram.h"
#include "progress.h"
#include "frand.h"
#include "geom.h"

/*#lake:stop*/

namespace lwml {

void bmputil::equalization( matrix& dst, const matrix& src, int hstlen )
{
  test_size2(dst.str(), src.str(), dst.col(), src.col());

  real min = src.min();
  real max = src.max();
  histogram hst(hstlen, min, max);
  for( int y = 0; y < src.str(); y++ ){
    for( int x = 0; x < src.col(); x++ )
      hst.put(src(y, x));
  }
  int_vector distr(hstlen);
  hst.set_integral(distr);
  for( int y = 0; y < src.str(); y ++ ){
    for( int x = 0; x < src.col(); x++ ){
      int idx = intervals::val2idx(src(y, x), min, max, hstlen);
      dst(y, x) = distr[idx];
    }
  }
}

void bmputil::equalization( matrix& src, int hstlen )
{
  equalization(src, src, hstlen);
}

void bmputil::leveling( int_matrix& dst, const matrix& src, int lev )
{
  test_size2(dst.str(), src.str(), dst.col(), src.col());

  real min = src.min();
  real max = src.max();
  for( int y = 0; y < src.str(); y++ ){
    for( int x = 0; x < src.col(); x++ )
      dst(y, x) = intervals::val2idx(src(y, x), min, max, lev);
  }
}

void bmputil::decimation( matrix& dst, const matrix& src )
{
  if( src.str() % dst.str() != 0 || src.col() % dst.col() != 0 )
    fail_assert("incorrect matrix sizes");

  int ny = dst.str();
  int nx = dst.col();
  int decy = src.str() / ny;
  int decx = src.col() / nx;

  for( int y = 0; y < ny; y++ ){
    for( int x = 0; x < nx; x++ ){
      int y0 = y * decy;
      int x0 = x * decx;
      real sum = 0.0;
      for( int dy = 0; dy < decy; dy++ ){
        for( int dx = 0; dx < decx; dx++ ){
          int yy = y0 + dy;
          int xx = x0 + dx;
          sum += src(yy, xx);
        }
      }
      dst(y, x) = sum;
    }
  }
}

void bmputil::thresholding( int_matrix& dst, const matrix& src, real thr )
{
  test_size2(dst.str(), src.str(), dst.col(), src.col());

  real min = src.min();
  real max = src.max();
  real lev = (1.0 - thr) * min + thr * max;

  for( int y = 0; y < src.str(); y++ ){
    for( int x = 0; x < src.col(); x++ ){
      if( src(y, x) > lev )
        dst(y, x) = 1;
      else
        dst(y, x) = 0;
    }
  }
}

void bmputil::clust_thresholding( int_matrix& dst, const matrix& src, int seed )
{
  test_size2(dst.str(), src.str(), dst.col(), src.col());

  frand rnd(seed);
  for( int y = 0; y < dst.str(); y++ ){
    for( int x = 0; x < dst.col(); x++ )
      dst(y, x) = rnd.get_int(2);
  }

  for( int step = 0; step < 100; step++ ){
    real s0 = 0.0;
    int n0 = 0;
    real s1 = 0.0;
    int n1 = 0;
    for( int y = 0; y < dst.str(); y++ ){
      for( int x = 0; x < dst.col(); x++ ){
        if( dst(y, x) ){
          s1 += src(y, x);
          n1++;
        } else {
          s0 += src(y, x);
          n0++;
        }
      }
    }
    if( n0 == 0 || n1 == 0 )
      fail_unexpected();
    s0 /= n0;
    s1 /= n1;
    real tr = s0 + 0.5 * (s1 - s0);

    int ch_num = 0;
    for( int y = 0; y < dst.str(); y++ ){
      for( int x = 0; x < dst.col(); x++ ){
        int new_val = (src(y, x) > tr);
        ch_num += (dst(y, x) != new_val);
        dst(y, x) = new_val;
      }
    }
    if( ch_num == 0 )
      return;
  }

  fail_unexpected();
}

void bmputil::sobel( matrix& dst, const matrix& src )
{
  test_size2(dst.str(), src.str(), dst.col(), src.col());

  int lx = src.col();
  int ly = src.str();
  for( int x = 0; x < lx; x++ ){
    for( int y = 0; y < ly; y++ ){
      real b = (y+1 < ly) ? src(y+1, x) : 0.0;
      real t = (y-1 >= 0) ? src(y-1, x) : 0.0;
      real r = (x+1 < lx) ? src(y, x+1) : 0.0;
      real l = (x-1 >= 0) ? src(y, x-1) : 0.0;
      dst(y, x) = t_max<real>(fabs(b - t), fabs(r - l));
    }
  }
}

void bmputil::filter( matrix& dst, const matrix& src, real cnt, real cross, real diag )
{
  test_size2(dst.str(), src.str(), dst.col(), src.col());

  for( int x = 0; x < src.col(); x++ ){
    for( int y = 0; y < src.str(); y++ ){
      real sum = 0;
      for( int dx = -1; dx <= 1; dx++ ){
        for( int dy = -1; dy <= 1; dy++ ){
          if( x + dx < 0 || x + dx > src.col() - 1 ||
              y + dy < 0 || y + dy > src.str() - 1 )
            continue;
          real val;
          if( dx == 0 && dy == 0 )
            val = cnt;
          else if( dx == 0 || dy == 0 )
            val = cross;
          else
            val = diag;
          sum += src(y+dy, x+dx) * val;
        }
      }
      dst(y, x) = sum;
    }
  }
}

void bmputil::diff( matrix& dst, const matrix& src1, const matrix& src2 )
{
  test_size2(dst.str(), src1.str(), dst.col(), src1.col());
  test_size2(dst.str(), src2.str(), dst.col(), src2.col());

  for( int y = 0; y < src1.str(); y++ ){
    for( int x = 0; x < src1.col(); x++ )
      dst(y, x) = fabs(src1(y, x) - src2(y, x));
  }
}

void bmputil::hist( vector& dst, const matrix& src, int hst_len )
{
  histogram hst(hst_len, src.min(), src.max());
  for( int y = 0; y < src.str(); y++ ){
    for( int x = 0; x < src.col(); x++ ){
      hst.put(src(y, x));
    }
  }
  dst.resize(hst.len());
  for( int j = 0; j < hst.len(); j++ )
    dst[j] = hst[j];
}

void bmputil::cooccuring( int_matrix& dst, const int_matrix& src, int c1, int c2 )
{
  test_size2(dst.str(), src.str(), dst.col(), src.col());

  dst.set_zero();
  for( int y = 0; y < src.str()-1; y++ ){
    for( int x = 0; x < src.col()-1; x++ ){
      if( src(y, x) != c1 ) continue;
      if( src(y+1, x) == c2 ) dst(y, x)++;
      if( src(y, x+1) == c2 ) dst(y, x)++;
    }
  }
}

void bmputil::cooccuring( int_matrix& dst, const int_matrix& src, int dc )
{
  test_size2(dst.str(), src.str(), dst.col(), src.col());

  dst.set_zero();
  for( int y = 0; y < src.str()-1; y++ ){
    for( int x = 0; x < src.col()-1; x++ ){
      if( labs(src(y, x) - src(y+1, x)) == dc ) dst(y, x)++;
      if( labs(src(y, x) - src(y, x+1)) == dc ) dst(y, x)++;
    }
  }
}

void bmputil::counting( matrix& dst, const matrix& src, int nbh_size, tick_mode tick )
{
  test_size2(dst.str(), src.str(), dst.col(), src.col());

  dst.set_zero();
  int ysz = src.str()-nbh_size;
  if( tick == tmON )
    progress::start("counting", ysz);
  for( int y = nbh_size; y < ysz; y++ ){
    int x = nbh_size;
    real sum = 0.0;
    for( int dx = -nbh_size; dx <= nbh_size; dx++ ){
      for( int dy = -nbh_size; dy <= nbh_size; dy++ )
        sum += src(y+dy, x+dx);
    }
    dst(y, x) = sum;
    int left = x - nbh_size;
    int right = x + nbh_size;
    while( right < src.col()-1 ){
      for( int dy = -nbh_size; dy <= nbh_size; dy++ )
        sum -= src(y+dy, left);
      for( int dy = -nbh_size; dy <= nbh_size; dy++ )
        sum += src(y+dy, right+1);
      left++;
      right++;
      x++;
      dst(y, x) = sum;
    }
    if( tick == tmON )
      progress::up(y);
  }
  if( tick == tmON )
    progress::finish();
}

void bmputil::counting( int_matrix& dst, const int_matrix& src, int nbh_size, tick_mode tick )
{
  test_size2(dst.str(), src.str(), dst.col(), src.col());

  dst.set_zero();
  int ysz = src.str()-nbh_size;
  if( tick == tmON )
    progress::start("counting", ysz);
  for( int y = nbh_size; y < ysz; y++ ){
    int x = nbh_size;
    int sum = 0;
    for( int dx = -nbh_size; dx <= nbh_size; dx++ ){
      for( int dy = -nbh_size; dy <= nbh_size; dy++ )
        sum += src(y+dy, x+dx);
    }
    dst(y, x) = sum;
    int left = x - nbh_size;
    int right = x + nbh_size;
    while( right < src.col()-1 ){
      for( int dy = -nbh_size; dy <= nbh_size; dy++ )
        sum -= src(y+dy, left);
      for( int dy = -nbh_size; dy <= nbh_size; dy++ )
        sum += src(y+dy, right+1);
      left++;
      right++;
      x++;
      dst(y, x) = sum;
    }
    if( tick == tmON )
      progress::up(y);
  }
  if( tick == tmON )
    progress::finish();
}

void bmputil::rot( int_matrix& dst, const int_matrix& src, real angle )
{
  test_size2(dst.str(), src.str(), dst.col(), src.col());
  int lx = dst.col();
  int ly = dst.str();

  fpoint cnt(lx/2.0 - 0.5, ly/2.0 - 0.5);
  for( int y = 0; y < ly; y++ ){
    for( int x = 0; x < lx; x++ ){
      fpoint sp = fpoint(x, y).rot(angle, cnt);
      int sx = fpr::lround(sp.x());
      sx = t_max(sx, 0);
      sx = t_min(sx, lx-1);
      int sy = fpr::lround(sp.y());
      sy = t_max(sy, 0);
      sy = t_min(sy, ly-1);
      dst(y, x) = src(sy, sx);
    }
  }
}

}; // namespace lwml
