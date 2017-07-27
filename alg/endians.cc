#include "lwml/alg/endians.h"

/*#lake:stop*/

namespace lwml {

typedef union { // little endian
  uint32 dword;
  struct {
    unsigned int byte0 : 8;
    unsigned int byte1 : 8;
    unsigned int byte2 : 8;
    unsigned int byte3 : 8;
  } bytes;
} le_dword;

typedef union { // big endian
  uint32 dword;
  struct {
    unsigned int byte3 : 8;
    unsigned int byte2 : 8;
    unsigned int byte1 : 8;
    unsigned int byte0 : 8;
  } bytes;
} be_dword;

typedef union { // middle endian
  uint32 dword;
  struct {
    unsigned int byte2 : 8;
    unsigned int byte3 : 8;
    unsigned int byte0 : 8;
    unsigned int byte1 : 8;
  } bytes;
} me_dword;

#define COPY_BYTES( dst, src ) \
  dst.bytes.byte0 = src.bytes.byte0; \
  dst.bytes.byte1 = src.bytes.byte1; \
  dst.bytes.byte2 = src.bytes.byte2; \
  dst.bytes.byte3 = src.bytes.byte3

uint32 endian::le2be_32( uint32 x )
{
  le_dword le_x;
  be_dword be_x;
  le_x.dword = x;
  COPY_BYTES(be_x, le_x);
  return be_x.dword;
}

uint32 endian::be2le_32( uint32 x )
{
  be_dword be_x;
  le_dword le_x;
  be_x.dword = x;
  COPY_BYTES(le_x, be_x);
  return le_x.dword;
}

uint32 endian::le2me_32( uint32 x )
{
  le_dword le_x;
  me_dword me_x;
  le_x.dword = x;
  COPY_BYTES(me_x, le_x);
  return me_x.dword;
}

uint32 endian::me2le_32( uint32 x )
{
  me_dword me_x;
  le_dword le_x;
  me_x.dword = x;
  COPY_BYTES(le_x, me_x);
  return le_x.dword;
}

uint32 endian::me2be_32( uint32 x )
{
  me_dword me_x;
  be_dword be_x;
  me_x.dword = x;
  COPY_BYTES(be_x, me_x);
  return be_x.dword;
}

uint32 endian::be2me_32( uint32 x )
{
  be_dword be_x;
  me_dword me_x;
  be_x.dword = x;
  COPY_BYTES(me_x, be_x);
  return me_x.dword;
}

#define FILL_BYTES( dst, src ) \
  dst.bytes.byte0 = (src) & 0x000000ff; \
  dst.bytes.byte1 = ((src) & 0x0000ff00) >> 8; \
  dst.bytes.byte2 = ((src) & 0x00ff0000) >> 16; \
  dst.bytes.byte3 = ((src) & 0xff000000) >> 24

uint32 endian::to_le_32( uint32 x )
{
  le_dword le_x;
  FILL_BYTES(le_x, x);
  return le_x.dword;
}

uint32 endian::to_be_32( uint32 x )
{
  be_dword be_x;
  FILL_BYTES(be_x, x);
  return be_x.dword;
}

uint32 endian::to_me_32( uint32 x )
{
  me_dword me_x;
  FILL_BYTES(me_x, x);
  return me_x.dword;
}

#define MAKE_VAL( src ) \
  src.bytes.byte0 | (src.bytes.byte1 << 8) | \
  (src.bytes.byte2 << 16) | (src.bytes.byte3 << 24)

uint32 endian::from_le_32( uint32 x )
{
  le_dword le_x;
  le_x.dword = x;
  return MAKE_VAL(le_x);
}

uint32 endian::from_be_32( uint32 x )
{
  be_dword be_x;
  be_x.dword = x;
  return MAKE_VAL(be_x);
}

uint32 endian::from_me_32( uint32 x )
{
  me_dword me_x;
  me_x.dword = x;
  return MAKE_VAL(me_x);
}

typedef union { // little endian
  uint16 word;
  struct {
    unsigned int byte0 : 8;
    unsigned int byte1 : 8;
  } bytes;
} le_word;

typedef union { // big endian
  uint16 word;
  struct {
    unsigned int byte1 : 8;
    unsigned int byte0 : 8;
  } bytes;
} be_word;

typedef union { // middle endian
  uint16 word;
  struct {
    unsigned int byte0 : 8;
    unsigned int byte1 : 8;
  } bytes;
} me_word;

#define FILL_BYTES_16( dst, src ) \
  dst.bytes.byte0 = (src) & 0x00ff; \
  dst.bytes.byte1 = ((src) & 0xff00) >> 8;

uint16 endian::to_le_16( uint16 x )
{
  le_word le_x;
  FILL_BYTES_16(le_x, x);
  return le_x.word;
}

uint16 endian::to_be_16( uint16 x )
{
  be_word be_x;
  FILL_BYTES_16(be_x, x);
  return be_x.word;
}

uint16 endian::to_me_16( uint16 x )
{
  me_word me_x;
  FILL_BYTES_16(me_x, x);
  return me_x.word;
}

#define MAKE_VAL_16( src ) \
  src.bytes.byte0 | (src.bytes.byte1 << 8)

uint16 endian::from_le_16( uint16 x )
{
  le_word le_x;
  le_x.word = x;
  return MAKE_VAL_16(le_x);
}

uint16 endian::from_be_16( uint16 x )
{
  be_word be_x;
  be_x.word = x;
  return MAKE_VAL_16(be_x);
}

uint16 endian::from_me_16( uint16 x )
{
  me_word me_x;
  me_x.word = x;
  return MAKE_VAL_16(me_x);
}

uint16 endian::swab( uint16 x )
{
  uchar ch, *pnt;

  pnt = reinterpret_cast<uchar*>(&x);
  ch = pnt[0];
  pnt[0] = pnt[1];
  pnt[1] = ch;
  return x;
}

}; // namespace lwml
