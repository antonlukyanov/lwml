#include "clust.h"

#include "progress.h"

/*#lake:stop*/

namespace lwml {

clust::clust( const i_vector_set& cd, int cnum, int max_steps, tick_mode tick )
  : _len(cd.len()), _cnum(cnum), _dim(cd.dim()),
    _labels(cd.len()), _cvol(cnum), _kernel(cnum, cd.dim())
{
  init(cd);
  calc(cd, max_steps, tick);
}

void clust::init( const i_vector_set& cd )
{
  for( int j = 0; j < _len; j++ )
    _labels[j] = j % _cnum;
  calc_cvol();
  calc_kernels(cd);
}

void clust::calc_kernels( const i_vector_set& cd )
{
  _kernel.set_zero();
  for( int j = 0; j < _len; j++ ){
    for( int d = 0; d < _dim; d++ )
      _kernel(_labels[j], d) += cd(j, d) / _cvol[_labels[j]];
  }
}

void clust::calc_cvol()
{
  _cvol.set_zero();
  for( int j = 0; j < _len; j++ )
    _cvol[_labels[j]]++;
}

real clust::dist( int j, int cl, const i_vector_set& cd )
{
  real res = 0.0;
  for( int d = 0; d < _dim; d++ )
    res += fabs(cd(j, d) - _kernel(cl, d));
  return res;
}

int clust::find_best_class( int j, const i_vector_set& cd )
{
  int best_cl = 0;
  real best_dist = dist(j, best_cl, cd);
  for( int cl = 0; cl < _cnum; cl++ ){
    real cur_dist = dist(j, cl, cd);
    if( cur_dist < best_dist ){
      best_cl = cl;
      best_dist = cur_dist;
    }
  }
  return best_cl;
}

int clust::calc_labels( const i_vector_set& cd )
{
  int ch = 0;
  for( int j = 0; j < _len; j++ ){
    int cl = find_best_class(j, cd);
    ch += (cl != _labels[j]);
    _labels[j] = cl;
  }
  calc_cvol();
  return ch;
}

int clust::step( const i_vector_set& cd )
{
  int ch = calc_labels(cd);
  calc_kernels(cd);
  return ch;
}

void clust::calc( const i_vector_set& cd, int max_steps, tick_mode tick )
{
  if( tick == tmON )
    progress::start("clust", -1);
  int st;
  for( st = 1; st <= max_steps; st++ ){
    int ch = step(cd);
    if( ch == 0 ) break;
    if( tick == tmON )
      progress::up(st);
  }
  _steps = (st > max_steps) ? max_steps : st;
  if( tick == tmON )
    progress::finish();
}

}; // namespace lwml
