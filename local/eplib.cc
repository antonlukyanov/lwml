#include "eplib.h"
#include "stdmem.h"
#include "cstrng.h"
#include "basex.h"
#include "filename.h"
#include "stream.h"

/*#lake:stop*/

namespace lwml {

// epinfo

void epinfo::init( const char *fnm, const char *nm, const char *col, int ln, int st, int len )
{
  _fname = fnm;
  _name = nm;
  _color = col;
  _line = ln;
  _start = st;
  _length = len;
}

bool epinfo::is_name( const char* nm ) const
{
  return nm == 0 || _name.is_eq(nm) == 0;
}

bool epinfo::is_color( const char* col ) const
{
  return col == 0 || _color.is_eq(col) == 0;
}

bool epinfo::is_line( int ln ) const
{
  return ln == -1 || ln == _line;
}

bool epinfo::is( const char* nm, const char* col, int ln ) const
{
  return is_name(nm) && is_color(col) && is_line(ln);
}

// eplib

void eplib::setminmaxlen()
{
  int minlen, maxlen;
  minlen = maxlen = _info[0].len();
  for( int j = 1; j < _size; j++ ){
    int len = _info[j].len();
    if( len < minlen ) minlen = len;
    if( len > maxlen ) maxlen = len;
  }
  _minlen = minlen;
  _maxlen = maxlen;
}

eplib::eplib( const char *libname )
{
  referer<stream> file = stream::create(libname, fmREAD, fmTEXT);
  _size = file->count_lines();
  _info.resize(_size);
  for( int j = 0; j < _size; j++ ){
    strng buf;

    if( !file->get_str(buf) )
      throw ex_eplib("%s", libname);
    strng fname = buf.get_word();
    strng name = buf.get_word();
    strng color = buf.get_word();
    int line = buf.get_word().as_int();
    int start = buf.get_word().as_int();
    int length = buf.get_word().as_int();
    _info[j].init(fname.ascstr(), name.ascstr(), color.ascstr(), line, start, length);
  }
  setminmaxlen();
  _idata.resize(_maxlen);
}

void eplib::seek( int num )
{
  test_index(num, _size);
  referer<stream> file = stream::create(_info[num].fname().ascstr(), fmREAD, fmBINARY);
  file->seek(_info[num].start() * sizeof(short));
  file->read(_idata.access_raw_data(), _info[num].len() * sizeof(short));
}

void eplib::vget( vector& dst, int num )
{
  seek(num);
  int clen = _info[num].len();
  if( dst.len() < clen )
    fail_size(dst.len());
  dst.set_zero();
  for( int j = 0; j < clen; j++ )
    dst[j] = _idata[j];
}

int eplib::count( const char *name, const char* color, int line )
{
  int num = 0;
  for( int j = 0; j < _size; j++ )
    if( _info[j].is(name, color, line) )
      num++;
  return num;
}

void eplib::mget( matrix& dst, const char *name, const char *color, int line )
{
  int num = count(name, color, line);
  test_size(dst.str(), num);
  if( dst.col() < _maxlen )
    fail_size(dst.col());

  dst.set_zero();
  int d = 0;
  for( int j = 0; j < _size; j++ ){
    if( _info[j].is(name, color, line) ){
      seek(j);
      for( int k = 0; k < _info[j].len(); k++ )
        dst(d, k) = _idata[k];
      d++;
    }
  }
}

// epwindow

epwindow::epwindow( const matrix& src, int len )
{
  _src = &src;
  _len = len;
  if( src.col() < len )
    fail_size(src.col());
  _pos = 0;
}

void epwindow::seek( int pos )
{
  if( pos + _len > _src->col() )
    fail_index(pos);
  _pos = pos;
}

}; // namespace lwml
