// ������ � ������� � ������� windows bitmap
// lwml, (c) ltwood

#ifndef _BMP_
#define _BMP_

#include "defs.h"
#include "mdefs.h"
#include "stream.h"
#include "t_membuf.h"
#include "rgb.h"

/*#lake:stop*/

namespace lwml {

// ���������� ��� ������ ������������� ����������� �������

DEF_EX_TYPE_MSG(ex_integrity, ex_bmp, "incorrect bitmap");

// ������������ ���� ����� �� �������� ������� BMP
// ���� ����������� � ���� ���� ��������� � �������� ����� RGB.
// ������ �������� ���������� ������������ ������������ ���������.

class bmp_rgb : public value {
public:
  // �������������� �� ���������
  // ��� ����� ������������� ���������� ������ �����
  bmp_rgb()
    : _red(0), _green(0), _blue(0) {}

  // ������������� ���� ��������� ����������
  bmp_rgb( uchar r, uchar g, uchar b )
    : _red(r), _green(g), _blue(b) {}

  // ������������� �� �������� ������������� �����
  bmp_rgb( const rgbcolor& rgb )
    : _red(fpr::lround(255.0 * rgb.red())),
      _green(fpr::lround(255.0 * rgb.green())),
      _blue(fpr::lround(255.0 * rgb.blue()))
  {}

  // ������������� ��������� ���� �������� ���������
  void set( uchar r, uchar g, uchar b ){
    _red = r; _green = g; _blue = b;
  }

  // ��������� �������� �������� ���������
  uchar red()   const { return _red; }
  uchar green() const { return _green; }
  uchar blue()  const { return _blue; }

  // ��������� �������� �������� ���������
  // ������� ���������� ������ �� ����������,
  // �������� �������� �������� ����������

  void set_red( uchar r )   { _red = r; }
  void set_green( uchar g ) { _green = g; }
  void set_blue( uchar b )  { _blue = b; }

  // �������� ������� ������������ ������������� �����
  rgbcolor get_rgb() const { return rgbcolor(_red/255.0, _green/255.0, _blue/255.0); }

  // �������� ������������� �������� �������
  uchar gray() const { return static_cast<uchar>(fpr::lround(255.0 * get_rgb().gray())); }

private:
  uchar _red, _green, _blue;
};

// ������������ ����� ������� � ����� BMP

const int MAXPALSIZE = 256;

// �������, �������� � ������ ����� ������� BMP
// ������� ����������� � ������ ������������ ��������
// (true color, 24 ���� �� ������).
// ���������� ������������� ������� ��������� � ���,
// ��� ��� �������� � ����� ������� BMP.
// ��������� �������� ����������� �������������� � ������ ���������.

class bmp_pal : public value {
public:
  // ��������� ����� ������� � �������� ��������� �������
  // size - ������ �������
  // ��� size == 0 ��������� ������ �������,
  // ��������������� ������������� �����������.
  bmp_pal( int size = 0 );

  // ��������� ������� �� ����� ������� BMP
  // fname - ��� �����
  // sz - ������ �������� �������
  bmp_pal( const char* fname, int sz );

  // ��������� ������� �������
  // ���������� ������ ������� � �������
  int len() const { return _len; }

  // �������� ������� ������� � �������� ��������
  bmp_rgb get( int idx ) const;

  // ��������� �������� �������� ������� � �������� ��������
  void put( int idx, bmp_rgb rgb );

  // ������ ������� � ���� BMP
  // file - �������� ����, � ������� ������������ ������
  void save( referer<stream> file ) { write(file); }

private:
  int _len;
  uint32 _pal[MAXPALSIZE];

  // ������ ������� �������� ����� �� ����� �� ��������� ��������
  // ��������� ������ ������������ �������
  // ������������ ������ �������� ������ bitmap
  void read( referer<stream> file, int size );
  void write( referer<stream> file );
};

// ����� ��������� ������ � ������ ��������� bmp-�����

class bmp_header : public value {
public:
  // ��������� ��������� �� �����.
  // fname - ��� �����, �� �������� ������������ ������
  bmp_header( const char* fname );

  // �������� ��������� �� ����������
  // height - ������ �����������
  // width - ������ �����������
  // palsize - ������ �������
  bmp_header( int height, int width, int palsize );

  // ��������� ��������� � �������� ����
  void save( referer<stream> file ) { write(file); }

  // �������� ������ ������� � �������
  int palsize() const { return _palsize; }

  // �������� ������ ����������� � ��������
  int width()   const { return _width; }

  // �������� ������ ����������� � ��������
  int height()  const { return _height; }

  // �������� ������� ����� � ����� �� ������
  int bitsperpixel() const { return _bitsperpixel; }

  // �������� ������ ����������� � ������
  int bytesperline() const { return _bpl; }

private:
  int _palsize;
  int _width, _height;
  int _bitsperpixel;
  int _bpl;

  void write16( referer<stream>, int );
  void write32( referer<stream>, int );
  int read16( referer<stream>, int ofs );
  int read32( referer<stream>, int ofs );

  void write( referer<stream> file );
  void read( referer<stream> file );

  int width2bpl( int width );
  int palsize2bpp( int );
};

// ����������� � ������� BMP
// ���������� �������� � 1,4,8,24,32 ������ �� �����.
// ��� ������ ����������� �� ����� ��������� ��������� ������ �������� �������.
// ��� �������� ����������� ������ ���� ������ ������ �������
// � ��� ���� �� ������� ������� ������������ ������� ����� (��� �� �����)
// �� ���������� �������: 0->24, 2->1, 16->4, 256->8.
// Y-���������� ������������� ��� �������� ����.

class bitmap : public value {
public:
  // ������ ����������� �� �����
  bitmap( const char* fname );

  // �������� ����������� � ��������� �����������
  // height - ������ ������������ �����������
  // width - ������ ������������ �����������
  // pal - ������� ������������ �����������
  bitmap( int height, int width, const bmp_pal& pal );

  // ���������� ����������� � �����
  void save( const char* name );

  // ��������� ������ ����������� � ��������
  int width() const { return _hdr.width(); }

  // ��������� ������ ����������� � ��������
  int height() const { return _hdr.height(); }

  // ��������� ������� ����� � ����� �� ����� (1,4,8,24)
  int bpp() const { return _hdr.bitsperpixel(); }

  // �������� ������ �� ������� �����������
  bmp_pal& get_pal() { return _pal; }

  // �������� ���� ����� � ��������� ������������
  bmp_rgb get( int y, int x ) const;

  // ���������� ���� ����� � ��������, �������� ��� �������� ���� (RGB).
  // ��������� ������ ��� ����������� � �������� ����� 24 ���� �� �����.
  void put( int y, int x, bmp_rgb rgb );

  // ���������� ���� ���� ����� � ��������, �������� ��� �������� ���� (RGB).
  // ��������� ������ ��� ����������� � �������� ����� 24 ���� �� �����.
  void fill( bmp_rgb rgb );

  // ���������� ���� ����� � ��������, �������� �������� ����� � ������� �������.
  // ��������� ������ ��� ����������� � �������� ����� 1,4,8 ��� �� �����.
  // cidx - ������ ����� � ������� �������.
  void put( int y, int x, int cidx );

  // ���������� ���� ���� ����� � ��������, �������� �������� ����� � ������� �������.
  // ��������� ������ ��� ����������� � �������� ����� 1,4,8 ��� �� �����.
  // cidx - ������ ����� � ������� �������.
  void fill( int cidx );

private:
  bmp_header _hdr;
  bmp_pal _pal;
  t_membuf<uchar> _image;

  uchar read_byte( referer<stream> );
  void write_byte( referer<stream>, uchar );

  void read( referer<stream> );
  void write( referer<stream> );

  uchar getcolidx( int lidx, int x ) const;
  void  setcolidx( int lidx, int x, int cidx );
};

}; // namespace lwml

#endif // _BMP_
