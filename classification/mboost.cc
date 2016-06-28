#include "mboost.h"
#include "stream.h"

/*#build_stop*/

namespace lwml {

//
// mult_adaboost
//

int mult_adaboost::calc_adabost_num( int class_num, mboost_type m_type ) const
{
  if( m_type == ONE_VS_ALL ){
    if( class_num == 2 )
      return 1;
    else
      return class_num;
  }
  if( m_type == ONE_VS_ONE )
    return class_num*(class_num - 1) / 2;
  else
    return 1;
}

mult_adaboost::mult_adaboost(
  mboost_type m_type,
  int steps_num,
  const i_simple_classifier_maker& sc_fact,
  const keypoint_list_lvset& kpl,
  int classes_num,
  const i_msg_receiver& log,
  tick_mode tick,
  bool is_test
)
: _is_test(is_test),
  _dim(kpl.dim()),
  _max_step_num(0),
  _class_num(classes_num),
  _adaboost_num(calc_adabost_num(classes_num, m_type)),
  _coding_matrix(_class_num, _adaboost_num),
  _m_ab(_adaboost_num),
  _probabilities_of_classes(classes_num, matrix(_adaboost_num, is_test ? 1 : steps_num, 0.0)),
  _cl_probability(classes_num, 0.0)
{
  if( m_type == ONE_VS_ALL )
    gen_one_vs_all(_coding_matrix);
  if( m_type == ONE_VS_ONE )
    gen_one_vs_one(_coding_matrix);
  if( m_type == RAND_HALF )
    gen_rand_half(_coding_matrix);

  if( zzz_dump() )
    vbmp::save(zzz_dump_name("coding_matrix.bmp").ascstr(), _coding_matrix);

  for( int i = 0; i < _adaboost_num; i++ ){
    log.put_msg(strng::form("step=%d/%d\n", i+1, _adaboost_num).ascstr());
    int_vector v_cl(_class_num);
    _coding_matrix.get_col(v_cl, i);
    mkeypoint_vset vsets1(&kpl, v_cl, -1);
    mkeypoint_vset vsets2(&kpl, v_cl, 1);
    _m_ab[i] = new adaboost(sc_fact, vsets1, vsets2, steps_num, tick, _is_test);
    if( _m_ab[i]->step_num() > _max_step_num )
      _max_step_num = _m_ab[i]->step_num();
  }
  // считаем статистику для уверенности классификации
  calc_confidence(kpl);
  dump_probabilities(_max_step_num);
}

mult_adaboost::mult_adaboost( referer<luaconf> cnf, const char* root )
: _is_test(cnf->get_bool("%s.is_test", root)),
  _max_step_num(cnf->get_int("%s.max_step_num", root)), _class_num(cnf->get_int("%s.class_num", root)),
  _adaboost_num(cnf->get_int("%s.adaboost_num", root)),
  _coding_matrix(_class_num, _adaboost_num), _m_ab(_adaboost_num),
  _probabilities_of_classes(_class_num, matrix(_adaboost_num, _is_test ? 1 : _max_step_num, 0.0)), _cl_probability(_class_num, 0.0)
{
  _dim = cnf->get_int("%s.dim", root);
  for( int i = 0; i < _class_num; i++ ){
    for( int j = 0; j < _adaboost_num; j++ ){
      _coding_matrix(i, j) = cnf->get_int("%s.coding_matrix[%d][%d]", root, i+1, j+1);
    }
  }

  for( int i = 0; i < _adaboost_num; i++ ){
    strng str = strng::form("%s.adaboosts[%d].adaboost", root, i+1);
    _m_ab[i] = new adaboost(cnf, str.ascstr());
  }
  // чтение данных для вычисления степени уверенности классификации
  for( int cl_idx = 0; cl_idx < _class_num; cl_idx++ ){
    _cl_probability[cl_idx] = cnf->get_real("%s.confidence_inform.class_probability[%d]", root, cl_idx+1);
    strng str = cstrng::form("%s.confidence_inform.answer_probability[%d]", root, cl_idx+1);
    for( int ab_idx = 0; ab_idx < _m_ab.len(); ab_idx++ ){
      int len = _probabilities_of_classes[cl_idx].col();
      for( int step = 0; step < len; step++ )
        _probabilities_of_classes[cl_idx](ab_idx, step) = cnf->get_real("%s[%d][%d]", str.ascstr(), ab_idx+1, step);
    }
  }

  for( int cl_idx = 0; cl_idx < _class_num; cl_idx++ ){
    zzz("cl_idx=%d class_probabiliti=%f", cl_idx, _cl_probability[cl_idx]);
    for( int ab_idx = 0; ab_idx < _m_ab.len(); ab_idx++ ){
      zzz("cl_idx=%d ab_idx=%d num=%f", cl_idx, ab_idx, _probabilities_of_classes[cl_idx](ab_idx, _probabilities_of_classes[cl_idx].col()-1));
    }
  }
}

int mult_adaboost::classify( const vector& x, int num ) const
{
  test_size(x.len(), _dim);

  if( num == 0 || num > step_num() )
    num = step_num();
  if( num < 1 )
    runtime("mult_adaboost::classify: incorrect number of classifiers");

  real confidence;
  return calc_best_class(x, num, &confidence);
}

void mult_adaboost::get_feature_quality( vector& q, int num, int m_idx ) const
{
  q.set_val(0.0);
  referer<stream> quality_file;

  // calculate quality for simple Adaboost with index m_idx
  if( m_idx != -1 ){
    if( m_idx < _m_ab.len() )
      _m_ab[m_idx]->get_feature_quality(q, num);
    return;
  }

  // calculate quality for multiboost as maximum of simple Adaboosts
  // and print to dump information about all simple Adaboosts
  for( int i = 0; i < _m_ab.len(); i++ ){
    if( zzz_dump() )
      quality_file = stream::create(zzz_dump_name("features_quality_%d.dat", i).ascstr(), fmWRITE, fmTEXT);
    vector quality(_dim);
    _m_ab[i]->get_feature_quality(quality, num);
    for( int k = 0; k < quality.len(); k++ ){
      if( zzz_dump() )
        quality_file->printf("%d %f\n", k, quality[k]);
      if( q[k] < quality[k] )
        q[k] = quality[k];
    }
  }
}

real mult_adaboost::get_confidence( const vector& x, int num ) const
{
  real confidence;
  calc_best_class(x, num, &confidence);
  return confidence;
}
real mult_adaboost::get_confidence_for_one_class(const vector& x, int num, int class_idx) const
{
  // It hasn't realized yet
  return 0;
}

void mult_adaboost::save_result( referer<luaconf> res, const char* root ) const
{
  // основная таблица с параметрами обученного классификатора root
  if( _is_test )
    res->exec("%s.is_test = true", root);
  else
    res->exec("%s.is_test = false", root);

  // количество классов
  res->exec("%s.class_num = %d", root, _class_num);
  // количество итераций (столбцов в матрице)
  res->exec("%s.adaboost_num = %d", root, _adaboost_num);
  // размерность вектора признака
  res->exec("%s.dim = %d", root, _dim);
  // максимальное количество шагов классификатора
  res->exec("%s.max_step_num = %d", root, _max_step_num);

  // вывод coding_matrix
  res->exec("%s.coding_matrix = {}", root);
  for( int i = 0; i < _class_num; i++ ){
    res->exec("%s.coding_matrix[%d] = {}", root, i+1);
    for( int j = 0; j < _adaboost_num; j++ ){
      res->exec("%s.coding_matrix[%d][%d] = %d", root, i+1, j+1, _coding_matrix(i, j));
    }
  }

  // сереализация классификаторов на каждой итерации
  res->exec("%s.adaboosts = {}", root);
  for( int i = 0; i < _adaboost_num; i++ ){
    strng str = cstrng::form("%s.adaboosts[%d]", root, i+1);
    res->exec("%s = {}", str.ascstr());
    _m_ab[i]->save_result(res, str.ascstr());
  }
  // сохранение данных для вычисления степени уверенности классификации
  res->exec("%s.confidence_inform = {}", root);
  res->exec("%s.confidence_inform.answer_probability = {}", root);
  for( int cl_idx = 0; cl_idx < _class_num; cl_idx++ ){
    strng str = cstrng::form("%s.confidence_inform.answer_probability[%d]", root, cl_idx+1);
    res->exec("%s = {}", str.ascstr());
    for( int ab_idx = 0; ab_idx < _m_ab.len(); ab_idx++ ){
      res->exec("%s[%d] = {}", str.ascstr(), ab_idx+1);
      if( _is_test )
        res->exec("%s[%d][%d] = %f", str.ascstr(), ab_idx+1, 0, _probabilities_of_classes[cl_idx](ab_idx, 0));
      else{
        for( int step = 0; step < _max_step_num; step++ ){
          res->exec("%s[%d][%d] = %f", str.ascstr(), ab_idx+1, step, _probabilities_of_classes[cl_idx](ab_idx, step));
        }
      }
    }
  }
  res->exec("%s.confidence_inform.class_probability = {}", root);
  for( int cl_idx = 0; cl_idx < _class_num; cl_idx++ ){
    res->exec("%s.confidence_inform.class_probability[%d] = %f", root, cl_idx+1, _cl_probability[cl_idx]);
  }

}

real mult_adaboost::calc_error( const keypoint_list_lvset& kpl, int num ) const
{
  real sum_err = 0;
  for( int i = 0; i < _m_ab.len(); i++ ){
    int_vector v_cl(_class_num);
    _coding_matrix.get_col(v_cl, i);
    mkeypoint_vset vsets1(&kpl, v_cl, -1);
    mkeypoint_vset vsets2(&kpl, v_cl, 1);
    sum_err += _m_ab[i]->calc_error(vsets1, vsets2, num);
  }
  sum_err/= _m_ab.len();
  return sum_err;
}

void mult_adaboost::calc_errors( const keypoint_list_lvset& kpl, vector& errors ) const
{
  vector ab_errors(errors.len());
  errors.set_val(0.0);

  // calculate errors for multiboost as maximum of simple Adaboosts
  // and print to dump information about all simple Adaboosts
  referer<stream> errors_file;
  for( int i = 0; i < _m_ab.len(); i++ ){
    if( zzz_dump() )
      errors_file = stream::create(zzz_dump_name("errors_%d.dat", i).ascstr(), fmWRITE, fmTEXT);

    int_vector v_cl(_class_num);
    _coding_matrix.get_col(v_cl, i);
    mkeypoint_vset vsets1(&kpl, v_cl, -1);
    mkeypoint_vset vsets2(&kpl, v_cl, 1);
    _m_ab[i]->calc_errors(vsets1, vsets2, ab_errors);

    for( int k = 0; k < ab_errors.len(); k++ ){
      if( zzz_dump() )
        errors_file->printf("%d %f\n", k, ab_errors[k]);
      if( errors[k] < ab_errors[k] )
        errors[k] = ab_errors[k];
    }
  }
}

void mult_adaboost::calc_confidence( const keypoint_list_lvset& kpl )
{
  int dim = kpl.dim(); // размерность вектора-признаков
  vector x(dim);
  for( int j = 0; j < kpl.len(); j++ ){
    int cl_idx = kpl.loading(j);
    // увеличиваем счетчик количества точек попавших в этот класс
    _cl_probability[cl_idx]++;

    // берем вектор-признаков
    kpl.get(x, j);
    // бежим по всем adaboost-ам, считаем на скольких точках каждыий из нах сказал 0
    for( int ab_idx = 0; ab_idx < _m_ab.len(); ab_idx++ ){
      vector cl_value(_probabilities_of_classes[cl_idx].col(), 0.0);
      _m_ab[ab_idx]->classify(x, cl_value);

      for( int step = 0; step < cl_value.len(); step++ ){
        if( cl_value[step] < 0 )
          _probabilities_of_classes[cl_idx](ab_idx, step)++;
      }
    }
  }

  // вычисление вероятности
  for( int cl_idx = 0; cl_idx < _cl_probability.len(); cl_idx++ ){
    for( int ab_idx = 0; ab_idx < _m_ab.len(); ab_idx++ ){
      int len = _probabilities_of_classes[cl_idx].col();
      for( int step = 0; step < len; step++ ){
        if( _cl_probability[cl_idx] != 0 )  
          _probabilities_of_classes[cl_idx](ab_idx, step) /= _cl_probability[cl_idx];
        else
          _probabilities_of_classes[cl_idx](ab_idx, step) = 0.0;  
      }
      zzz("cl_idx=%d ab_idx=%d num=%f", cl_idx, ab_idx, _probabilities_of_classes[cl_idx](ab_idx, len-1));
    }
    _cl_probability[cl_idx] /= kpl.len();
    zzz("cl_idx=%d cl_probability=%f", cl_idx, _cl_probability[cl_idx]);
  }
}

int mult_adaboost::calc_best_class( const vector x, int num, real* confidence ) const
{
  if( num == 0 || num > step_num() )
    num = step_num();
  if( num < 1 )
    runtime("adaboost::classify: incorrect number of classifiers");

  int_vector ab_answer(_adaboost_num);
  // вычисляем ответы adaboost-ов
  for( int t = 0; t < _adaboost_num; t++ ){
    ab_answer[t] = _m_ab[t]->classify(x, num);
  }

  vector conditional_prob(_class_num, 1.0);
  calc_conditional_prob(num, ab_answer, conditional_prob);

  int best_cl = 0;
  for( int cl_idx = 1; cl_idx < conditional_prob.len(); cl_idx++ ){
    if( conditional_prob[cl_idx] > conditional_prob[best_cl] )
      best_cl = cl_idx;
  }
  *confidence = conditional_prob[best_cl];
  return best_cl;
}

real mult_adaboost::calc_conditional_prob( int num, const int_vector& ab_answer, vector& conditional_prob ) const
{
  if( !_is_test && num != step_num() )
    zzz("mult_adaboost::calc_conditional_prob incorrect STEP NUM %d", num);
  if( _is_test )
    num = 1;

  test_size(ab_answer.len(), _adaboost_num);
  test_size(conditional_prob.len(), _class_num);
  real sum_err = 0.0;
  conditional_prob.set_val(1.0);
  for( int cl_idx = 0; cl_idx < _class_num; cl_idx++ ){
    for( int ab_idx = 0; ab_idx < _adaboost_num; ab_idx++ ){
      if( ab_answer[ab_idx] == 0 )
        conditional_prob[cl_idx] *= _probabilities_of_classes[cl_idx](ab_idx, num-1); // -1 т.к. num-количество шагов классификатора
      else
        conditional_prob[cl_idx] *= 1 - _probabilities_of_classes[cl_idx](ab_idx, num-1);
    }
    conditional_prob[cl_idx] *= _cl_probability[cl_idx];
    sum_err += conditional_prob[cl_idx];
  }
  return sum_err;
}

void mult_adaboost::dump_probabilities( int num )
{
  if( !zzz_dump() )
    return;

  if( num == 0 || num > step_num() )
    num = step_num();
  if( _is_test )
    num = 1;
  if( num < 1 )
    runtime("adaboost::classify: incorrect number of classifiers");

  referer<stream> res = stream::create(zzz_dump_name("prob_table.txt").ascstr(), fmWRITE, fmTEXT);
  int_vector ab_answer(_adaboost_num);
  for( int i = 0; i < intpow(2, _adaboost_num); i++ ){
    res->printf("ab_answer=");
    for( int t = 0; t < _adaboost_num; t++ ){
      ab_answer[t] = ( i & (1 << t) ) != 0;
      res->printf("%d", ab_answer[t]);
    }

    vector conditional_prob(_class_num);
    real sum_err = calc_conditional_prob(num, ab_answer, conditional_prob);

    res->printf(" p_answer=%f ", sum_err);
    for( int k = 0; k < _class_num; k++ ){
      if( sum_err != 0 )
        res->printf("p_%d=%f ", k, conditional_prob[k] / sum_err);
      else
        res->printf("p_%d=%f ", k, 0.0);
    }
    res->printf("\n");
  }
}

void mult_adaboost::save_result_file( const char *file_name) const
{
}

}; // namespace lwml
