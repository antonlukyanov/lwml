//  ластеризатор (алгоритм динамических €дер)
// lwml, (c) ltwood

#ifndef _CLUST_
#define _CLUST_

#include "ivector.h"
#include "i_vset.h"
#include "matrix.h"

/*#lake:stop*/

namespace lwml {

class clust : public value {
public:
  clust( const i_vector_set&, int cnum, int max_steps = 100, tick_mode tick = tmOFF );

  int len() const { return _len; }
    // общее число векторов
  int cnum() const { return _cnum; }
    // число кластеров
  int dim() const { return _dim; }
    // размерность векторов
  int steps() const { return _steps; }
    // число шагов кластеризатора

  // ѕолучение номера кластера дл€ вектора с номером idx.
  int operator[]( int idx ) const {
    test_index(idx, _len);
    return _labels[idx];
  }
  
  // ѕолучение числа векторов в кластере cidx.
  int cvol( int cidx ) const {
    test_index(cidx, _cnum);
    return _cvol[cidx];
  }

  // ѕолучение координаты didx дл€ €дра cidx.
  real operator()( int cidx, int didx ) const {
    test_index(cidx, _cnum);
    test_index(didx, _dim);
    return _kernel(cidx, didx);
  }

private:
  int _len;
  int _cnum;
  int _dim;
  int_vector _labels;
  int_vector _cvol;
  matrix _kernel;
  int _steps;

  void init( const i_vector_set& cd );
  void calc_kernels( const i_vector_set& cd );
  void calc_cvol();
  real dist( int j, int cl, const i_vector_set& d );
  int find_best_class( int j, const i_vector_set& d );
  int calc_labels( const i_vector_set& cd );
  int step( const i_vector_set& cd );
  void calc( const i_vector_set& cd, int max_steps, tick_mode tick );
};

}; // namespace lwml

#endif // _CLUST_
