#include "hooke.h"

/*#lake:stop*/

namespace lwml {

// Поиск в окрестности.
// Для заданной точки x[] выбрать лучшего соседа, изменяя по одной
// координате за раз на величину +/-dx[j].
// fmin -- значение функции в точке x[].
// Функция возвращает значение функции во вновь выбранной точке.
// Знаки шагов dx[] существенны, поскольку сначала производится сдвиг
// в направлении +dx[j] и при уменьшении значения функции попытка
// сдвига в обратном направлении не производится.

real hooke::local_search( const i_vfunction& fn, const vector& dx, vector& x, real fmin )
{
  for( int i = 0; i < dx.len(); i++ ){
    x[i] += dx[i];
    real ftmp = fn(x);
    if( ftmp < fmin )
      fmin = ftmp;
    else {
      x[i] -= 2.0 * dx[i];
      ftmp = fn(x);
      if( ftmp < fmin )
        fmin = ftmp;
      else
        x[i] += dx[i];
    }
  }
  return fmin;
}

int hooke::calc(
  const i_vfunction& fn, vector& x0, vector& x, vector& dx, real rho, real eps, int maxit,
  i_hooke_progress* prg
)
{
  real step = 1.0;
  real log_step0 = log(step);
  real log_eps = log(eps);
  int iters;

  real f0 = fn(x0);
  x = x0;
  for( iters = 0; iters < maxit; ++iters ){
    if( prg ){
      real f1 = scale::coord(iters, 0, maxit-1, 0.0, 1.0);
      real f2 = scale::coord(log(step), log_step0, log_eps, 0.0, 1.0);
      prg->up(t_max(f1, f2));
    }

    real fx = local_search(fn, dx, x, f0);
    if( fx >= f0 ){
      if( (step *= rho) < eps )
        break;
      dx *= rho;
      x = x0;
      continue;
    }

    for( int i = 0; i < x.len(); i++ ){
      dx[i] = (x[i] <= x0[i]) ? -fabs(dx[i]) : fabs(dx[i]);
      real dx = x[i] - x0[i];
      x0[i] = x[i];
      x[i] += dx;
    }
    f0 = fx;
  }
  x = x0;

  return iters;
}

int hooke::calc(
  const i_vfunction& fn, vector& x0, vector& x, real dx_val, double rho, double eps, int maxit,
  i_hooke_progress* prg
)
{
  vector dx(x0.len(), dx_val);
  return calc(fn, x0, x, dx, rho, eps, maxit, prg);
}

}; // namespace lwml
