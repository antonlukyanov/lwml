#include "lwml/utils/debug.h"
#include "lwml/m_types/ivector.h"
#include "lwml/console/progress.h"

#include "lwml/classification/iclassifier.h"

/*#build_stop*/

namespace lwml {

/**
 * Формирует взвешенный усредненный вектор по векторам, относящимся к заданному классу.
 * @param mid Возвращает количество образов в заданном классе
 * @param vs Набор векторов данного класса
 * @param w Веса векторов
 */
void project_classifier::set_mid( vector& mid, const i_vector_set& vs, const vector& w ) const
{
  vector v(vs.dim());
  mid.set_zero();
  for( int j = 0; j < vs.len(); j++ ){
    real ww = w[j];
    vs.get(v, j);
    for( int k = 0; k < mid.len(); k++ )
      mid[k] += ww * v[k];
  }
  mid /= w.inner_sum();
}

project_classifier::project_classifier( int dim, referer<luaconf> cnf, const char* root )
  : _dir(dim)
{
  for( int i = 0; i < _dir.len(); i++ )
    _dir[i] = cnf->get_real("%s.direction[%d]", root, i+1);

  _thr = cnf->get_real("%s.threshold", root);
  _is_invers = cnf->get_bool("%s.is_invers", root);
}

project_classifier::project_classifier( const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2 )
  : _dir(vs1.dim()), _is_invers(false)
{
  test_size(vs1.dim(), vs2.dim());
  test_size(vs1.len(), w1.len());
  test_size(vs2.len(), w2.len());
  int dim = vs1.dim();

  vector mid1(dim), mid2(dim);
  set_mid(mid1, vs1, w1);
  set_mid(mid2, vs2, w2);

  for( int j = 0; j < dim; j++ )
    _dir[j] = mid2[j] - mid1[j];

  // заполняем вектор проекций
  int slen = vs1.len() + vs2.len();
  vector p(slen);
  vector x(dim);
  for( int j = 0; j < vs1.len(); j++ ){
    vs1.get(x, j);
    p[j] = vector::inner_mul(x, _dir);
  }
  int sh = vs1.len();
  for( int j = 0; j < vs2.len(); j++ ){
    vs2.get(x, j);
    p[sh + j] = vector::inner_mul(x, _dir);
  }
  t_sorter<vector> sort(p);

  // заполняем векторы, хранящие суммарный вес точек,
  // лежащих левее (нестрого) данной точки в каждом из множеств
  vector sum1(slen), sum2(slen);
  sum1[0] = sum2[0] = 0.0;
  if( sort[0] < sh ) // первая точка лежит в первом множестве
    sum1[0] = w1[sort[0]];
  else // первая точка лежит во втором множестве
    sum2[0] = w2[sort[0] - sh];
  for( int j = 1; j < slen; j++ ){
    sum1[j] = sum1[j-1];
    sum2[j] = sum2[j-1];
    if( sort[j] < sh ) // очередная точка лежит в первом множестве
      sum1[j] += w1[sort[j]];
    else // очередная точка лежит во втором множестве
      sum2[j] += w2[sort[j] - sh];
  }

  real gsum1 = w1.inner_sum();
  real min_v = 1;
  int min_j = -1;
  int is_ok = 0;
  for( int j = 0; j < slen - 1; j++ ){
    // первое слагаемое - сумма весов точек, лежащих строго правее данной
    real error = (gsum1 - sum1[j]) + sum2[j];
    // на самом деле точки по этой координате не отделимы
    if( p[sort[j]] == p[sort[j+1]] ){
      zzz("project_classifier: can't separate points p[%d]=%f and p[%d]=%f", j, p[sort[j]], j+1, p[sort[j+1]]);
      continue;
    }
    bool invers = false;
    if( error > 0.5 ){
      error = 1 - error;
      invers = true;
    }

    // еще не было разделяющей гиперплоскости или была, но на новой меньше ошибка
    if( is_ok == 0 || (is_ok == 1 && error < min_v) ){
      is_ok = 1;
      min_v = error;
      min_j = j;
      _is_invers = invers;
    }
  }

  // точки не отделимы
  if( min_j == -1 ){
    zzz("project_classifier: can't separate projections");
    _thr = p[sort[min_j+1]];
    return;
  }
  if( min_j == slen-1 )
    runtime("adaboost::mk_simp_classifier: incorrect minimum index");

  _thr = 0.5 * (p[sort[min_j]] + p[sort[min_j+1]]);
}

int project_classifier::classify( const vector& x ) const
{
  test_size(x.len(), _dir.len());

  return _is_invers^(vector::inner_mul(x, _dir) > _thr);
}

void project_classifier::get_weights( vector& w ) const
{
  test_size(w.len(), _dir.len());
  w = _dir;
  w /= _dir.norm();
}

void project_classifier::serialize( referer<luaconf> cnf, const char* root ) const
{
  cnf->exec("%s = {}", root);
  cnf->exec("%s.direction = {}", root);
  for( int i = 0; i < _dir.len(); i++ )
    cnf->exec("%s.direction[%d] = %.12g", root, i+1, _dir[i]);
  cnf->exec("%s.threshold = %.12g", root, _thr);
  if( _is_invers )
    cnf->exec("%s.is_invers = true", root);
  else
    cnf->exec("%s.is_invers = false", root);
}

const char* project_classifier::get_name() const
{
  return "project_classifier";
}

/**
 * Подсчет ошибки.
 *
 * @Todo: поменять "real*" и "bool*" на ссылки.
 *
 * @param vs1 Набор векторов первого класса
 * @param w1 Веса для векторов первого класса
 * @param vs2 Набор векторов первого класса
 * @param w2 Веса для векторов второго класса
 * @param cidx
 * @param thr
 * @param is_invers
 * @param buf
 * @param sort Отсортированный вектор проекций
 * @param sum1 Суммарный вес точек перовго класса
 * @param sum2 Суммарный вес точек второго класса
 * @return
 */
real coord_classifier::calc_error(
  const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2, int cidx, real* thr, bool * is_invers,
  vector& buf, t_sorter<vector>& sort, vector& sum1, vector& sum2
) const
{
  // Заполняем и сортируем вектор проекций.
  for( int j = 0; j < vs1.len(); j++ )
    buf[j] = vs1.get(j, cidx);
  int sh = vs1.len();
  for( int j = 0; j < vs2.len(); j++ )
    buf[sh + j] = vs2.get(j, cidx);
  sort.put(buf);

  // Заполняем векторы, хранящие суммарный вес точек,
  // лежащих левее (нестрого) данной точки в каждом из множеств.
  sum1[0] = sum2[0] = 0.0;
  // Первая точка лежит в первом множестве.
  if( sort[0] < sh )
    sum1[0] = w1[sort[0]];
  // Первая точка лежит во втором множестве.
  else
    sum2[0] = w2[sort[0] - sh];

  int slen = vs1.len() + vs2.len();
  for( int j = 1; j < slen; j++ ){
    sum1[j] = sum1[j-1];
    sum2[j] = sum2[j-1];
    // Очередная точка лежит в первом множестве.
    if( sort[j] < sh )
      sum1[j] += w1[sort[j]];
    // Очередная точка лежит во втором множестве.
    else
      sum2[j] += w2[sort[j] - sh];
  }

  // Отыскиваем оптимальный порог.
  // gsum1 равен сумме элементов вектора w1, т.к. sum1[k] = sum1[k-1] + w1[sort[k]], где sort[k] -
  // перестановка и sum1[0] = w1[sort[0]].
  real gsum1 = sum1[slen - 1];
  real min_error = 1.0;
  int min_j = -1;
  int is_ok = 0;

  *is_invers = false;
  for( int j = 0; j < slen-1; j++ ){
    // Первое слагаемое - сумма весов точек, лежащих строго правее данной.
    real error = (gsum1 - sum1[j]) + sum2[j];

    // Если ошибка больше 0.5, то тогда меняем классы местами и получаем ошибку меньше 0.5.
    bool invers = false;
    if( error > 0.5 ){
      error = 1.0 - error;
      invers = true;
    }

    // На самом деле точки по этой координате не отделимы.
    if( buf[sort[j]] == buf[sort[j+1]] )
      continue;

    // Еще не было разделяющей гиперплоскости или была, но на новой меньше ошибка.
    if( is_ok == 0 || (is_ok == 1 && error < min_error) ){
      min_error = error;
      min_j = j;
      is_ok = 1;
      *is_invers = invers;
    }
  }

  // min_j м.б. равен -1 только когда в buf одни и те же значения.
  if( min_j == -1 ){
    *thr = buf[sort[min_j+1]];
    // Типa все точки попали не туда.
    // По этой координате классы не отделимы, 1 - максимальное значение ошибки.
    return 1.0;
  }

  // Такое возможно только в ситуации, когда ошибка постоянно уменьшается.
  // Отделить значения на последнем индексе невозможно.
  if( min_j == slen-1 )
    runtime("adaboost::mk_simp_classifier: incorrect minimum index");

  *thr = 0.5 * (buf[sort[min_j]] + buf[sort[min_j+1]]);

  return min_error;
}

coord_classifier::coord_classifier( int dim, referer<luaconf> cnf, const char* root )
{
  _dim = dim;
  _cidx = cnf->get_int("%s.coord_idx", root);
  _thr = cnf->get_real("%s.threshold", root);
  _is_invers = cnf->get_bool("%s.is_invers", root);
}

coord_classifier::coord_classifier( const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2 )
{
  test_size(vs1.dim(), vs2.dim());
  test_size(vs1.len(), w1.len());
  test_size(vs2.len(), w2.len());
  _dim = vs1.dim();

  int slen = vs1.len() + vs2.len();
  vector buf(slen), sum1(slen), sum2(slen);
  t_sorter<vector> sort(slen);

  real opt_thr;
  int opt_cidx = 0;
  bool is_invers = false;
  real opt_error = calc_error(vs1, w1, vs2, w2, opt_cidx, &opt_thr, &is_invers, buf, sort, sum1, sum2);
  for( int j = 1; j < _dim; j++ ){
    real thr;
    bool invers;
    real error = calc_error(vs1, w1, vs2, w2, j, &thr, &invers, buf, sort, sum1, sum2);
    if( error < opt_error ){
      opt_error = error;
      opt_thr = thr;
      opt_cidx = j;
      is_invers = invers;
    }
  }
  _cidx = opt_cidx;
  _thr = opt_thr;
  _is_invers = is_invers;
  //zzz("coord_classifier: opt_cidx=%d _thr=%f _is_invers=%d", _cidx, _thr, _is_invers);
}

void coord_classifier::get_weights( vector& w ) const
{
  test_size(w.len(), _dim);
  w.set_zero();
  w[_cidx] = 1.0;
}

void coord_classifier::serialize( referer<luaconf> cnf, const char* root ) const
{
  cnf->exec("%s = {}", root);
  cnf->exec("%s.coord_idx = %d", root, _cidx);
  cnf->exec("%s.threshold = %.12g", root, _thr);
  if( _is_invers )
    cnf->exec("%s.is_invers = true", root);
  else
    cnf->exec("%s.is_invers = false", root);
}

const char* coord_classifier::get_name() const
{
  return "coord_classifier";
}


} /* namespace lwml */
