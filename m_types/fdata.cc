#include "lwml/m_types/fdata.h"
#include "lwml/types/cstrng.h"
#include "lwml/alg/search.h"
#include "lwml/alg/t_sorter.h"
#include "lwml/base/basex.h"

/*#lake:stop*/

namespace lwml {

funcdata::funcdata( const char* fname )
{
  referer<stream> file = stream::create(fname, fmREAD, fmTEXT);
  _len = file->count_lines();
  _arg.resize(_len);
  _dat.resize(_len);
  vector vx(_len), vy(_len);
  for( int j = 0; j < _len; j++ ){
    strng line;
    if( !file->get_str(line) )
      throw ex_funcdata("%s", fname);
    double x, y;
    if( sscanf(line.ascstr(), "%lf %lf", &x, &y) != 2 )
      throw ex_funcdata("%s", fname);
    vx[j] = x;
    vy[j] = y;
  }
  t_sorter<vector> idx(vx);
  for( int j = 0; j < _len; j++ ){
    _arg[j] = vx[idx[j]];
    _dat[j] = vy[idx[j]];
  }
  _ax = _arg[0];
  _bx = _arg[_len-1];
}

funcdata::funcdata( const vector& vx, const vector& vy )
{
  test_size(vy.len(), vx.len());

  _len = vx.len();
  _arg.resize(_len);
  _dat.resize(_len);

  t_sorter<vector> idx(vx);
  for( int j = 0; j < _len; j++ ){
    _arg[j] = vx[idx[j]];
    _dat[j] = vy[idx[j]];
  }
  _ax = _arg[0];
  _bx = _arg[_len-1];
}

real funcdata::delta()
{
  return (_bx - _ax) / (_len-1);
}

real funcdata::delta_var()
{
  real mid = delta();
  real err = 0.0;
  for( int j = 1; j < _len; j++ ){
    real delta = _arg[j] - _arg[j-1];
    err += fabs(delta - mid);
  }
  err /= _len - 1;
  return err;
}

void funcdata::print_resume( referer<stream> file )
{
  file->printf("funcdata: len=%d ax=%g bx=%g dx=%g dxv=%g\n", _len, _ax, _bx, delta(), delta_var());
}

void funcdata::get_arg( vector& dst )
{
  test_size(dst.len(), _len);
  dst = _arg;
}

void funcdata::get_val( vector& dst )
{
  test_size(dst.len(), _len);
  dst = _dat;
}

real funcdata::get_interp( vector& dst )
{
  real dh = (_bx - _ax) / dst.len();
  real h = _ax;
  for( int j = 0; j < dst.len(); j++ ){
    int idx = search::interval(_arg, h);
    real a = _arg[idx];
    real b = _arg[idx+1];
    real va = _dat[idx];
    real vb = _dat[idx+1];

    dst[j] = va + (vb-va) * (h-a) / (b-a);
    h += dh;
  }
  return dh;
}

}; // namespace lwml
