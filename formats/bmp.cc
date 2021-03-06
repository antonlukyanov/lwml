#include "bmp.h"
#include "stdmem.h"
#include "filename.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

// constants

// file header offsets
const int MAGIC_OFFSET            = 0x00;
const int IMAGEOFFSET_OFFSET      = 0x0a;

// image header offsets
const int IMAGEHEADERSIZE_OFFSET  = 0x0e;
const int WIDTH_OFFSET            = 0x12;
const int HEIGHT_OFFSET           = 0x16;
const int PLANES_OFFSET           = 0x1a;
const int BITSPERPIXEL_OFFSET     = 0x1c;
const int COMPRESSION_OFFSET      = 0x1e;
const int IMAGESIZE_OFFSET        = 0x22;

// std values
const int FILEHEADERSIZE          = 14;
const int MAGIC                   = 0x4D42; // magic word = "BM"
const int PLANES                  = 1;
const int PAL_ELEM_SIZE           = 4;

// supported values
const int NO_COMPRESSION          =  0;

// image header sizes for different bitmap versions
const int IHSZ_BITMAPCOREHEADER   = 12;
const int IHSZ_BITMAPINFOHEADER   = 40;
const int IHSZ_BITMAPCOREHEADER2  = 64;
const int IHSZ_BITMAPV2INFOHEADER = 52;
const int IHSZ_BITMAPV3INFOHEADER = 56;
const int IHSZ_BITMAPV4HEADER     = 108;
const int IHSZ_BITMAPV5HEADER     = 124;

// bmp_pal

void bmp_pal::read( referer<stream> file, int size, int offset )
{
  if( size == 0 )
    _len = 0;
  else{
    _len = size;
    file->seek(offset);
    file->read(_pal, size * PAL_ELEM_SIZE);
  }
}

void bmp_pal::write( referer<stream> file, int offset ) const
{
  if( _len != 0 ){
    file->seek(offset);
    file->write(_pal, _len * PAL_ELEM_SIZE);
  }
}

bmp_pal::bmp_pal( int size )
{
  if( size < 0 )
    runtime("incorrect palette size (%d)", size);
  if( size > MAXPALSIZE )
    runtime("too long bitmap palette (%d)", size);
  _len = size;
  if( _len != 0 ){
    for( int j = 0; j < _len; j++ ){
      uint c = (j * (MAXPALSIZE-1)) / (_len-1);
      _pal[j] = (c << 16) | (c << 8) | c;
    }
  }
}

bmp_pal::bmp_pal( const char* fname, int sz, int offset )
{
  if( sz < 0 )
    runtime("negative palette size (%d)", sz);
  if( sz > MAXPALSIZE )
    runtime("too long bitmap palette (%d)", sz);
  referer<stream> file = stream::create(fname, fmREAD, fmBINARY);
  read(file, sz, offset);
}

bmp_rgb bmp_pal::get( int idx ) const
{
  test_index(idx, _len);
  uint col = _pal[idx];
  return bmp_rgb((col & 0xff0000) >> 16, (col & 0x00ff00) >> 8, col & 0x0000ff);
}

void bmp_pal::put( int idx, bmp_rgb rgb )
{
  test_index(idx, _len);
  _pal[idx] = (rgb.red() << 16) | (rgb.green() << 8) | rgb.blue();
}

// bmp_header

void bmp_header::write16( referer<stream> file, int x ) const
{
  uint16 buf = x;
  file->write(&buf, sizeof(buf));
}

void bmp_header::write32( referer<stream> file, int x ) const
{
  uint32 buf = x;
  file->write(&buf, sizeof(buf));
}

int bmp_header::read16( referer<stream> file, int ofs )
{
  uint16 x;
  file->seek(ofs);
  file->read(&x, sizeof(x));
  return x;
}

int bmp_header::read32( referer<stream> file, int ofs )
{
  uint32 x;
  file->seek(ofs);
  file->read(&x, sizeof(x));
  return x;
}

// ??????????? ????:
//   magic, image header size, planes, compression
// ???????????? ????:
//   file size, reserved, image size,
//   xbitspermeter, xbitspermeter,
//   colors, importantcolors,
// ?????? ????:
//   image offset, width, height, bits per pixel

void bmp_header::write( referer<stream> file ) const
{
  write16(file, MAGIC);                          // magic
  write32(file, 0);                              // file size
  write32(file, 0);                              // reserved
  write32(file, _imageoffset);                   // image offset
  write32(file, _imageheadersize);               // image header size
  write32(file, _width);                         // width
  write32(file, _height);                        // height
  write16(file, PLANES);                         // planes
  write16(file, _bitsperpixel);                  // bits per pixel
  write32(file, NO_COMPRESSION);                 // compression
  write32(file, 0);                              // image size
  write32(file, 0);                              // xbitspermeter
  write32(file, 0);                              // ybitspermeter
  write32(file, 0);                              // colors
  write32(file, 0);                              // importantcolors
}

void bmp_header::read( referer<stream> file )
{
  if( read16(file, MAGIC_OFFSET) != MAGIC )
    throw ex_bmp("wrong magic word");

  _imageheadersize = read32(file, IMAGEHEADERSIZE_OFFSET);
  if(
    _imageheadersize != IHSZ_BITMAPCOREHEADER && _imageheadersize != IHSZ_BITMAPINFOHEADER &&
    _imageheadersize != IHSZ_BITMAPCOREHEADER2 && _imageheadersize != IHSZ_BITMAPV2INFOHEADER &&
    _imageheadersize != IHSZ_BITMAPV3INFOHEADER && _imageheadersize != IHSZ_BITMAPV4HEADER &&
    _imageheadersize != IHSZ_BITMAPV5HEADER
  )
    throw ex_bmp("unknown bitmap type");
  if( _imageheadersize < IHSZ_BITMAPINFOHEADER )
    throw ex_bmp("unsupported bitmap type");

  if( read16(file, PLANES_OFFSET) != PLANES )
    throw ex_bmp("wrong planes number");

  if( read32(file, COMPRESSION_OFFSET) != NO_COMPRESSION )
    throw ex_bmp("can't read compressed bitmap");

  _imageoffset = read32(file, IMAGEOFFSET_OFFSET);
  _palsize = (_imageoffset - (FILEHEADERSIZE + _imageheadersize)) / PAL_ELEM_SIZE;
  _width = read32(file, WIDTH_OFFSET);
  _height = read32(file, HEIGHT_OFFSET);
  _bitsperpixel = read16(file, BITSPERPIXEL_OFFSET);
  _bpl = width2bpl(_width);
}

int bmp_header::width2bpl( int width )
{
  int bpl = 0; // has no mean!
  switch( _bitsperpixel ){
    case 1:
      bpl = (width + 7) / 8;
      break;
    case 4:
      bpl = (width + 1) / 2;
      break;
    case 8:
      bpl = width;
      break;
    case 24:
      bpl = width * 3;
      break;
    case 32:
      bpl = width * 4;
      break;
    default:
      fail_unexpected();
  }
  return (bpl + 3) & ~0x03;
}

int bmp_header::palsize2bpp( int ps )
{
  switch( ps ){
    case 0:   return 24;
    case 2:   return 1;
    case 16:  return 4;
    case 256: return 8;
    default:
      runtime("incorrect palette size (%d) but may be 0, 2, 16, 256 only", ps);
      return 0; // has no mean!
  }
}

bmp_header::bmp_header( const char* fname )
{
  referer<stream> file = stream::create(fname, fmREAD, fmBINARY);
  read(file);
}

bmp_header::bmp_header( int height, int width, int palsize )
{
  _imageheadersize = IHSZ_BITMAPINFOHEADER;
  _imageoffset = FILEHEADERSIZE + _imageheadersize + palsize * PAL_ELEM_SIZE;
  _palsize = palsize;
  _width = width;
  _height = height;
  _bitsperpixel = palsize2bpp(_palsize);
  _bpl = width2bpl(_width);
}

int bmp_header::paloffset() const
{
  return FILEHEADERSIZE + _imageheadersize;
}

// bitmap

uchar bitmap::read_byte( referer<stream> file )
{
  uchar x;
  file->read(&x, sizeof(x));
  return x;
}

void bitmap::write_byte( referer<stream> file, uchar x ) const
{
  file->write(&x, sizeof(x));
}

void bitmap::read( referer<stream> file, int offset )
{
  file->seek(offset);
  int byte_num = _hdr.bytesperline() * _hdr.height();
  for( int j = 0; j < byte_num; j++ )
    _image[j] = read_byte(file);
}

void bitmap::write( referer<stream> file, int offset ) const
{
  file->seek(offset);
  int byte_num = _hdr.bytesperline() * _hdr.height();
  for( int j = 0; j < byte_num; j++ )
    write_byte(file, _image[j]);
}

bitmap::bitmap( const char* fname )
  : _hdr(fname),
    _pal(fname, _hdr.palsize(), _hdr.paloffset()),
    _image(_hdr.bytesperline() * _hdr.height())
{
  referer<stream> file = stream::create(fname, fmREAD, fmBINARY);
  read(file, _hdr.image_offset());
}

bitmap::bitmap( int height, int width, const bmp_pal& pal )
  : _hdr(height, width, pal.len()),
    _pal(pal),
    _image(_hdr.bytesperline() * _hdr.height())
{
}

void bitmap::save( const char* name ) const
{
  referer<stream> file = stream::create(name, fmWRITE, fmBINARY);
  _hdr.save(file);
  _pal.save(file, _hdr.paloffset());
  write(file, _hdr.image_offset());
}

uchar bitmap::getcolidx( int lidx, int x ) const
{
  uchar sh;
  int bpp = _hdr.bitsperpixel();
  switch( bpp ){
    case 1:
      lidx += x / 8;
      sh = 7 - x % 8;
      return (_image[lidx] & (1 << sh)) >> sh;
    case 4:
      lidx += x / 2;
      sh = (x & 1) ? 0 : 4;
      return (_image[lidx] & (0x0f << sh)) >> sh;
    case 8:
      lidx += x;
      return _image[lidx];
    default:
      fail_unexpected();
      return 0; // has no mean!
  }
}

void bitmap::setcolidx( int lidx, int x, int cidx )
{
  uchar sh;
  int bpp = _hdr.bitsperpixel();
  switch( bpp ){
    case 1:
      lidx += x / 8;
      sh = 7 - x % 8;
      _image[lidx] &= ~(1 << sh);
      _image[lidx] |= (cidx & 1) << sh;
      break;
    case 4:
      lidx += x / 2;
      sh = (x&1) ? 0 : 4;
      _image[lidx] &= ~(0x0f << sh);
      _image[lidx] |= (cidx & 0x0f) << sh;
      break;
    case 8:
      lidx += x;
      _image[lidx] = (cidx & 0x00FF);
      break;
    default:
      fail_unexpected();
  }
}

bmp_rgb bitmap::get( int y, int x ) const
{
  int lx = _hdr.width();
  int ly = _hdr.height();
  test_index2(x, lx, y, ly);

  // ????????? ? ???????????? ??? bmp ??????? ?????????
  y = _hdr.height() - y - 1;

  // ????????? ???????? ??????
  int lidx = y * _hdr.bytesperline();

  const int bpp = _hdr.bitsperpixel();

  if( bpp == 24 || bpp == 32 ){
    lidx += x * (bpp / 8); // ????????? ???????? RGB
    return bmp_rgb(_image[lidx+2], _image[lidx+1], _image[lidx]);
  }else{
    uchar cidx = getcolidx(lidx, x); // ???????? ?????? ????? ?????
    return _pal.get(cidx);
  }
}

void bitmap::put( int y, int x, bmp_rgb rgb )
{
  int lx = _hdr.width();
  int ly = _hdr.height();
  test_index2(x, lx, y, ly);
  if( _hdr.bitsperpixel() != 24 && _hdr.bitsperpixel() != 32 )
    fail_assert("can't set RGB for image with palette");

  // ????????? ? ???????????? ??? bmp ??????? ?????????
  y = _hdr.height() - y - 1;

  // ????????? ???????? ??????
  int lidx = y * _hdr.bytesperline();

  lidx += x * (_hdr.bitsperpixel() / 8); // ????????? ???????? RGB
  _image[lidx+2] = rgb.red();
  _image[lidx+1] = rgb.green();
  _image[lidx] = rgb.blue();
}

void bitmap::fill( bmp_rgb rgb )
{
  if( _hdr.bitsperpixel() != 24 && _hdr.bitsperpixel() != 32 )
    fail_assert("can't set RGB for image with palette");

  int lx = _hdr.width();
  int ly = _hdr.height();
  for( int y = 0; y < ly; y++ ){
    int yy = _hdr.height() - y - 1;      // ????????? ? ???????????? ??? bmp ??????? ?????????
    int lidx = yy * _hdr.bytesperline(); // ????????? ???????? ??????

    for( int x = 0; x < lx; x++ ){
      int lidx_x = lidx + x * (_hdr.bitsperpixel() / 8); // ????????? ???????? RGB
      _image[lidx_x+2] = rgb.red();
      _image[lidx_x+1] = rgb.green();
      _image[lidx_x] = rgb.blue();
    }
  }
}

void bitmap::put( int y, int x, int cidx )
{
  int lx = _hdr.width();
  int ly = _hdr.height();
  test_index2(x, lx, y, ly);
  if( _hdr.bitsperpixel() == 24 || _hdr.bitsperpixel() == 32 )
    fail_assert("can't use palette with truecolor bitmap");
  if( cidx < 0 || cidx >= _hdr.palsize() )
    runtime("color out of palette (%d)", cidx);

  // ????????? ? ???????????? ??? bmp ??????? ?????????
  y = _hdr.height() - y - 1;

  // ????????? ???????? ??????
  int lidx = y * _hdr.bytesperline();

  // ?????????? ?????? ????? ??? ?????
  setcolidx(lidx, x, cidx);
}

void bitmap::fill( int cidx )
{
  if( _hdr.bitsperpixel() == 24 || _hdr.bitsperpixel() == 32 )
    fail_assert("can't use palette with truecolor bitmap");
  if( cidx < 0 || cidx >= _hdr.palsize() )
    runtime("color out of palette (%d)", cidx);

  int lx = _hdr.width();
  int ly = _hdr.height();
  for( int y = 0; y < ly; y++ ){
    int yy = _hdr.height() - y - 1;      // ????????? ? ???????????? ??? bmp ??????? ?????????
    int lidx = yy * _hdr.bytesperline(); // ????????? ???????? ??????

    for( int x = 0; x < lx; x++ )
      setcolidx(lidx, x, cidx);
  }
}

void bitmap::rgb2bayer( t_membuf<uchar>& res, bool is_bayerGR ) const
{
  int height = _hdr.height();
  int width = _hdr.width();
  // Demosaic using a basic nearest-neighbor algorithm, operating on groups of four pixels.
  int res_idx = 0;
  for( int y = 0; y < height; y++ ){
    for( int x = 0; x < width; x++ ){
      if( is_bayerGR ){
        // ??????????? ??????? BGR
        if( y % 2 == 0 && x % 2 == 0)
          res[res_idx++] = _image[3*x + y*width*3 + 1];
        if( y % 2 == 0 && x % 2 == 1)
          res[res_idx++] = _image[3*x + y*width*3 + 2];
        if( y % 2 == 1 && x % 2 == 0)
          res[res_idx++] = _image[3*x + y*width*3];
        if( y % 2 == 1 && x % 2 == 1)
          res[res_idx++] = _image[3*x + y*width*3 + 1];
      }
      else{
        if( y % 2 == 0 && x % 2 == 0)
          res[res_idx++] = _image[3*x + y*width*3];
        if( y % 2 == 0 && x % 2 == 1)
          res[res_idx++] = _image[3*x + y*width*3 + 1];
        if( y % 2 == 1 && x % 2 == 0)
          res[res_idx++] = _image[3*x + y*width*3 + 1];
        if( y % 2 == 1 && x % 2 == 1)
          res[res_idx++] = _image[3*x + y*width*3 + 2];
      }
    }
  }
}

void bitmap::save_as_bayer( const char* name, bool is_bayerGR ) const
{
  referer<stream> file = stream::create(name, fmWRITE, fmBINARY);
  int byte_num = _hdr.width() * _hdr.height();
  t_membuf<uchar> bayer(byte_num);
  rgb2bayer(bayer, is_bayerGR);  
  for( int j = 0; j < byte_num; j++ )
    write_byte(file, bayer[j]);
}

void bitmap::read_bayer( const char* name, bool is_bayerGR )
{
  int height = _hdr.height();
  int width = _hdr.width();
  int byte_num = width * height;
  t_membuf<uchar> bayer(byte_num);

  referer<stream> file = stream::create(name, fmREAD, fmBINARY);
  file->seek(0);
  for( int j = 0; j < byte_num; j++ )
    bayer[j] = read_byte(file);

  for( int y = 0; y < height; y++ ){
    for( int x = 0; x < width; x++ ){
      int xPos = x - (x - 1) % 2;
      int yPos = (y - (y - 1) % 2) * width;

      int r, g, b;
      if( is_bayerGR ){
        r = bayer[xPos + yPos  - width];
        g = (bayer[xPos + yPos] + bayer[xPos - 1 + yPos - width]) / 2;
        b = bayer[xPos - 1 + yPos];
      }
      else{
        r = bayer[xPos + yPos];
        g = (bayer[xPos + yPos - width] + bayer[xPos - 1 + yPos]) / 2;
        b = bayer[xPos - 1 + yPos - width];
      }
      //!! ?????? ???, ?????? ??? ??? ? ??????????? bmp (?????? ? ????? ?????? ????)
      put(height - y - 1 , x, bmp_rgb(r, g, b)); 
    }
  }
}

void bitmap::demosaicing_bayer_IN( const char* name, bool is_bayerGR )
{  
  int height = _hdr.height();
  int width = _hdr.width();
  int byte_num = width * height;
  t_membuf<uchar> bayer(byte_num);

  referer<stream> file = stream::create(name, fmREAD, fmBINARY);
  file->seek(0);
  for( int j = 0; j < byte_num; j++ )
    bayer[j] = read_byte(file);
  
  int ln_bt = _hdr.bytesperline();
  int px_bt = _hdr.bitsperpixel();
  int sh = 0;// ????? ?? ?????????? y 
  // ??? bayerBG ????????? ?????, ????? ?? ???????????? ? bayerGR
  if( !is_bayerGR )
    sh = 1;
  int r, g, b;
  for( int y = 0; y < height; y++ ){
    for( int x = 0; x < width; x++ ){
      // ????????? ?????? ? ????? ?????? ????????
      int idx = y*ln_bt + x * (px_bt / 8);
      // ???? ???????, ?? ?? ?????????
      if( x == 0 || x == width-1 || y == 0 || y == height-1 ){
        _image[idx+2] = 0;
        _image[idx+1] = 0;
        _image[idx] = 0;
        continue;
      }
      if( (y + sh + x) % 2 == 0 ){
        r = (bayer[x-1 + y*width] + bayer[x+1 + y*width]) / 2;
        b = (bayer[x + (y-1)*width] + bayer[x + (y+1)*width]) / 2;
        g = (bayer[x + y*width] + bayer[x+1 + (y+1)*width] + bayer[x+1 + (y-1)*width] +
             bayer[x-1 + (y+1)*width] + bayer[x-1 + (y-1)*width]) / 5;
      }else if( (y + sh + x) % 2 == 1 ){
        r = bayer[x + y*width];
        g = (bayer[x + (y-1)*width] + bayer[x + (y+1)*width] + bayer[x-1 + y*width] + bayer[x+1 + y*width]) / 4;
        b = (bayer[x+1 + (y+1)*width] + bayer[x+1 + (y-1)*width] +
             bayer[x-1 + (y+1)*width] + bayer[x-1 + (y-1)*width]) / 4;
      }
      // ???? ?????? ????????, ?? ???????? ??????? B ? R ???? ???????? ???????
      if( (y+sh) % 2 == 1 ){
        uchar tmp = b;
        b = r;
        r = tmp;
      }
      _image[idx+2] = r;
      _image[idx+1] = g;
      _image[idx] = b;
    }
  }
}

void bitmap::demosaicing_bayer( const char* name, bool is_bayerGR )
{  
  int height = _hdr.height();
  int width = _hdr.width();
  int byte_num = width * height;
  t_membuf<uchar> bayer(byte_num);

  referer<stream> file = stream::create(name, fmREAD, fmBINARY);
  file->seek(0);
  for( int j = 0; j < byte_num; j++ )
    bayer[j] = read_byte(file);
  
  int ln_bt = _hdr.bytesperline();
  int px_bt = _hdr.bitsperpixel();
  int sh = 0;// ????? ?? ?????????? y 
  // ??? bayerBG ????????? ?????, ????? ?? ???????????? ? bayerGR
  if( !is_bayerGR )
    sh = 1;
  int r, g, b;
  for( int y = 0; y < height; y++ ){
    for( int x = 0; x < width; x++ ){
      if( (y + sh + x) % 2 == 0 ){
        if( x == 0 ){
          r =  bayer[x+1 + y*width];
          if( y == 0 ){
            g = (bayer[x + y*width] + bayer[x+1 + (y+1)*width]) / 2;
            b = bayer[x + (y+1)*width];
          }else if( y == height - 1 ){
            g = (bayer[x + y*width] + bayer[x+1 + (y-1)*width]) / 2;
            b = bayer[x + (y-1)*width];
          }else{
            g = (bayer[x + y*width] + bayer[x+1 + (y+1)*width] + bayer[x+1 + (y-1)*width]) / 3;
            b = (bayer[x + (y-1)*width] + bayer[x + (y+1)*width]) / 2;
          }
        }else if( x == width-1 ){
          r = bayer[x-1 + y*width];
          if( y == 0 ){
            g = (bayer[x + y*width] + bayer[x-1 + (y+1)*width]) / 2;
            b = bayer[x + (y+1)*width];
          }else if( y == height - 1){
            g = (bayer[x + y*width] + bayer[x-1 + (y-1)*width]) / 2;
            b = bayer[x + (y-1)*width];
          }else{
            g = (bayer[x + y*width] + bayer[x-1 + (y+1)*width] + bayer[x-1 + (y-1)*width]) / 3;
            b = (bayer[x + (y-1)*width] + bayer[x + (y+1)*width]) / 2;
          }
        }
        else{
          r = (bayer[x-1 + y*width] + bayer[x+1 + y*width]) / 2;
          if( y == 0 ){
            b = bayer[x + (y+1)*width];
            g = (bayer[x + y*width] + bayer[x+1 + (y+1)*width] + bayer[x-1 + (y+1)*width]) / 3;
          }else if( y == height - 1){
            b = bayer[x + (y-1)*width];
            g = (bayer[x + y*width] + bayer[x+1 + (y-1)*width] + bayer[x-1 + (y-1)*width]) / 3;
          }else{
           b = (bayer[x + (y-1)*width] + bayer[x + (y+1)*width]) / 2;
           g = (bayer[x + y*width] + bayer[x+1 + (y+1)*width] + bayer[x+1 + (y-1)*width] +
             bayer[x-1 + (y+1)*width] + bayer[x-1 + (y-1)*width]) / 5;
          }
        }
      }
      else{
        r = bayer[x + y*width];
        if( x == 0 ){
          if( y == 0 ){
            g = (bayer[x + (y+1)*width] + bayer[x+1 + y*width]) / 2;
            b = bayer[x+1 + (y+1)*width];
          }else if( y == height - 1 ){
            g = (bayer[x + (y-1)*width] + bayer[x+1 + y*width]) / 2;
            b = bayer[x+1 + (y-1)*width];
          }else{
            g = (bayer[x + (y-1)*width] + bayer[x + (y+1)*width] + bayer[x+1 + y*width]) / 3;
            b = (bayer[x+1 + (y+1)*width] + bayer[x+1 + (y-1)*width] ) / 2;
          }
        }else if( x == width- 1 ){
          if( y == 0 ){
            g = (bayer[x + (y+1)*width] + bayer[x-1 + y*width]) / 2;
            b = bayer[x-1 + (y+1)*width];
          }else if( y == height - 1 ){
            g = (bayer[x + (y-1)*width] + bayer[x-1 + y*width]) / 2;
            b = bayer[x-1 + (y-1)*width];
          }else{
            g = (bayer[x + (y-1)*width] + bayer[x + (y+1)*width] + bayer[x-1 + y*width]) / 3;
            b = ( bayer[x-1 + (y+1)*width] + bayer[x-1 + (y-1)*width]) / 2;
          }
        }else{
          if( y == 0 ){
            g = (bayer[x + (y+1)*width] + bayer[x-1 + y*width] + bayer[x+1 + y*width]) / 3;
            b = (bayer[x+1 + (y+1)*width] + bayer[x-1 + (y+1)*width]) / 2;
          }else if( y == height - 1 ){
            g = (bayer[x + (y-1)*width] + bayer[x-1 + y*width] + bayer[x+1 + y*width]) / 3;
            b = (bayer[x+1 + (y-1)*width] + bayer[x-1 + (y-1)*width]) / 2;
          }else{       
            g = (bayer[x + (y-1)*width] + bayer[x + (y+1)*width] + bayer[x-1 + y*width] + bayer[x+1 + y*width]) / 4;
            b = (bayer[x+1 + (y+1)*width] + bayer[x+1 + (y-1)*width] +
             bayer[x-1 + (y+1)*width] + bayer[x-1 + (y-1)*width]) / 4;
          }
        }
      }
      // ???? ?????? ????????, ?? ???????? ??????? B ? R ???? ???????? ???????
      if( (y+sh) % 2 == 1 ){
        uchar tmp = b;
        b = r;
        r = tmp;
      }
      
      int idx = y*ln_bt + x * (px_bt / 8); // ????????? ???????? RGB; 
      _image[idx+2] = r;
      _image[idx+1] = g;
      _image[idx] = b;
    }
  }
}

}; // namespace lwml
