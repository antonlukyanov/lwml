#include "lwml/m_types/matrix.h"

#include "lwml/memory/stdmem.h"
#include "lwml/io/filename.h"

/*#lake:stop*/

namespace lwml {

void real_matrix::alloc( int m, int n )
{
  _str = m;
  _data = raw_alloc<real*>(m);
  _col = n;
  _repr = raw_alloc<real>(m * n);
  nan_filling(_repr, m * n);
  real *dat = _repr;
  for( int j = 0; j < _str; j++ ){
    _data[j] = dat;
    dat += _col;
  }
}

void real_matrix::dealloc()
{
  mem::free(_data);
  mem::free(_repr);
}

real_matrix::real_matrix()
{
  _str = 0;
  _data = NULL;
  _col = 0;
  _repr = NULL;
}

real_matrix::real_matrix( int m, int n )
{
  alloc(m, n);
}

real_matrix::real_matrix( int m, int n, real val )
{
  alloc(m, n);
  set_val(val);
}

void real_matrix::read_data( referer<stream> file, const char* fname )
{
  cstrng buf;
  if( !file->get_str(buf) )
    throw ex_matrix("%s", fname);
  if( !buf.subs(0, 7).is_eq("#RMATR:") && !buf.subs(0, 7).is_eq("#LMATR:") )
    throw ex_matrix("%s", fname);
  cstrng sz = buf.subs(7, buf.len()-7);
  int nstr, ncol;
  if( !sz.get_word(",").as_int_ex(&nstr) )
    throw ex_matrix("%s", fname);
  if( !sz.get_word(",").as_int_ex(&ncol) )
    throw ex_matrix("%s", fname);
  if( nstr == 0 || ncol == 0 )
    throw ex_matrix("%s", fname);

  alloc(nstr, ncol);
  for( int i = 0; i < nstr; i++ ){
    for( int j = 0; j < ncol; j++ ){
      real buf;
      if( fscanf(file->file(), "%lf", &buf) != 1 ){
        dealloc();
        throw ex_matrix("%s", fname);
      }
      _data[i][j] = buf;
    }
  }
}

real_matrix::real_matrix( referer<stream> file )
{
  read_data(file, "<<stream>>");
}

real_matrix::real_matrix( const char *fname )
{
  referer<stream> file = stream::create(fname, fmREAD, fmTEXT);
  read_data(file, fname);
}

real_matrix::real_matrix( const real_matrix& src )
{
  alloc(src.str(), src.col());
  operator=(src);
}

real_matrix::~real_matrix()
{
  dealloc();
}

void real_matrix::resize( int m, int n )
{
  dealloc();
  alloc(m, n);
}

void real_matrix::resize( int m, int n, real val )
{
  resize(m, n);
  set_val(val);
}

real_matrix& real_matrix::operator=( const real_matrix& src )
{
  if( &src == this ) return *this;
  test_size2(src.str(), str(), src.col(), col());
  memcpy(_repr, src._repr, _str * _col * sizeof(real));
  for( int j = 0; j < _str; j++ )
    _data[j] = _repr + (src._data[j] - src._repr);
  return *this;
}

void real_matrix::save( referer<stream> file, int prec, real_format nf ) const
{
  file->printf("#RMATR:%d,%d\n", _str, _col);
  for( int i = 0; i < _str; i++ ){
    for( int j = 0; j < _col; j++ )
      file->printf((nf == nfSCNT) ? "%.*e " : "%.*f ", prec, _data[i][j]);
    file->printf("\n");
  }
}

void real_matrix::save( const char *name, int prec, real_format nf ) const
{
  save(stream::create(name, fmWRITE, fmTEXT), prec, nf);
}

void real_matrix::copy( const real_matrix& src, real val, int i0, int j0 )
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

void real_matrix::copy_sub( const real_matrix& src, int i0, int j0 )
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

void real_matrix::set_val( real val )
{
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      _data[i][j] = val;
}

void real_matrix::set_zero()
{
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      _data[i][j] = 0.0;
}

void real_matrix::set_diag( real x )
{
  test_size(str(), col());
  set_zero();
  for( int i = 0; i < _str; i++ )
    _data[i][i] = x;
}

void real_matrix::swap( int k, int j )
{
  test_index2(k, str(), j, str());
  t_swap(_data[k], _data[j]);
}

void real_matrix::tran()
{
  test_size(str(), col());
  for( int i = 0; i < _str; i++ ){
    for( int j = i + 1; j < _col; j++ )
      t_swap(_data[i][j], _data[j][i]);
  }
}

void real_matrix::center()
{
  real m = mid();
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      _data[i][j] -= m;
}

real real_matrix::mag() const
{
  double sum = 0.0;
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      sum += fsqr(_data[i][j]);
  return sum;
}

real real_matrix::norm() const
{
  real max = max_abs();
  if( max == 0.0 )
    return 0.0;
  double s = 0.0;
  for( int i = 0; i < _str; i++ ){
    for( int j = 0; j < _col; j++ )
      s += fsqr(_data[i][j] / max);
  }
  return max * sqrt(s);
}

real real_matrix::norm_manh() const
{
  real max = 0.0;
  for( int j = 0; j < _col; j++ ){
    real sum = 0.0;
    for( int i = 0; i < _str; i++ )
      sum += fabs(_data[i][j]);
    if( sum > max )
      max = sum;
  }
  return max;
}

real real_matrix::trace() const
{
  test_size(str(), col());
  double s = 0.0;
  for( int i = 0; i < _str; i++ )
    s += _data[i][i];
  return s;
}

real real_matrix::min() const
{
  real res = _data[0][0];
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      if( _data[i][j] < res )
        res = _data[i][j];
  return res;
}

real real_matrix::max() const
{
  real res = _data[0][0];
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      if( _data[i][j] > res )
        res = _data[i][j];
  return res;
}

real real_matrix::min_abs() const
{
  real res = fabs(_data[0][0]);
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ ){
      real cur = fabs(_data[i][j]);
      if( cur < res ) res = cur;
    }
  return res;
}

real real_matrix::max_abs() const
{
  real res = fabs(_data[0][0]);
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ ){
      real cur = fabs(_data[i][j]);
      if( cur > res ) res = cur;
    }
  return res;
}

real real_matrix::inner_sum() const
{
  double sum = 0.0;
  for( int i = 0; i < _str; i++ ){
    for( int j = 0; j < _col; j++ )
      sum += _data[i][j];
  }
  return sum;
}

real real_matrix::mid() const
{
  double sum = 0.0;
  for( int i = 0; i < _str; i++ ){
    for( int j = 0; j < _col; j++ )
      sum += _data[i][j];
  }
  return sum / str() / col();
}

real real_matrix::dist( const real_matrix& m1, const real_matrix& m2 )
{
  test_size2(m1.str(), m2.str(), m1.col(), m2.col());
  int ns = m1.str();
  int nc = m1.col();

  real max = 0.0;
  for( int i = 0; i < ns; i++ ){
    for( int j = 0; j < nc; j++ ){
      real cur = fabs(m1(i, j) - m2(i, j));
      if( cur > max ) max = cur;
    }
  }
  if( max == 0.0 )
    return 0.0;
  double s = 0.0;
  for( int i = 0; i < ns; i++ ){
    for( int j = 0; j < nc; j++ )
      s += fsqr((m1(i, j) - m2(i, j)) / max);
  }
  return max * sqrt(s);
}

real_matrix& real_matrix::operator+=( const real_matrix& src )
{
  test_size2(src.str(), str(), src.col(), col());
  for( int i = 0; i < _str; i++ ){
    for( int j = 0; j < _col; j++ )
      _data[i][j] += src(i, j);
  }
  return *this;
}

real_matrix& real_matrix::operator-=( const real_matrix& src )
{
  test_size2(src.str(), str(), src.col(), col());
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      _data[i][j] -= src(i, j);
  return *this;
}

real_matrix& real_matrix::operator*=( real mu )
{
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      _data[i][j] *= mu;
  return *this;
}

real_matrix& real_matrix::operator/=( real mu )
{
  for( int i = 0; i < _str; i++ )
    for( int j = 0; j < _col; j++ )
      _data[i][j] /= mu;
  return *this;
}

void real_matrix::get_col( vector& v, int c ) const
{
  test_index(c, col());
  test_size(v.len(), str());
  for( int j = 0; j < _str; j++ )
    v[j] = _data[j][c];
}

void real_matrix::get_str( vector& v, int s ) const
{
  test_index(s, str());
  test_size(v.len(), col());
  for( int j = 0; j < _col; j++ )
    v[j] = _data[s][j];
}

void real_matrix::put_col( const vector& v, int c )
{
  test_index(c, col());
  test_size(v.len(), str());
  for( int j = 0; j < _str; j++ )
    _data[j][c] = v[j];
}

void real_matrix::put_str( const vector& v, int s )
{
  test_index(s, str());
  test_size(v.len(), col());
  for( int j = 0; j < _col; j++ )
    _data[s][j] = v[j];
}

void real_matrix::inc_mul( const real_matrix& src )
{
  // (m|n)*(n|k)=(m|k)=(m|n) => k=n => (m|n)*(n|n)=(m|n)
  test_size2(src.str(), src.col(), src.str(), col());
  int sz = src.str();

  vector buf(_col);
  for( int i = 0; i < _str; i++ ){
    get_str(buf, i);
    for( int j = 0; j < _col; j++ ){
      double s = 0.0;
      for( int k = 0; k < sz; k++ )
        s += buf[k] * src(k, j);
      operator()(i, j) = s;
    }
  }
}

void real_matrix::inc_mul_tran( const real_matrix& src )
{
  // (m|n)*(k|n)T=(m|n)*(n|k)=(m|k)=(m|n) => k=n => (m|n)*(n|n)T = (m|n)
  test_size2(src.str(), src.col(), src.str(), col());
  int sz = src.col();

  vector buf(_col);
  for( int i = 0; i < _str; i++ ){
    get_str(buf, i);
    for( int j = 0; j < _col; j++ ){
      double s = 0.0;
      for( int k = 0; k < sz; k++ )
        s += buf[k] * src(j, k);
      operator()(i, j) = s;
    }
  }
}

void real_matrix::mul(
  real_matrix& dst, const real_matrix& src1, const real_matrix& src2
)
{
  // (m|n)*(n|k)=(m|k)
  test_size(src1.col(), src2.str());
  test_size2(dst.str(), src1.str(), dst.col(), src2.col());
  int ds = dst.str();
  int dc = dst.col();
  int sz = src1.col();

  for( int i = 0; i < ds; i++ ){
    for( int j = 0; j < dc; j++ ){
      double s = 0.0;
      for( int k = 0; k < sz; k++ )
        s += src1(i, k) * src2(k, j);
      dst(i, j) = s;
    }
  }
}

void real_matrix::mul_tran( real_matrix& dst, const real_matrix& src1, const real_matrix& src2 )
{
  // (m|n)*(k|n)T=(m|n)*(n|k)=(m|k)
  test_size(src1.col(), src2.col());
  test_size2(dst.str(), src1.str(), dst.col(), src2.str());
  int ds = dst.str();
  int dc = dst.col();
  int sz = src1.col();

  for( int i = 0; i < ds; i++ ){
    for( int j = 0; j < dc; j++ ){
      double s = 0.0;
      for( int k = 0; k < sz; k++ )
        s += src1(i, k) * src2(j, k);
      dst(i, j) = s;
    }
  }
}

real real_matrix::inner_mul( int j1, int j2 )
{
  double s = 0.0;
  for( int i = 0; i < _col; i++ )
    s += _data[j1][i] * _data[j2][i];
  return s;
}

// gramm

void real_matrix::gramm( real_matrix& a )
{
  int ns = a.str();
  int nc = a.col();
  for( int k = 0; k < ns; k++ )            // current vect is m[k]
    for( int j = 0; j < k; j++ ){              // modified by vect m[i]
      real e = a.inner_mul(j, j);
      if( e == 0.0 )
        throw ex_sing();
      real w = a.inner_mul(k, j) / e;
      for( int i = 0; i < nc; i++ )
        a(k, i) -= w * a(j, i);
    }
}

/*
void minittakens( real_matrix *matr, vector& vect, int winlen, int shift )
{
  int wnum = winnum(vect.len(), winlen, shift);

  minitptr(matr, wnum, winlen);
  {
    int j;
    real *dat = vect->data;
    for( j = 0; j < wnum; j++ ){
      matr->data[j] = dat;
      dat += shift;
    }
  }
}
*/

}; // namespace lwml
