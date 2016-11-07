#include "lwml/m_alg/cga.h"

/*#lake:stop*/

namespace lwml {

real_ga::real_ga( int dim, int len, real alpha, real prtr, uint seed )
  : _dim(dim), _len(len), _alpha(alpha), _prtr(prtr), _rnd(seed),
    _pop1(len, dim), _pop2(len, dim),
    _cost(len), _child(dim), _state(true)
{
  _min_score_idx = -1;  // защита от попытки выполнить get() до start()
}

void real_ga::update_opt()
{
  _min_score = REAL_MAX;
  _min_score_idx = -1;

  for( int k = 0; k < _len; k++ )
    if( _cost[k] < _min_score ){
      _min_score = _cost[k];
      _min_score_idx = k;
    }
}

void real_ga::start( const i_vfunction& fn, real a, real b )
{
  matrix& crnt = get_crnt();
  vector x(_dim);
  for( int k = 0; k < _len; k++ ){
    for( int j = 0; j < _dim; j++ )
      crnt(k, j) = scale::coord(_rnd.get_real(), 0.0, 1.0, a, b);
    crnt.get_str(x, k);
    _cost[k] = fn.func(x);
  }
  update_opt();
}

void real_ga::start( const i_vfunction& fn, const vector& a, const vector& b )
{
  test_size2(a.len(), _dim, b.len(), _dim);

  matrix& crnt = get_crnt();
  vector x(_dim);
  for( int k = 0; k < _len; k++ ){
    for( int j = 0; j < _dim; j++ )
      crnt(k, j) = scale::coord(_rnd.get_real(), 0.0, 1.0, a[j], b[j]);
    crnt.get_str(x, k);
    _cost[k] = fn.func(x);
  }
  update_opt();
}

void real_ga::select3( int k, int& a, int& b, int& c )
{
  do a = _rnd.get_int(_len); while( a == k );
  do b = _rnd.get_int(_len); while( b == k || b == a );
  do c = _rnd.get_int(_len); while( c == k || c == a || c == b );
}

real real_ga::step( const i_vfunction& fn )
{
  _penalty_count = 0;
  _sur_count = 0;
  const matrix& crnt = get_crnt();
  matrix& next = get_next();
  for( int k = 0; k < _len; k++ ){
    int a, b, c;
    select3(k, a, b, c);
    int j = _rnd.get_int(_dim);
    for( int jj = 0; jj < _dim; jj++ ){
      if( _rnd.get_real() < _prtr || jj == _dim-1 )
        _child[j] = crnt(c, j) + _alpha * (crnt(a, j) - crnt(b, j));
      else
        _child[j] = crnt(k, j);
      j = (j + 1) % _dim;
    }

    real score = fn.func(_child);
    if( score == REAL_MAX )
      _penalty_count++; // учет штрафа
    if( score <= _cost[k] ){
      next.put_str(_child, k);
      _cost[k] = score;
      _sur_count++; // учет выживаемости детей
    } else {
      for( int s = 0; s < _dim; s++ )
        next(k, s) = crnt(k, s);
    }
  }

  _state = !_state;     // смена ролей популяций

  update_opt();
  return _min_score;
}

real real_ga::steps( const i_vfunction& fn, int max_gen )
{
  real score = 0.0;
  for( int j = 0; j < max_gen; j++ )
    score = step(fn);
  return score;
}

void real_ga::diam( vector& diam ) const
{
  vector ax(_dim), bx(_dim);
  const matrix& data = get_crnt();
  data.get_str(ax, 0);
  data.get_str(bx, 0);
  for( int k = 0; k < _len; k++ ){
    for( int j = 0; j < _dim; j++ ){
      real val = data(k, j);
      if( val < ax[j] ) ax[j] = val;
      if( val > bx[j] ) bx[j] = val;
    }
  }
  for( int j = 0; j < _dim; j++ )
    diam[j] = bx[j] - ax[j];
}

}; // namespace lwml
