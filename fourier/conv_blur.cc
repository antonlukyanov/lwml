// Свертка матрицы с произвольным ядром
// lwml, (c) ltwood

#include "lwml/fourier/conv_blur.h"

#include "lwml/fourier/fft2d.h"

namespace lwml {

conv_blur::conv_blur( int ly, int lx, int hly, int hlx )
  : _ly(ly), _lx(lx), _hly(hly), _hlx(hlx),
    _ny2(up2pow2(ly + hly)), _nx2(up2pow2(lx + hlx)),
    _rx(_ny2, _nx2), _ix(_ny2, _nx2),
    _rg(_ny2, _nx2), _ig(_ny2, _nx2)
{
}

void conv_blur::proc( const matrix& src, const i_function2& func, tick_mode tick )
{
  test_size2(src.str(), _ly, src.col(), _lx);

  _rg.set_zero();
  // дискретизируем ядро и находим спектр
  for( int jy = 0; jy < _ny2; jy++ ){
    int iy = (jy < _ny2 / 2) ? jy : jy-_ny2; // отражение
    for( int jx = 0; jx < _nx2; jx++ ){
      int ix = (jx < _nx2 / 2) ? jx : jx-_nx2; // отражение
      _rg(jy, jx) = func(ix, iy);
    }
  }
  _ig.set_zero();
  fft2d::cfft(_rg, _ig, tick);

  _rx.set_zero();
  for( int jy = 0; jy < _ly; jy++ )
    for( int jx = 0; jx < _lx; jx++ )
      _rx(jy, jx) = src(jy, jx);
  _ix.set_zero();
  fft2d::cfft(_rx, _ix, tick);

  // перемножить спектры
  for( int jy = 0; jy < _ny2; jy++ )
    for( int jx = 0; jx < _nx2; jx++ ){
      real re_k = _rg(jy, jx);
      real im_k = _ig(jy, jx);
      //!! NB: быстрое комплексное умножение ??
      real a = (_rx(jy, jx) + _ix(jy, jx)) * (re_k + im_k);
      real b = _rx(jy, jx) * re_k, c = _ix(jy, jx) * im_k;
      _rx(jy, jx) = b - c;
      _ix(jy, jx) = a - b - c;
    }
  fft2d::cifft(_rx, _ix, tick);

  _rx *= (_ny2 * _nx2);
}

void conv_blur::get( matrix& dst ) const
{
  test_size2(dst.str(), _ly, dst.col(), _lx);

  for( int jy = 0; jy < _ly; jy++ )
    for( int jx = 0; jx < _lx; jx++ )
      dst(jy, jx) = _rx(jy, jx);
}

}; // namespace lwml
