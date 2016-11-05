#include "mineuro.h"

#include "stdmem.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

// std_equation_param

std_equation_param::std_equation_param( real la, real Rna, real Rka )
  : i_equation_param(la), _rna(Rna), _rka(Rka)
{
}

real std_equation_param::u0( real s ) const
{
  return (1.0/la()) * exp(la() * al() * s);
}

const int MAX_POWER = 100;

real std_equation_param::fna( real u ) const
{
  real p = fsqr(u);
  if( p > MAX_POWER )
    return 0.0;
  else
    return _rna * exp(-p);
}

real std_equation_param::fka( real u ) const
{
  real p = fsqr(u);
  if( p > MAX_POWER )
    return 0.0;
  else
    return _rka * exp(-p);
}

// scheme

scheme::scheme( referer<i_equation_param> eq, int pp1, real val, int history_len )
  : _tl(history_len < pp1 + 1 ? pp1 + 1 : history_len)
{
  _eq = eq;
  _pp1 = pp1;
  _dt = 1.0 / real_cast(pp1);
  _t = 0.0;
  for( int j = _tl.size() - 1; j >= 0; j-- )
    _tl.push(log(_eq->u0(-j * _dt)));

  _val = val;
}

real scheme::next( real lu1, real lu0, real du )
{
  real y = _eq->fka(exp(lu1)) - _eq->fna(exp(lu0)) - 1.0;
  return lu0 + _dt * (_eq->la() * y + du / exp(lu0));
}

real scheme::step( real du )
{
  real lu1 = _tl[_pp1];
  real lu0 = _tl[0];
  real lu01 = next(lu1, lu0, du);
  _t += _dt;
  _tl.roll(lu01);
  return exp(lu01);
}

/*
real mineuron::link2sigma( real link )
{
  return -log(link) / par->la;
}

real mineuron::sigma2link( real sigma )
{
  return exp(-par.la * sigma);
}
*/

// spike_observer

spike_observer::spike_observer( referer<scheme> psch )
{
  _psch = psch;
  reset();
}

void spike_observer::reset()
{
  _st = WAIT_START;
  update();
}

void spike_observer::test() const
{
  if( _st != READY )
    fail_assert("using of spike_observer before end of spike");
}

void spike_observer::update()
{
  switch( _st ){
    case WAIT_START:
      if( _psch->is_spikestart() ){
        _tss = _psch->get_t();
        _st = WAIT_MAX;
      }
      break;
    case WAIT_MAX:
      if( _psch->is_max() ){
        _tmax = _psch->get_t();
        _vmax = _psch->get_u();
        _st = WAIT_END;
      }
      break;
    case WAIT_END:
      if( _psch->is_spikeend() ){
        _tse = _psch->get_t();
        _st = WAIT_MIN;
      }
      break;
    case WAIT_MIN:
      if( _psch->is_min() ){
        _tmin = _psch->get_t();
        _vmin = _psch->get_u();
        _st = WAIT_RA;
      }
      break;
    case WAIT_RA:
      if( _psch->is_spikestart() ){
        _tra = _psch->get_t();
        _st = READY;
      }
      break;
    case READY:
      break;
  }
}

// dynel_matrix

dynel_matrix::dynel_matrix( int str, int col, referer<dynel_matrix_factory> fact )
{
  _factory = fact;
  _str = str;
  _col = col;
  _data.resize(_str * _col);
  for( int s = 0; s < _str; s++ ){
    for( int c = 0; c < _col; c++ )
      _data[index(s, c)] = _factory->create(s, c);
  }
}

// network

i_network2d::i_network2d( int str, int col, referer<dynel_matrix_factory> fact, real w )
  : _net(str, col, fact), _buffer(str, col)
{
  _str = str;
  _col = col;
  updatebuffer();
  _weight = w;
}

void i_network2d::updatebuffer()
{
  for( int s = 0; s < _str; s++ )
    for( int c = 0; c < _col; c++ )
      _buffer(s, c) = _net(s, c)->get();
}

void i_network2d::step()
{
  for( int s = 0; s < _str; s++ )
    for( int c = 0; c < _col; c++ )
      _buffer(s, c) = _net(s, c)->step(_weight * calcin(s, c));
  updatebuffer();
}

// hexagonal_network

hexagonal_network::hexagonal_network( int str, int col, referer<dynel_matrix_factory> fact, real w )
  : i_network2d(str, col, fact, w)
{
}

real hexagonal_network::sget( int s, int c )
{
  if( 0 <= s && s < str() && 0 <= c && c < col() )
    return get(s, c);
  else
    return 0.0;
}

real hexagonal_network::calcin( int s, int c )
{
  return sget(s-1, c) + sget(s-1, c+1) +
         sget(s, c-1) + sget(s, c+1) +
         sget(s+1, c) + sget(s+1, c+1) -
         6.0 * sget(s, c);
}

}; // namespace lwml
