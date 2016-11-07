#include "lwml/alg/search.h"

/*#lake:stop*/

namespace lwml {

int search::element( const int_vector& src, int el )
{
  int a = 0;
  int b = src.len() - 1;
  while( a <= b ){
    int c = (a + b) / 2;
    if( el == src[c] )
      return c;
    if( el < src[c] )
      b = c-1;
    else
      a = c+1;
  }
  return -1;
}

int search::element( const int_vector& src, const t_sorter<int_vector>& idx, int el )
{
  test_size(src.len(), idx.len());

  int a = 0;
  int b = src.len() - 1;
  while( a <= b ){
    int c = (a + b) / 2;
    if( el == src[idx[c]] )
      return c;
    if( el < src[idx[c]] )
      b = c-1;
    else
      a = c+1;
  }
  return -1;
}

int search::interval( const vector& src, real el )
{
  int a = 0;
  int b = src.len() - 1;
  if( el < src[a] || el > src[b] )
    return -1;
  while( b - a > 1 ){
    int c = (a + b) / 2;
    if( el >= src[c] )
      a = c;
    else
      b = c;
  }
  return a;
}

int search::interval( const vector& src, const t_sorter<vector>& idx, real el )
{
  test_size(src.len(), idx.len());

  int a = 0;
  int b = src.len() - 1;
  if( el < src[idx[a]] || el > src[idx[b]] )
    return -1;
  while( b - a > 1 ){
    int c = (a + b) / 2;
    if( el >= src[idx[c]] )
      a = c;
    else
      b = c;
  }
  return a;
}

} // namespace lwml
