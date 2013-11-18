// lwml, (c) ltwood

#ifndef _MINEURO_
#define _MINEURO_

#include "defs.h"
#include "t_ring.h"
#include "matrix.h"
#include "basex.h"
#include "refcount.h"
#include "t_array.h"

/*#lake:stop*/

namespace lwml {

// параметры, определющие абстрактное уравнение нейрона

class i_equation_param : public interface, public refcount {
public:
  i_equation_param( real la ) { _la = la; }

  virtual real u0( real s ) const = 0;
  virtual real fna( real u ) const = 0;
  virtual real fka( real u ) const = 0;

  real la() const { return _la; };
  real al1() const { return fka(0.0) - 1.0; }
  real al2() const { return fna(0.0) + 1.0; }
  real al() const { return fka(0.0) - fna(0.0) - 1.0; }
  real T1() const { return 1.0 + al1(); }
  real T2() const { return 2.0 + al1() + al2()/al(); }

private:
  real _la; // большой параметр в уравнении нейрона
};

// параметры, определющие уравнение нейрона при
// заданном виде функций u0(), fna(), fka()

class std_equation_param : public i_equation_param { // public refcount
private:
  std_equation_param( real la, real Rna, real Rka );

public:
  static referer<std_equation_param> create( real la, real Rna, real Rka ) {
    return referer<std_equation_param>(new(lwml_alloc) std_equation_param(la, Rna, Rka));
  }

  virtual real u0( real s ) const;
  virtual real fna( real u ) const;
  virtual real fka( real u ) const;

  static real calc_u0( real Rna, real Rka ) { return sqrt(log(Rka-Rna)); }
  static real calc_la0( real Rna, real Rka ) {
    return (acos(Rna/Rka)/(2.0*log(Rka-Rna))) * sqrt((Rka-Rna)/(Rka+Rna));
  }

private:
  real _rna, _rka;
};

// общий динамический элемент со скалярным входом

class i_dynel : public interface, public refcount {
public:
  virtual real step( real du = 0.0 ) = 0;
  virtual real get() const = 0;
};

// численная схема решени уравнени нейрона

// Состояние is_spikestart() выставляется на один шаг в момент, когда
// впервые выставлено состояние is_inspike(), т.е. когда достигнут уровень val.
// Состояние is_spikeend() выставляется на один шаг в момент, когда
// впервые сброшено состояние is_inspike(), т.е. когда значение стало меньше val.
// Состояние is_max() выставляется на один шаг в момент, когда
// состояние is_dn() впервые выставлено после состояния is_up().
// Состояние is_min() выставляется на один шаг в момент, когда
// состояние is_up() впервые выставлено после состояния is_dn().

class scheme : public i_dynel { // public refcount
private:
  scheme( referer<i_equation_param> eq, int pp1, real val, int history_len );

public:
  // pp1 -- число точек на единицу длины
  // val -- уровень, соответствующий началу спайка
  static referer<scheme> create( referer<i_equation_param> eq, int pp1, real val = 1.0, int history_len = 0 ) {
    return referer<scheme>(new(lwml_alloc) scheme(eq, pp1, val, history_len));
  }

  referer<scheme> clone() {
    scheme* ptr = new(lwml_alloc) scheme(_eq, _pp1, _val, _tl.size());
    ptr->_tl = _tl;
    ptr->_t = _t;
    return referer<scheme>(ptr);
  }

  virtual real step( real du = 0.0 );
  virtual real get() const { return get_u(); }

  real get_t() const { return _t; }
  real get_dt() const { return _dt; }
  real get_u( int k = 0 ) const { return exp(_tl[k]); }
  real get_logu( int k = 0 ) const { return _tl[k]; }
  real get_l() const { return _eq->fka(exp(_tl[_pp1])) - _eq->fna(exp(_tl[0])) - 1.0; }

  bool is_inspike() const    { return exp(_tl[0]) >= _val; }
  bool is_spikestart() const { return exp(_tl[1]) < _val && exp(_tl[0]) >= _val; }
  bool is_spikeend() const   { return exp(_tl[1]) >= _val && exp(_tl[0]) < _val; }
  bool is_inspike( real val ) const    { return exp(_tl[0]) >= val; }
  bool is_spikestart( real val ) const { return exp(_tl[1]) < val && exp(_tl[0]) >= val; }
  bool is_spikeend( real val ) const   { return exp(_tl[1]) >= val && exp(_tl[0]) < val; }
  bool is_up() const         { return _tl[1] <= _tl[0]; }
  bool is_dn() const         { return _tl[1] > _tl[0]; }
  bool is_max() const        { return _tl[2] <= _tl[1] && _tl[1] > _tl[0]; }
  bool is_min() const        { return _tl[2] >= _tl[1] && _tl[1] < _tl[0]; }

//  real link2sigma( real link );
//  real sigma2link( real sigma );

private:
  t_ring<real> _tl;              // очередь прошлых значений
  referer<i_equation_param> _eq; // решаемое уравнение
  int _pp1;                      // число отрезков интегрировани на единицу времени
  real _dt;                      // шаг по времени
  real _t;                       // текущий момент времени
  real _val;                     // индикатор нахождени в спайке

  real next( real u10, real u00, real du );
};

// spike observer

class spike_observer : public value {
private:
  enum state { WAIT_START, WAIT_MAX, WAIT_END, WAIT_MIN, WAIT_RA, READY };

public:
  spike_observer( referer<scheme> );

  void reset();
  void update();

  bool is_ready() const { return _st == READY; }

  real ss_time()  const { test(); return _tss; }  // начало спайка
  real max_time() const { test(); return _tmax; } // максимум
  real se_time()  const { test(); return _tse; }  // конец спайка
  real min_time() const { test(); return _tmin; } // минимум
  real ra_time()  const { test(); return _tra; }  // реактивация

  real max_val()  const { test(); return _vmax; }
  real min_val()  const { test(); return _vmin; }

  real up_time() const  { return max_time() - ss_time(); }
  real dn1_time() const { return se_time() - max_time(); }
  real dn2_time() const { return min_time() - se_time(); }
  real time1() const    { return se_time() - ss_time(); }
  real time2() const    { return ra_time() - ss_time(); }

private:
  referer<scheme> _psch;
  state _st;

  real _tss, _tmax, _tse, _tmin, _tra;
  real _vmax, _vmin;

  void test() const;
};

// фабрика элементов матрицы из динамических элементов

class dynel_matrix_factory : public interface, public refcount {
public:
  virtual referer<i_dynel> create( int s, int c ) = 0;
  virtual void destroy( i_dynel* ) = 0;
};

// матрица динамических элементов

class dynel_matrix : public value {
public:
  dynel_matrix( int str, int col, referer<dynel_matrix_factory> fact );

  int str() const { return _str; }
  int col() const { return _col; }

  referer<i_dynel> operator()( int s, int c ) const {
    test_index2(s, _str, c, _col);
    return _data[index(s, c)];
  }

private:
  referer<dynel_matrix_factory> _factory;
  int _str, _col;        // число строк и столбцов
  t_array< referer<i_dynel> > _data;

  int index( int s, int c ) const { return s * _col + c; }
};

// сеть динамических элементов

class i_network2d : public interface, public value {
public:
  i_network2d( int str, int col, referer<dynel_matrix_factory> fact, real w );

  int str() const { return _str; }
  int col() const { return _col; }

  virtual real calcin( int s, int c ) = 0;

  void step();

  real get( int s, int c ) const { return _buffer(s, c); }
  real operator()( int s, int c ) const { return _buffer(s, c); }

private:
  int _str, _col;
  dynel_matrix _net;
  matrix _buffer;           // буфер выходов элементов сети
  real _weight;

  void updatebuffer();
};

// Правильная треугольная сеть динамических элементов

class hexagonal_network : public i_network2d { // public value
public:
  hexagonal_network( int str, int col, referer<dynel_matrix_factory> fact, real w );

  virtual real calcin( int s, int c );

private:
  real sget( int s, int c );
};

}; // namespace lwml

#endif // _MINEURO_
