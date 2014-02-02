#include "fft2d.h"

#include "progress.h"
#include "fft.h"

/*#lake:stop*/

namespace lwml {

void fft2d::cfft( matrix& rm, matrix& im, tick_mode tick )
{
  test_size2(rm.str(), im.str(), rm.col(), im.col());
  int ns = rm.str();
  int nc = rm.col();

  int steps = nc + ns;
  if( tick == tmON )
    progress::start("dft2d", steps);

  // обработка столбцов
  vector rcv(ns);
  vector icv(ns);
  for( int c = 0; c < nc; c++ ){
    rm.get_col(rcv, c);
    im.get_col(icv, c);
    fft::cfft(rcv, icv);
    rm.put_col(rcv, c);
    im.put_col(icv, c);
    if( tick == tmON )
      progress::up(c);
  }

  // обработка строк
  vector rsv(nc);
  vector isv(nc);
  for( int s = 0; s < ns; s++ ){
    rm.get_str(rsv, s);
    im.get_str(isv, s);
    fft::cfft(rsv, isv);
    rm.put_str(rsv, s);
    im.put_str(isv, s);
    if( tick == tmON )
      progress::up(nc + s);
  }

  if( tick == tmON )
    progress::finish();
}

void fft2d::cifft( matrix& rm, matrix& im, tick_mode tick )
{
  test_size2(rm.str(), im.str(), rm.col(), im.col());
  int ns = rm.str();
  int nc = rm.col();

  int steps = ns + nc;
  if( tick == tmON )
    progress::start("idft2d", steps);

  vector rsv(nc);
  vector isv(nc);
  for( int s = 0; s < ns; s++ ){
    rm.get_str(rsv, s);
    im.get_str(isv, s);
    fft::cifft(rsv, isv);
    rm.put_str(rsv, s);
    im.put_str(isv, s);
    if( tick == tmON )
      progress::up(s);
  }

  vector rcv(ns);
  vector icv(ns);
  for( int c = 0; c < nc; c++ ){
    rm.get_col(rcv, c);
    im.get_col(icv, c);
    fft::cifft(rcv, icv);
    rm.put_col(rcv, c);
    im.put_col(icv, c);
    if( tick == tmON )
      progress::up(ns + c);
  }

  if( tick == tmON )
    progress::finish();
}

void fft2d::disturb( matrix& m )
{
  for( int i = 0; i < m.str(); i++ )
    for( int j = 0; j < m.col(); j++ )
      if( (i+j) % 2 == 1 )
        m(i, j) = -m(i, j);
}

}; // namespace lwml
