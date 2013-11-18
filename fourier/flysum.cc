#include "flysum.h"

/*#lake:stop*/

namespace lwml {

void flysum::calc( vector& dst, const vector& src, int len2 )
{
  test_size(dst.len(), src.len());
  int n = src.len();

  real sum = 0;
  for( int j = 0; j <= len2; j++ )
    sum += src[j];
  dst[0] = sum;
  for( int j = 1; j < n; j++ ){
    if( j - len2 - 1 >= 0 )
      sum -= src[j - len2 - 1];
    if( j + len2 < n )
      sum += src[j + len2];
    dst[j] = sum;
  }
}

}; // namespace lwml
