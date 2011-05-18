#include "bitset2d.h"

/*#lake:stop*/

namespace lwml {

void bitset2d::set( int ax, int bx, int ay, int by )
{
  for( int ix = ax; ix <= bx; ix++ ){
    for( int iy = ay; iy <= by; iy++ )
      _bits.set(idx2lidx(ix, iy));
  }
}

void bitset2d::clear( int ax, int bx, int ay, int by )
{
  for( int ix = ax; ix <= bx; ix++ ){
    for( int iy = ay; iy <= by; iy++ )
      _bits.clear(idx2lidx(ix, iy));
  }
}

void bitset2d::flip( int ax, int bx, int ay, int by )
{
  for( int ix = ax; ix <= bx; ix++ ){
    for( int iy = ay; iy <= by; iy++ )
      _bits.flip(idx2lidx(ix, iy));
  }
}

}; // namespace lwml
