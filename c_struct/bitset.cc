#include "bitset.h"

/*#lake:stop*/

namespace lwml {

bitset::bitset( int size, bool v )
  : _data((size + 7)/ 8)
{
  _size = size;
  if( v )
    set_one();
  else
    set_zero();
}

void bitset::char_read( const char* buf )
{
  for( int j = 0; j < _data.len(); j++ )
    _data[j] = buf[j];
}

void bitset::char_write( char* buf ) const
{
  for( int j = 0; j < _data.len(); j++ )
    buf[j] = _data[j];
}

void bitset::set_zero()
{
  for( int j = 0; j < _data.len(); j++ )
    _data[j] = 0;
}

void bitset::set_one()
{
  for( int b = 0; b < _size; b++ )
    set(b);
}

void bitset::set( int bit )
{
  #if _CHECK_IDX_
    test_index(bit, _size);
  #endif
  _data[bit/8] |= 1 << (bit%8);
}

void bitset::clear( int bit )
{
  #if _CHECK_IDX_
    test_index(bit, _size);
  #endif
  _data[bit/8] &= ~(1 << (bit%8));
}

void bitset::flip( int bit )
{
  #if _CHECK_IDX_
    test_index(bit, _size);
  #endif
  _data[bit/8] ^= 1 << (bit%8);
}

bool bitset::test( int bit ) const
{
  #if _CHECK_IDX_
    test_index(bit, _size);
  #endif
  return (_data[bit/8] & (1 << (bit%8))) != 0;
}

int bitset::get_max(){
  int i = _size - 1;
  while( i >= 0 ){
    if( test(i) )
      break;
    i--;
  }
  return i;
}

bool bitset::is_meet( const bitset& ls ) const
{
  #if _CHECK_IDX_
    test_size(ls._size, _size);
  #endif
  for( int j = 0; j < _data.len(); j++ )
    if( (_data[j] & ls._data[j]) != 0 )
      return true;
  return false;
}

bitset& bitset::join( const bitset& ls )
{
  #if _CHECK_IDX_
    test_size(ls._size, _size);
  #endif
  for( int j = 0; j < _data.len(); j++ )
    _data[j] |= ls._data[j];
  return *this;
}

bitset& bitset::meet( const bitset& ls )
{
  #if _CHECK_IDX_
    test_size(ls._size, _size);
  #endif
  for( int j = 0; j < _data.len(); j++ )
    _data[j] &= ls._data[j];
  return *this;
}

bitset& bitset::symm_dif( const bitset& ls ){
  #if _CHECK_IDX_
    test_size(ls._size, _size);
  #endif
  for( int j = 0; j < _data.len(); j++ )
    _data[j] ^= ls._data[j];
  return *this;
}

bool bitset::is_empty() const
{
  for( int j = 0; j < _data.len(); j++ ){
    if( _data[j] )
      return false;
  }
  return true;
}

}; // namespace lwml
