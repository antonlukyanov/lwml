#include "expfilt.h"

/*#lake:stop*/

namespace lwml {

void expfilt::calc( vector& img, real alpha )
{
  int sz = img.len();
  real sum = 0.0;
  for( int j = 0; j < sz; j++ ){
    sum = alpha * sum + (1.0-alpha) * img[j];
    img[j] = sum;
  }
  sum = 0.0;
  for( int j = sz-1; j >= 0; j-- ){
    sum = alpha * sum + (1.0-alpha) * img[j];
    img[j] = sum;
  }
}

void expfilt::calc( matrix& img, real alpha )
{
  int lx = img.col();
  int ly = img.str();
  for( int s = 0; s < ly; s++ ){
    real sum = 0.0;
    for( int c = 0; c < lx; c++ ){
      sum = alpha * sum + (1.0-alpha) * img(s, c);
      img(s, c) = sum;
    }
    sum = 0.0;
    for( int c = lx-1; c >= 0; c-- ){
      sum = alpha * sum + (1.0-alpha) * img(s, c);
      img(s, c) = sum;
    }
  }
  for( int c = 0; c < lx; c++ ){
    real sum = 0.0;
    for( int s = 0; s < ly; s++ ){
      sum = alpha * sum + (1.0-alpha) * img(s, c);
      img(s, c) = sum;
    }
    sum = 0.0;
    for( int s = ly-1; s >= 0; s-- ){
      sum = alpha * sum + (1.0-alpha) * img(s, c);
      img(s, c) = sum;
    }
  }
}

}; // namespace lwml
