#include "imatrix.h"

#include "stdmem.h"
#include "filename.h"

/*#lake:stop*/

namespace lwml {

void int_matrix::alloc( int m, int n )
{
  _str = m;
  _data = raw_alloc<int*>(m);
  _col = n;
  _repr = raw_alloc<int>(m * n);
  int *dat = _repr;
  for( int j = 0; j < _str; j++ ){
    _data[j] = dat;
    dat += _col;
  }
}

void int_matrix::dealloc()
{
  mem::free(_data);
  mem::free(_repr);
}

int_matrix::int_matrix()
{
  _str = 0;
  _data = NULL;
  _col = 0;
  _repr = NULL;
}

int_matrix::int_matrix( int m, int n )
{
  alloc(m, n);
}

int_matrix::int_matrix( int m, int n, int val )
{
  alloc(m, n);
  set_val(val);
}

void int_matrix::read_data( referer<stream> file, const char* fname )
{
  cstrng buf;
  if( !file->get_str(buf) )
    throw ex_imatrix("%s", fname);
  if( !buf.subs(0, 7).is_eq("#LMATR:") )
    throw ex_imatrix("%s", fname);
  cstrng sz = buf.subs(7, buf.len()-7);
  int nstr, ncol;
  if( !sz.get_word(",").as_int_ex(&nstr) )
    throw ex_imatrix("%s", fname);
  if( !sz.get_word(",").as_int_ex(&ncol) )
    throw ex_imatrix("%s", fname);
  if( nstr == 0 || ncol == 0 )
    throw ex_imatrix("%s", fname);

  alloc(nstr, ncol);
  for( int i = 0; i < nstr; i++ ){
    for( int j = 0; j < ncol; j++ ){
      int buf;
      if( fscanf(file->file(), "%d", &buf) != 1 ){
        dealloc();
        throw ex_imatrix("%s", fname);
      }
      _data[i][j] = buf;
    }
  }
}

int_matrix::int_matrix( referer<stream> file )
{
  read_data(file, "<<stream>>");
}

int_matrix::int_matrix( const char *fname )
{
  referer<stream> file = stream::create(fname, fmREAD, fmTEXT);
  read_data(file, fname);
}

int_matrix::int_matrix( const int_matrix& src )
{
  alloc(src.str(), src.col());
  operator=(src);
}

int_matrix::~int_matrix()
{
  dealloc();
}

void int_matrix::resize( int m, int n )
{
  dealloc();
  alloc(m, n);
}

void int_matrix::resize( int m, int n, int val )
{
  resize(m, n);
  set_val(val);
}

int_matrix& int_matrix::operator=( const int_matrix& src )
{
  if( &src == this ) return *this;
  test_size2(src.str(), str(), src.col(), col());
  memcpy(_repr, src._repr, _str * _col * sizeof(int));
  for( int j = 0; j < _str; j++ )
    _data[j] = _repr + (src._data[j] - src._repr);
  return *this;
}

void int_matrix::save( referer<stream> file ) const
{
  file->printf("#LMATR:%d,%d\n", _str, _col);
  for( int i = 0; i < _str; i++ ){
    for( int j = 0; j < _col; j++ )
      file->printf("%d ", _data[i][j]);
    file->printf("\n");
  }
}

void int_matrix::save( const char *name ) const
{
  save(stream::create(name, fmWRITE, fmTEXT));
}

void int_matrix::copy( const int_matrix& src, int val, int i0, int j0 )
{
  int ns = src.str();
  int nc = src.col();
  if( ns > _str - i0 )
    fail_size(ns);
  if( nc > _col - j0 )
    fail_size(nc);

  for( int i = 0; i < i0; i++ ){
    for( int j = 0; j < _col; j++ )
      _data[i][j] = val;
  }
  for( int i = 0; i < ns; i++ ){
    for( int j = 0; j < j0; j++ )
      _data[i0 + i][j] = val;
    for( int j = 0; j < nc; j++ )
      _data[i0 + i][j0 + j] = src(i, j);
    for( int j = j0 + nc; j < _col; j++ )
      _data[i0 + i][j] = val;
  }
  for( int i = i0 + ns; i < _str; i++ ){
    for( int j = 0; j < _col; j++ )
      _data[i][j] = val;
  }
}

void int_matrix::copy_sub( const int_matrix& src, int i0, int j0 )
{
  // i0 + _str - 1 > src.str() - 1 || j0 + _col - 1 > src.col() - 1
  if( i0 < 0 || i0 + _str > src.str() )
    fail_index(i0);
  if( j0 < 0 || j0 + _col > src.col() )
    fail_index(j0);
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      _data[i][j] = src(i0 + i, j0 + j);
}

void int_matrix::set_val( int val )
{
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      _data[i][j] = val;
}

void int_matrix::set_zero()
{
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      _data[i][j] = 0;
}

void int_matrix::set_diag( int x )
{
  test_size(str(), col());
  set_zero();
  for( int i = 0; i < _str; i++ )
    _data[i][i] = x;
}

void int_matrix::swap( int k, int j )
{
  test_index2(k, str(), j, str());
  t_swap(_data[k], _data[j]);
}

void int_matrix::tran()
{
  test_size(str(), col());
  for( int i = 0; i < _str; i++ ){
    for( int j = i + 1; j < _col; j++ )
      t_swap(_data[i][j], _data[j][i]);
  }
}

int int_matrix::min() const
{
  int res = _data[0][0];
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      if( _data[i][j] < res )
        res = _data[i][j];
  return res;
}

int int_matrix::max() const
{
  int res = _data[0][0];
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      if( _data[i][j] > res )
        res = _data[i][j];
  return res;
}

int int_matrix::min_abs() const
{
  int res = labs(_data[0][0]);
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ ){
      int cur = labs(_data[i][j]);
      if( cur < res ) res = cur;
    }
  return res;
}

int int_matrix::max_abs() const
{
  int res = labs(_data[0][0]);
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ ){
      int cur = labs(_data[i][j]);
      if( cur > res ) res = cur;
    }
  return res;
}

int int_matrix::inner_sum() const
{
  int sum = 0;
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      sum += _data[i][j];
  return sum;
}

void int_matrix::get_col( int_vector& v, int c ) const
{
  test_index(c, col());
  test_size(v.len(), str());
  for( int j = 0; j < _str; j++ )
    v[j] = _data[j][c];
}

void int_matrix::get_str( int_vector& v, int s ) const
{
  test_index(s, str());
  test_size(v.len(), col());
  for( int j = 0; j < _col; j++ )
    v[j] = _data[s][j];
}

void int_matrix::put_col( const int_vector& v, int c )
{
  test_index(c, col());
  test_size(v.len(), str());
  for( int j = 0; j < _str; j++ )
    _data[j][c] = v[j];
}

void int_matrix::put_str( const int_vector& v, int s )
{
  test_index(s, str());
  test_size(v.len(), col());
  for( int j = 0; j < _col; j++ )
    _data[s][j] = v[j];
}

}; // namespace lwml
