#include "ivector.h"

#include "stdmem.h"
#include "mdefs.h"
#include "debug.h"

/*#lake:stop*/

namespace lwml {

void int_vector::alloc( int len )
{
  _data = raw_alloc<int>(_len = len);
}

void int_vector::dealloc()
{
  mem::free(_data);
}

void int_vector::realloc( int len )
{
  _data = raw_realloc<int>(_data, _len = len);
}

int_vector::int_vector( int len )
{
  alloc(len);
}

int_vector::int_vector( int len, int val )
{
  alloc(len);
  for( int j = 0; j < len; j++ )
    _data[j] = val;
}

int_vector::int_vector( int len, const int* val_arr )
{
  alloc(len);
  for( int j = 0; j < len; j++ )
    _data[j] = val_arr[j];
}

void int_vector::read_raw_data( const char* fname )
{
  referer<stream> file = stream::create(fname, fmREAD, fmTEXT);

  int buf;
  int sz = 0;
  while( fscanf(file->file(), "%d", &buf) == 1 )
    sz++;
  if( sz == 0 )
    throw ex_ivector("%s", fname);

  alloc(sz);
  file->seek(0);
  for( int j = 0; j < _len; j++ ){
    if( fscanf(file->file(), "%d", &buf) != 1 ){
      dealloc();
      throw ex_ivector("%s", fname);
    }
    _data[j] = buf;
  }
}

bool int_vector::read_data( referer<stream> file, const char* fname )
{
  cstrng buf;
  if( file->get_str(buf) && buf.subs(0, 7).is_eq("#LVECT:") ){
    int sz;
    if( !buf.subs(7, buf.len()-7).as_int_ex(&sz) )
      throw ex_ivector("%s", fname);
    if( sz == 0 )
      throw ex_ivector("%s", fname);

    alloc(sz);
    for( int j = 0; j < _len; j++ ){
      int buf;
      if( fscanf(file->file(), "%d", &buf) != 1 ){
        dealloc();
        throw ex_ivector("%s", fname);
      }
      _data[j] = buf;
    }
    return true;
  } else {
    return false;
  }
}

int_vector::int_vector( referer<stream> file )
{
  if( !read_data(file, "<<stream>>") )
    throw ex_ivector("<<stream>>");
}

int_vector::int_vector( const char *fname )
{
  referer<stream> file = stream::create(fname, fmREAD, fmTEXT);
  if( !read_data(file, fname) ){
    zzz_ex("lwml:message", "can't read header in <%s>, reading raw data", fname);
    read_raw_data(fname);
  }
}

int_vector::int_vector( const int_vector& src )
{
  alloc(src.len());
  operator=(src);
}

int_vector::~int_vector()
{
  dealloc();
}

int_vector& int_vector::operator=( const int_vector& src )
{
  if( &src == this ) return *this;
  test_size(src._len, _len);
  memcpy(_data, src._data, _len * sizeof(int));
  return *this;
}

void int_vector::save( referer<stream> file ) const
{
  file->printf("#LVECT:%d\n", _len);
  for( int j = 0; j < _len; j++ )
    file->printf("%d\n", _data[j]);
}

void int_vector::save( const char *name ) const
{
  save(stream::create(name, fmWRITE, fmTEXT));
}

void int_vector::export_as_text( const char *name ) const
{
  referer<stream> file = stream::create(name, fmWRITE, fmTEXT);
  for( int j = 0; j < _len; j++ )
    file->printf("%d\n", _data[j]);
}

void int_vector::copy( const int_vector& src, int val, int idx0 )
{
  int sz = src.len();
  if( sz > _len - idx0 )
    fail_size(sz);

  for( int j = 0; j < idx0; j++ )
    _data[j] = val;
  for( int j = 0; j < sz; j++ )
    _data[idx0 + j] = src[j];
  for( int j = idx0 + sz; j < _len; j++ )
    _data[j] = val;
}

void int_vector::copy_sub( const int_vector& src, int idx )
{
  // idx + _len - 1 > src.len() - 1
  if( idx < 0 || idx + _len > src.len() )
    fail_index(idx);
  for( int j = 0; j < _len; j++ )
    _data[j] = src[idx + j];
}

void int_vector::set_val( int val )
{
  for( int j = 0; j < _len; j++ )
    _data[j] = val;
}

void int_vector::set_zero()
{
  for( int j = 0; j < _len; j++ )
    _data[j] = 0;
}

void int_vector::swap( int k, int j )
{
  test_index2(k, len(), j, len());
  t_swap(_data[k], _data[j]);
}

void int_vector::revers()
{
  int n = len();
  for( int j = 0; j < n/2; j++ )
    swap(j, n-1-j);
}

int int_vector::min() const
{
  int res = _data[0];
  for( int k = 1; k < _len; k++ )
    if( _data[k] < res ) res = _data[k];
  return res;
}

int int_vector::max() const
{
  int res = _data[0];
  for( int k = 1; k < _len; k++ )
    if( _data[k] > res ) res = _data[k];
  return res;
}

int int_vector::min_idx() const
{
  int min = _data[0];
  int idx = 0;
  for( int k = 1; k < _len; k++ )
    if( _data[k] < min ){
      min = _data[k];
      idx = k;
    }
  return idx;
}

int int_vector::max_idx() const
{
  int max = _data[0];
  int idx = 0;
  for( int k = 1; k < _len; k++ )
    if( _data[k] > max ){
      max = _data[k];
      idx = k;
    }
  return idx;
}

int int_vector::min_abs() const
{
  int res = labs(_data[0]);
  for( int k = 1; k < _len; k++ ){
    int cur = labs(_data[k]);
    if( cur < res ) res = cur;
  }
  return res;
}

int int_vector::max_abs() const
{
  int res = labs(_data[0]);
  for( int k = 1; k < _len; k++ ){
    int cur = labs(_data[k]);
    if( cur > res ) res = cur;
  }
  return res;
}

int int_vector::min_abs_idx() const
{
  int min = labs(_data[0]);
  int idx = 0;
  for( int k = 1; k < _len; k++ ){
    int cur = labs(_data[k]);
    if( cur < min ){
      min = cur;
      idx = k;
    }
  }
  return idx;
}

int int_vector::max_abs_idx() const
{
  int max = labs(_data[0]);
  int idx = 0;
  for( int k = 1; k < _len; k++ ){
    int cur = labs(_data[k]);
    if( cur > max ){
      max = cur;
      idx = k;
    }
  }
  return idx;
}

int int_vector::inner_sum() const
{
  int sum = 0;
  for( int j = 0; j < _len; j++ )
    sum += _data[j];
  return sum;
}

}; // namespace lwml
