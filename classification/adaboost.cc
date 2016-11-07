#include "lwml/classification/adaboost.h"

#include "lwml/utils/debug.h"
#include "lwml/m_types/ivector.h"
#include "lwml/console/progress.h"

/*#build_stop*/

namespace lwml {

// AdaBoost Ctors ans Ctor utilities

adaboost::adaboost( referer<luaconf> cnf, const char* root )
: _is_test(cnf->get_int("#%s.qualities", root) != 1),
  _dim(cnf->get_int("%s.dim", root)), _step_num(cnf->get_int("%s.classifier_num", root)),
  _alpha(_step_num),
  _p1(cnf->get_int("#%s.p1", root)), _p2(cnf->get_int("#%s.p2", root)),
  _cl(_step_num), _qualities(cnf->get_int("#%s.qualities", root), vector(_dim))
{
  refowner<i_simple_classifier_maker> sc_fact;
  cstrng cl_type = cnf->get_str("%s.simple_classifier", root);
  if( cl_type.is_eq("project_classifier") )
    sc_fact.reset(new project_classifier_maker());
  else if( cl_type.is_eq("coord_classifier") )
    sc_fact.reset(new coord_classifier_maker());
  else
    fail_assert("adaboost::adaboost(): incorrect simple_classifier type");

  for( int i = 0; i < _alpha.len(); i++ )
    _alpha[i] = cnf->get_real("%s.alpha[%d]", root, i+1);
  for( int i = 0; i < _p1.len(); i++ ){
    _p1[i] = cnf->get_real("%s.p1[%d]", root, i+1);
    _p2[i] = cnf->get_real("%s.p2[%d]", root, i+1);
  }

  for( int i = 0; i < _cl.len(); i++ ){
    strng str = strng::form("%s.simple_classifiers[%d]", root, i+1);
    _cl[i].reset(sc_fact->create(_dim, cnf, str.ascstr()));
  }

  for( int i = 0; i < _qualities.len(); i++ ){
    for( int j = 0; j < _dim; j++ )
      _qualities[i][j] = cnf->get_real("%s.qualities[%d][%d]", root, i+1, j+1);
  }

}

// размерности векторов в наборах полагаются одинаковыми
// длины векторов весов согласованы с длинами наборов векторов
real adaboost::calc_error(
  const i_vector_set& vs1, const vector& w1,
  const i_vector_set& vs2, const vector& w2,
  const i_simple_classifier* cl
) const
{
  real res = 0.0;
  vector x(vs1.dim());
  for( int j = 0; j < vs1.len(); j++ ){
    vs1.get(x, j);
    res += (cl->classify(x) == 0) ? 0.0 : w1[j];
  }
  for( int j = 0; j < vs2.len(); j++ ){
    vs2.get(x, j);
    res += (cl->classify(x) == 0) ? w2[j] : 0.0;
  }
  return res;
}

real adaboost::recalc_weights(
  const i_vector_set& vs1, const i_vector_set& vs2,
  const i_simple_classifier* cl, real error
)
{
  test_size(vs1.dim(), vs2.dim());

  if( error == 0 )
    return 1.0;

  real alpha = 0.5 * log(1.0/error - 1.0);

  vector x(vs1.dim());
  for( int j = 0; j < vs1.len(); j++ ){
    vs1.get(x, j);
    bool is_ok = (cl->classify(x) == 0);
    _w1[j] *= exp(-alpha * (is_ok ? 1.0 : -1.0));
  }
  for( int j = 0; j < vs2.len(); j++ ){
    vs2.get(x, j);
    bool is_ok = (cl->classify(x) == 1);
    _w2[j] *= exp(-alpha * (is_ok ? 1.0 : -1.0));
  }

  real sum = _w1.inner_sum() + _w2.inner_sum();
  if( sum != 0 ){
    _w1 /= sum;
    _w2 /= sum;
  }
  return alpha;
}

// размерности векторов в наборах полагаются одинаковыми
void adaboost::mk_classifier(
  const i_simple_classifier_maker& sc_fact,
  const i_vector_set& vs1, const i_vector_set& vs2,
  int num, tick_mode tick
)
{
  int m = vs1.len() + vs2.len();
  _w1.set_val(1.0 / m);
  _w2.set_val(1.0 / m);

  if( tick == tmON )
    progress::start("adaboost", num);

  for( int j = 0; j < num; j++ ){
    _cl[j].reset(sc_fact.create(vs1, _w1, vs2, _w2));
    real error = calc_error(vs1, _w1, vs2, _w2, _cl[j].get());
    if( error >= 0.5 )
      runtime("adaboost::mk_classifier: too large error");
    _alpha[j] = recalc_weights(vs1, vs2, _cl[j].get(), error);

    _step_num++;

    if( error == 0.0 ){ // все нашли идеально
      if( tick == tmON )
        progress::finish();
      return;
    }
    else if( tick == tmON )
      progress::up(j);
  }
  if( tick == tmON )
    progress::finish();
}

void adaboost::calc_part_confidence( const vector& x, int cl )
{
  // пробегаемся по всем шагам классификатора
  vector cl_value(_step_num, 0.0);
  classify(x, cl_value);
  for( int k = 0; k < step_num(); k++ ){
    if( cl == 1 ){
      bool is_ok = (cl_value[k] < 0.0); // классифицировали в 1-й класс
      _n_r1[k] += is_ok ? 1 : 0;
      _n_w1[k] += !is_ok ? 1 : 0;
    }
    else if( cl == 2 ){
      bool is_ok = !(cl_value[k] < 0.0); // классифицировали во 2-й класс
      _n_r2[k] += is_ok ? 1 : 0;
      _n_w2[k] += !is_ok ? 1 : 0;
    }
  }
}

void adaboost::calc_confidences( const i_vector_set& vs1, const i_vector_set& vs2 )
{
  test_size(vs1.dim(), _dim);
  test_size(vs2.dim(), _dim);

  // пробегаемся по всем точкам обоих классов
  vector x(vs1.dim());
  for( int j = 0; j < vs1.len(); j++ ){
    vs1.get(x, j);
    calc_part_confidence(x, 1);
  }
  for( int j = 0; j < vs2.len(); j++ ){
    vs2.get(x, j);
    calc_part_confidence(x, 2);
  }

  // вычисление условных вероятностей
  if( _is_test ){
    int k = step_num()-1;
    _p1[0] = (double)_n_r1[k]/ (_n_r1[k] + _n_w1[k]);
    _p2[0] = (double)_n_r2[k]/ (_n_r2[k] + _n_w2[k]);
  }
  else{
    for( int k = 0; k < step_num(); k++ ){
      _p1[k] = (double)_n_r1[k]/ (_n_r1[k] + _n_w1[k]);
      _p2[k] = (double)_n_r2[k]/ (_n_r2[k] + _n_w2[k]);
    }
  }
}

// Оценивает качество признаков на заданной тестовой выборке.
// Усредняет веса координат классифицируемого вектора,
// в усреднении участвуют только те простые классификаторы,
// которые сработали на одном из векторов тестовой выборки.
void adaboost::calc_feature_quality( const i_vector_set& vs1, const i_vector_set& vs2 )
{
  test_size(vs1.dim(), _dim);
  test_size(vs2.dim(), _dim);

  int_vector vnums(_step_num, 0.0);
  vector x(_dim);
  for( int j = 0; j < vs1.len(); j++ ){
    vs1.get(x, j);
    calc_feature_quality(vnums, x);
  }
  for( int j = 0; j < vs2.len(); j++ ){
    vs2.get(x, j);
    calc_feature_quality(vnums, x);
  }

  // мы посчитали качество и количество точек для каждого шага
  // теперь записываем значения - сумма до этого шага
  for( int k = 0; k < step_num() -1; k++ ){
    _qualities[k+1] += _qualities[k];
    vnums[k+1] += vnums[k];
  }

  // вычисляем среднее значение
  for( int k = 0; k < step_num(); k++ ){
    _qualities[k] /= vnums[k];
  }
}

// Считает качества признаков для
void adaboost::calc_feature_quality( int_vector& vnums, const vector& x )
{
  if( _step_num == 0 )
    return;
  test_size(vnums.len(), _step_num);
  vector buf(_dim);
  vector cl_value(_step_num, 0.0);
  classify(x, cl_value);
  for( int k = 0; k < step_num(); k++ ){
    if( k == 0|| cl_value[k-1]*cl_value[k] < 0 ){ // сумма сменила знак
      _cl[k]->get_weights(buf);
      vnums[k]++;
      _qualities[k] += buf;
    }
  }
}

// AdaBoost regular methods

int adaboost::classify( const vector& x, int num ) const
{
  test_size(x.len(), _dim);

  if( num == 0 || num > step_num() )
    num = step_num();
  if( num < 1 )
    runtime("adaboost::classify: incorrect number of classifiers");

  real sum = 0.0;
  for( int j = 0; j < num; j++ ){
    real v = (_cl[j]->classify(x) == 0) ? -1.0 : 1.0;
    sum += _alpha[j] * v;
  }
  return (sum < 0.0) ? 0 : 1;
}

void adaboost::classify( const vector& x, vector& cl_value ) const
{
  int st_num = t_min(cl_value.len(), step_num());
  cl_value.set_val(0.0);
  real classify_sum = 0;
  // пробегаемся по всем шагам классификатора
  for( int k = 0; k < st_num; k++ ){
    real v = (_cl[k]->classify(x) == 0) ? -1.0 : 1.0;
    classify_sum += _alpha[k] * v;
    cl_value[k] = classify_sum;
  }

  // дозаполняем вектор, если надо
  if( cl_value.len() > step_num() ){
    for( int k = st_num; k < cl_value.len(); k++ )
      cl_value[k] = cl_value[st_num-1];
  }
}

real adaboost::calc_error( const i_vector_set& vs1, const i_vector_set& vs2, int num ) const
{
  test_size(vs1.dim(), _dim);
  test_size(vs2.dim(), _dim);

  int err_num = 0;
  vector x(vs1.dim());
  for( int j = 0; j < vs1.len(); j++ ){
    vs1.get(x, j);
    err_num += (classify(x, num) != 0);
  }
  for( int j = 0; j < vs2.len(); j++ ){
    vs2.get(x, j);
    err_num += (classify(x, num) != 1);
  }
  return real_cast(err_num) / (vs1.len() + vs2.len());
}

void adaboost::calc_errors( const i_vector_set& vs1, const i_vector_set& vs2, vector& errors ) const
{
  test_size(vs1.dim(), _dim);
  test_size(vs2.dim(), _dim);

  // сколько шагов классификатора выполнять
  int st_num = t_min(errors.len(), step_num());

  vector x(vs1.dim());
  errors.set_val(0.0);
  vector cl(st_num, 0.0);
  // пробегаемся по всем точкам из обучающего набора
  for( int j = 0; j < vs1.len(); j++ ){
    vs1.get(x, j);
    classify(x, cl);
    // бежим по всем шагам классификатора
    for( int k = 0; k < st_num; k++ )
      errors[k] += (cl[k] < 0) ? 0 : 1;
  }
  for( int j = 0; j < vs2.len(); j++ ){
    vs2.get(x, j);
    classify(x, cl);
    for( int k = 0; k < st_num; k++ )
      errors[k] += (cl[k] < 0) ? 1 : 0;
  }

  errors /= (vs1.len() + vs2.len());
  // если вектор бОльшей длинны чем количество шагов классификатора, то дозаполняем крайним значением
  if( errors.len() > st_num )
    for( int k = st_num; k < errors.len(); k++ ){
      errors[k] = errors[st_num-1];
    }
}

real adaboost::get_complexity( int cl, int idx ) const
{
  if( cl == 0 ){
    test_index(idx, _w1.len());
    return _w1[idx];
  }
  else if( cl == 1 ){
    test_index(idx, _w2.len());
    return _w2[idx];
  }
  else
    fail_assert("adaboost::get_weigth(): incorrect class index");

  return 0.0; // dummy
}

void adaboost::get_feature_quality( vector& q, int num, int m_idx ) const
{
  test_index(num, _step_num);
  if( num == 0 || _is_test )
    q = _qualities[_qualities.len() - 1];
  else
    q = _qualities[num];
}

real adaboost::get_confidence( const vector& x, int num ) const
{
  test_size(x.len(), _dim);

  if( num == 0 || num > step_num() )
    num = step_num();
  if( num < 1 || num > step_num() )
    runtime("adaboost::classify: incorrect number of classifiers");
  if( !_is_test )
    num = 1;

  if( classify(x, num) == 0 )
    return _p1[num-1]; // num -количество шагов классификатора
  else
    return _p2[num-1];
}

void adaboost::save_result( referer<luaconf> res, const char* root ) const
{
  // основная таблица с параметрами обученного классификатора
  res->exec("%s.adaboost = {}", root);
  // размерность вектора признака
  res->exec("%s.adaboost.dim = %d", root, _dim);
  // количество шагов классификатора
  res->exec("%s.adaboost.classifier_num = %d", root, _step_num);
  // тип используемого простого классификатора
  res->exec("%s.adaboost.simple_classifier = \"%s\"", root, _cl[0]->get_name());

  // массив alpha
  res->exec("%s.adaboost.alpha = {}", root);
  for( int i = 0; i < _step_num; i++ )
    res->exec("%s.adaboost.alpha[%d] = %.12g", root, i+1, _alpha[i]);

  // массив p1 p2 для подсчета уверенности классификации
  res->exec("%s.adaboost.p1 = {}", root);
  for( int i = 0; i < _p1.len(); i++ )
    res->exec("%s.adaboost.p1[%d] = %.12g", root, i+1, _p1[i]);

  res->exec("%s.adaboost.p2 = {}", root);
  for( int i = 0; i < _p2.len(); i++ )
    res->exec("%s.adaboost.p2[%d] = %.12g", root, i+1, _p2[i]);

  // сереализация простых классификаторов
  res->exec("%s.adaboost.simple_classifiers = {}", root);
  for( int i = 0; i < _step_num; i++ ){
    strng str = cstrng::form("%s.adaboost.simple_classifiers[%d]", root, i+1);
    _cl[i]->serialize(res, str.ascstr());
  }

  // вывод качества признаков
  // для каждого шага adaboost, выводится качество для каждого признака
  res->exec("%s.adaboost.qualities = {}", root);
  if( !_is_test ){
    res->exec("%s.adaboost.qualities[%d] = {}", root, 1);
    for( int j = 0; j < _dim; j++ )
      res->exec("%s.adaboost.qualities[%d][%d] = %.12g", root, 1, j+1, _qualities[0][j]);
  }
  else{
    for( int i = 0; i < _qualities.len(); i++ ){
      res->exec("%s.adaboost.qualities[%d] = {}", root, i+1);
      for( int j = 0; j < _dim; j++ )
        res->exec("%s.adaboost.qualities[%d][%d] = %.12g", root, i+1, j+1, _qualities[i][j]);
    }
  }
}

real adaboost::get_confidence_for_one_class( const vector& x, int num, int class_idx ) const {
  return 0;
}

void adaboost::save_result_file( const char *file_name) const
{
}


}; // namespace lwml
