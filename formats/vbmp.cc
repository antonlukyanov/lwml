#include "vbmp.h"

#include "limlib.h"
#include "t_utils.h"

/*#lake:stop*/

namespace lwml {

// Переворот картинки в буфере.
// Работает в предположении что img.len() делится на lx без остатка.
void vbmp::revers()
{
  for( int y = 0; y < _ly/2; y++ ){
    for( int x = 0; x < _lx; x++ ){
      int j = y * _lx + x;
      int jj = ((_ly-1) - y) * _lx + x;
      t_swap(_img[jj], _img[j]);
    }
  }
}

// Обращение яркостей картинки в буфере.
void vbmp::col_rev()
{
  int sz = _img.len();
  for( int j = 0; j < sz; j++ )
    _img[j] = 255 - _img[j];
}

vbmp::vbmp( int ly, int lx )
  : _lx(lx), _ly(ly), _img(lx * ly)
{
}

vbmp::vbmp( const char* fn )
{
  if( limlib::is_active() && limlib::getsize(fn, _lx, _ly) ){
    _img.resize(_lx * _ly);
    if( !limlib::load(fn, _img.access_raw_data()) )
      throw ex_limlib("%s, <%s>", limlib::errmsg(), fn);
  } else {
    bmp_header hdr(fn);
    _lx = hdr.width();
    _ly = hdr.height();
    _img.resize(_lx * _ly);

    bitmap src(fn);
    int idx = 0;
    for( int y = 0; y < _ly; y++ ){
      for( int x = 0; x < _lx; x++ )
        _img[idx++] = src.get(y, x).gray();
    }
  }
}

void vbmp::resize( int ly, int lx )
{
  _lx = lx;
  _ly = ly;
  _img.resize(lx * ly);
}

const int BMP_COLORDEPTH = 256;

void vbmp::save( const char* name ) const
{
  bmp_pal pal(BMP_COLORDEPTH);
  bitmap bmp(_ly, _lx, pal);

  int idx = 0;
  for( int s = 0; s < _ly; s++ ){
    for( int c = 0; c < _lx; c++ )
      bmp.put(s, c, _img[idx++]);
  }
  bmp.save(name);
}

int vbmp::get( t_membuf<uchar>& dst ) const
{
  if( dst.len() != _img.len() )
    dst.resize(_img.len());
  t_vcopy(dst, _img);
  return _lx;
}

void vbmp::get( t_membuf2d<uchar>& dst ) const
{
  if( dst.str() != _ly || dst.col() != _lx )
    dst.resize(_ly, _lx);
  for( int y = 0; y < _ly; y++ ){
    int idx0 = _lx * y;
    for( int x = 0; x < _lx; x++ )
      dst(y, x) = _img[idx0 + x];
  }
}

void vbmp::get( matrix& dst ) const
{
  test_size2(dst.str(), _ly, dst.col(), _lx);

  int idx = 0;
  for( int y = 0; y < _ly; y++ ){
    for( int x = 0; x < _lx; x++ )
      dst(y, x) = _img[idx++];
  }
}

void vbmp::get( int_matrix& dst ) const
{
  test_size2(dst.str(), _ly, dst.col(), _lx);

  int idx = 0;
  for( int y = 0; y < _ly; y++ ){
    for( int x = 0; x < _lx; x++ )
      dst(y, x) = _img[idx++];
  }
}

void vbmp::put( const t_membuf<uchar>& src, int lx )
{
  if( src.len() % lx != 0 )
    fail_size(src.len());
  if( _img.len() != src.len() )
    _img.resize(src.len());
  _lx = lx;
  _ly = src.len() / lx;
  t_vcopy(_img, src);
}

void vbmp::put( const t_membuf2d<uchar>& src )
{
  if( _ly != src.str() || _lx != src.col() )
    _img.resize(src.str() * src.col());
  _lx = src.col();
  _ly = src.str();
  for( int y = 0; y < _ly; y++ ){
    int idx0 = _lx * y;
    for( int x = 0; x < _lx; x++ )
      _img[idx0 + x] = src(y, x);
  }
}

void vbmp::put( const matrix& src )
{
  _lx = src.col();
  _ly = src.str();
  if( _lx * _ly != _img.len() )
    _img.resize(_lx * _ly);

  real ac = src.min();
  real bc = src.max();
  if( bc == ac )
    bc += 1.0;
  if( ac == ac + 1.0 || bc == bc + 1.0 )
    throw ex_sing();

  int idx = 0;
  for( int s = 0; s < src.str(); s++ ){
    for( int c = 0; c < src.col(); c++ )
      _img[idx++] = scale::interval(src(s, c), ac, bc, 0, BMP_COLORDEPTH-1);
  }
}

void vbmp::put_noscale( const matrix& src )
{
  _lx = src.col();
  _ly = src.str();
  if( _lx * _ly != _img.len() )
    _img.resize(_lx * _ly);

  real ac = src.min();
  real bc = src.max();
  if( fpr::lround(ac) < 0 || fpr::lround(bc) > BMP_COLORDEPTH-1 )
    runtime("incorrect value range for bitmap");

  int idx = 0;
  for( int s = 0; s < src.str(); s++ ){
    for( int c = 0; c < src.col(); c++ )
      _img[idx++] = fpr::lround(src(s, c));
  }
}

void vbmp::put( const int_matrix& src )
{
  _lx = src.col();
  _ly = src.str();
  if( _lx * _ly != _img.len() )
    _img.resize(_lx * _ly);

  real ac = src.min();
  real bc = src.max();
  if( bc == ac )
    bc += 1.0;
  if( ac == ac + 1.0 || bc == bc + 1.0 )
    throw ex_sing();

  int idx = 0;
  for( int s = 0; s < src.str(); s++ ){
    for( int c = 0; c < src.col(); c++ )
      _img[idx++] = scale::interval(src(s, c), ac, bc, 0, BMP_COLORDEPTH-1);
  }
}

void vbmp::put_noscale( const int_matrix& src )
{
  _lx = src.col();
  _ly = src.str();
  if( _lx * _ly != _img.len() )
    _img.resize(_lx * _ly);

  int ac = src.min();
  int bc = src.max();
  if( ac < 0 || bc > BMP_COLORDEPTH-1 )
    runtime("incorrect value range for bitmap");

  int idx = 0;
  for( int s = 0; s < src.str(); s++ ){
    for( int c = 0; c < src.col(); c++ )
      _img[idx++] = src(s, c);
  }
}

void vbmp::save(
  const char* fn, const t_membuf<uchar>& src, int lx,
  color_mode cm, direction dir
)
{
  vbmp buf;
  buf.put(src, lx);
  if( dir == REVERSE )
    buf.revers();
  if( cm == NEGATIVE )
    buf.col_rev();
  buf.save(fn);
}

void vbmp::save(
  const char* fn, const t_membuf2d<uchar>& src,
  color_mode cm, direction dir
)
{
  vbmp buf;
  buf.put(src);
  if( dir == REVERSE )
    buf.revers();
  if( cm == NEGATIVE )
    buf.col_rev();
  buf.save(fn);
}

void vbmp::save(
  const char* fn, const matrix& src,
  color_mode cm, direction dir
)
{
  vbmp buf;
  buf.put(src);
  if( dir == REVERSE )
    buf.revers();
  if( cm == NEGATIVE )
    buf.col_rev();
  buf.save(fn);
}

void vbmp::save(
  const char* fn, const int_matrix& src,
  color_mode cm, direction dir
)
{
  vbmp buf;
  buf.put(src);
  if( dir == REVERSE )
    buf.revers();
  if( cm == NEGATIVE )
    buf.col_rev();
  buf.save(fn);
}

}; // namespace lwml
