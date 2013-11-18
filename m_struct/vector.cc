#include "vector.h"

#include "stdmem.h"
#include "mdefs.h"
#include "filename.h"
#include "debug.h"

/*#lake:stop*/

namespace lwml {

void real_vector::alloc( int len )
{
  _data = raw_alloc<real>(_len = len);
  nan_filling(_data, len);
}

void real_vector::dealloc()
{
  mem::free(_data);
}

void real_vector::realloc( int len )
{
  _data = raw_realloc<real>(_data, _len = len);
}

real_vector::real_vector( int len )
{
  alloc(len);
}

real_vector::real_vector( int len, real val )
{
  alloc(len);
  set_val(val);
}

real_vector::real_vector( int len, const real* src )
{
  alloc(len);
  for( int j = 0; j < len; j++ )
    _data[j] = src[j];
}

void real_vector::read_raw_data( const char* fname )
{
  referer<stream> file = stream::create(fname, fmREAD, fmTEXT);

  real buf;
  int sz = 0;
  while( fscanf(file->file(), "%lf", &buf) == 1 )
    sz++;
  if( sz == 0 )
    throw ex_vector("%s", fname);

  alloc(sz);
  file->seek(0);
  for( int j = 0; j < _len; j++ ){
    if( fscanf(file->file(), "%lf", &buf) != 1 ){
      dealloc();
      throw ex_vector("%s", fname);
    }
    _data[j] = buf;
  }
}

bool real_vector::read_data( referer<stream> file, const char* fname )
{
  cstrng buf;
  if( file->get_str(buf) && (buf.subs(0, 7).is_eq("#RVECT:") || buf.subs(0, 7).is_eq("#LVECT:")) ){
    int sz;
    if( !buf.subs(7, buf.len()-7).as_int_ex(&sz) )
      throw ex_vector("%s", fname);
    if( sz == 0 )
      throw ex_vector("%s", fname);

    alloc(sz);
    for( int j = 0; j < _len; j++ ){
      real buf;
      if( fscanf(file->file(), "%lf", &buf) != 1 ){
        dealloc();
        throw ex_vector("%s", fname);
      }
      _data[j] = buf;
    }
    return true;
  } else {
    return false;
  }
}

real_vector::real_vector( referer<stream> file )
{
  if( !read_data(file, "<<stream>>") )
    throw ex_vector("<<stream>>");
}

real_vector::real_vector( const char *fname )
{
  referer<stream> file = stream::create(fname, fmREAD, fmTEXT);
  if( !read_data(file, fname) ){
    zzz_ex("lwml:message", "can't read header in <%s>, reading raw data", fname);
    read_raw_data(fname);
  }
}

real_vector::real_vector( const real_vector& src )
{
  alloc(src.len());
  operator=(src);
}

real_vector::~real_vector()
{
  dealloc();
}

real_vector& real_vector::operator=( const real_vector& src )
{
  if( &src == this ) return *this;
  test_size(src._len, _len);
  memcpy(_data, src._data, _len * sizeof(real));
  return *this;
}

void real_vector::save( referer<stream> file, int prec, real_format nf ) const
{
  file->printf("#RVECT:%d\n", _len);
  for( int j = 0; j < _len; j++ )
    file->printf((nf == nfSCNT) ? "%.*e\n" : "%.*f\n", prec, _data[j]);
}

void real_vector::save( const char *name, int prec, real_format nf ) const
{
  save(stream::create(name, fmWRITE, fmTEXT), prec, nf);
}

void real_vector::export_as_text( const char *name, int prec, real_format nf ) const
{
  referer<stream> file = stream::create(name, fmWRITE, fmTEXT);
  for( int j = 0; j < _len; j++ )
    file->printf((nf == nfSCNT) ? "%.*e\n" : "%.*f\n", prec, _data[j]);
}

void real_vector::copy( const real_vector& src, real val, int idx0 )
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

void real_vector::copy_sub( const real_vector& src, int idx )
{
  // idx + _len - 1 > src.len() - 1
  if( idx < 0 || idx + _len > src.len() )
    fail_index(idx);
  for( int j = 0; j < _len; j++ )
    _data[j] = src[idx + j];
}

void real_vector::set_val( real val )
{
  for( int j = 0; j < _len; j++ )
    _data[j] = val;
}

void real_vector::set_zero()
{
  for( int j = 0; j < _len; j++ )
    _data[j] = 0.0;
}

void real_vector::swap( int k, int j )
{
  test_index2(k, len(), j, len());
  t_swap(_data[k], _data[j]);
}

void real_vector::revers()
{
  int n = len();
  for( int j = 0; j < n/2; j++ )
    swap(j, n-1-j);
}

void real_vector::center()
{
  real m = mid();
  for( int k = 0; k < _len; k++ )
    _data[k] -= m;
}

real real_vector::mag() const
{
  double s = 0.0;
  for( int i = 0; i < _len; i++ )
    s += fsqr(_data[i]);
  return s;
}

real real_vector::norm() const
{
  real max = max_abs();
  if( max == 0.0 )
    return 0.0;
  double s = 0.0;
  for( int k = 0; k < _len; k++ )
    s += fsqr(_data[k] / max);
  return max * sqrt(s);
}

real real_vector::norm_manh() const
{
  double sum = 0.0;
  for( int k = 0; k < _len; k++ )
    sum += fabs(_data[k]);
  return sum;
}

real real_vector::min() const
{
  real res = _data[0];
  for( int k = 1; k < _len; k++ )
    if( _data[k] < res ) res = _data[k];
  return res;
}

real real_vector::max() const
{
  real res = _data[0];
  for( int k = 1; k < _len; k++ )
    if( _data[k] > res ) res = _data[k];
  return res;
}

int real_vector::min_idx() const
{
  real min = _data[0];
  int idx = 0;
  for( int k = 1; k < _len; k++ ){
    real cur = _data[k];
    if( cur < min ){
      min = cur;
      idx = k;
    }
  }
  return idx;
}

int real_vector::max_idx() const
{
  real max = _data[0];
  int idx = 0;
  for( int k = 1; k < _len; k++ ){
    real cur = _data[k];
    if( cur > max ){
      max = cur;
      idx = k;
    }
  }
  return idx;
}

real real_vector::min_abs() const
{
  real res = fabs(_data[0]);
  for( int k = 1; k < _len; k++ ){
    real cur = fabs(_data[k]);
    if( cur < res ) res = cur;
  }
  return res;
}

real real_vector::max_abs() const
{
  real res = fabs(_data[0]);
  for( int k = 1; k < _len; k++ ){
    real cur = fabs(_data[k]);
    if( cur > res ) res = cur;
  }
  return res;
}

int real_vector::min_abs_idx() const
{
  real min = fabs(_data[0]);
  int idx = 0;
  for( int k = 1; k < _len; k++ ){
    real cur = fabs(_data[k]);
    if( cur < min ){
      min = cur;
      idx = k;
    }
  }
  return idx;
}

int real_vector::max_abs_idx() const
{
  real max = fabs(_data[0]);
  int idx = 0;
  for( int k = 1; k < _len; k++ ){
    real cur = fabs(_data[k]);
    if( cur > max ){
      max = cur;
      idx = k;
    }
  }
  return idx;
}

real real_vector::inner_sum() const
{
  double sum = 0.0;
  for( int j = 0; j < _len; j++ )
    sum += _data[j];
  return sum;
}

real real_vector::mid() const
{
  if( len() == 0 )
    return 0.0;
  double sum = 0.0;
  for( int j = 0; j < _len; j++ )
    sum += _data[j];
  return sum / len();
}

real real_vector::disp() const
{
  if( len() == 0 )
    return 0.0;
  real m = mid();
  double disp = 0.0;
  for( int j = 0; j < _len; j++ )
    disp += fsqr(_data[j] - m);
  return disp / len();
}

real real_vector::acorr( int k ) const
{
  if( len() == 0 )
    fail_assert("empty vector");
  if( k < 0 ) k = -k;
  real m = mid();
  double res = 0.0;
  for( int j = 0; j < _len - k; j++ )
    res += (_data[j]-m) * (_data[j+k]-m) / len();
  return res;
}

real real_vector::interpol( real t ) const
{
  if( t < 0 || t > len() - 1 )
    fail_assert("argument out of bounds");
  int idx = int_cast(t);
  real delta = _data[idx+1] - _data[idx];
  return _data[idx] + delta * (t - idx);
}

real real_vector::dist( const real_vector& v1, const real_vector& v2 )
{
  test_size(v1.len(), v2.len());
  int sz = v1.len();

  real max = fabs(v1[0] - v2[0]);
  for( int k = 1; k < sz; k++ ){
    real cur = fabs(v1[k] - v2[k]);
    if( cur > max ) max = cur;
  }
  if( max == 0.0 )
    return 0.0;
  double s = 0.0;
  for( int k = 0; k < sz; k++ )
    s += fsqr((v1[k] - v2[k]) / max);
  return max * sqrt(s);
}

real real_vector::inner_mul( const real_vector& v1, const real_vector& v2 )
{
  test_size(v1.len(), v2.len());
  int sz = v1.len();

  double s = 0.0;
  for( int i = 0; i < sz; i++ )
    s += v1[i] * v2[i];
  return s;
}

real real_vector::corr( const real_vector& v1, const real_vector& v2 )
{
  test_size(v1.len(), v2.len());
  if( v1.len() == 0 )
    fail_assert("empty vector");
  int sz = v1.len();

  real mx = v1.mid();
  real my = v2.mid();

  double sxy = 0.0;
  for( int j = 0; j < sz; j++ )
    sxy += (v1[j] - mx) * (v2[j] - my) / v1.len();

  return sxy / sqrt(v1.disp() * v2.disp());
}

real real_vector::ccorr( const real_vector& v1, const real_vector& v2, int k )
{
  int sz1 = v1.len();
  int sz2 = v2.len();
  real mid1 = v1.mid();
  real mid2 = v2.mid();
  double res = 0.0;
  for( int j = 0; j < sz1; j++ )
    if( j + k >= 0 && j + k < sz2 )
      res += (v1[j]-mid1) * (v2[j+k]-mid2);
  return res;
}

real real_vector::project( const real_vector& dst, const real_vector& src )
{
  test_size(dst.len(), src.len());
  return real_vector::inner_mul(src, dst) / dst.norm();
}

real_vector& real_vector::operator+=( const real_vector& src )
{
  test_size(src.len(), len());
  for( int j = 0; j < _len; j++ )
    _data[j] += src[j];
  return *this;
}

real_vector& real_vector::operator-=( const real_vector& src )
{
  test_size(src.len(), len());
  for( int j = 0; j < _len; j++ )
    _data[j] -= src[j];
  return *this;
}

real_vector& real_vector::operator*=( real mu )
{
  for( int j = 0; j < _len; j++ )
    _data[j] *= mu;
  return *this;
}

real_vector& real_vector::operator/=( real mu )
{
  for( int j = 0; j < _len; j++ )
    _data[j] /= mu;
  return *this;
}

}; // namespace lwml
