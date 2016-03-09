// AdaBoost
// lwml, (c) ltwood

#ifndef _ADABOOST_
#define _ADABOOST_

#include "iclassifier.h"
#include "keypoints.h"

/*#build_stop*/

namespace lwml {

/**
 * @short Discrete AdaBoost
 */
class adaboost : public i_classifier {
public:
  /**
   * Создание классификатора по набору параметров.
   *
   * @param sc_fact
   *   Фабрика простых классификаторов.
   * @param vs1
   *   Набор векторов первого класса.
   * @param vs2
   *   Набор векторов второго класса.
   * @param num
   *   Число шагов обучения.
   * @param tick
   *   Параметр прогресс индикации.
   */
  adaboost(
    const i_simple_classifier_maker& sc_fact,
    const i_vector_set& vs1, const i_vector_set& vs2,
    int num, tick_mode tick = tmOFF, bool is_test = false
  ) : _is_test(is_test), _step_num(0), _w1(vs1.len()), _w2(vs2.len()), _alpha(num),
      _n_r1(num, 0), _n_w1(num, 0),
      _n_r2(num, 0), _n_w2(num, 0), _p1( _is_test ? 1: num, 0.5), _p2( _is_test ? 1: num, 0.5),
      _cl(num), _qualities(num, vector(vs1.dim(), 0.0))
  {
    test_size(vs1.dim(), vs2.dim());
    _dim = vs1.dim();
    mk_classifier(sc_fact, vs1, vs2, num, tick);
    calc_feature_quality(vs1, vs2);
    calc_confidences(vs1, vs2);
  }

  /**
   * Восстановление обученного классификатора по внешнему lua-файлу.
   *
   * @param cnf
   *   Файл конфигурации.
   * @param root
   */
  adaboost( referer<luaconf> cnf, const char* root );

  virtual int dim() const { return _dim; }
  virtual int class_num() const { return 2; }
  virtual int step_num() const { return _step_num; }

  virtual int classify( const vector&, int num = 0 ) const;
  // для заданного вектора-признаков строит вектор -
  // взвешенная сумма классификаторов на каждом шаге обучения
  // если длина вектора больше, то дозаполняет крайним значением
  void classify( const vector&, vector& cl_value ) const;

  // функции calc_error(s) могут вызываться для разных наборов точек, не обязательно для тех на которых обучились
  real calc_error( const i_vector_set& vs1, const i_vector_set& vs2, int num = 0 ) const;
  void calc_errors( const i_vector_set& vs1, const i_vector_set& vs2, vector& errors ) const;

  /**
   * Трудность классификации, для точек из обучающего набора
   * @param cl Номер класса
   * @param idx Индекс точки в обучающем наборе
   * @return
   */
  virtual real get_complexity( int cl, int idx ) const;

  /**
   * Получить качетво признаков при классификации
   * @param q Посчитанный вектор качества признаков
   * @param num  Количество шагов классификатора
   * @param m_idx Номер классификатора (нужно для многоклассовой классификации)
   * @return
   */
  virtual void get_feature_quality( vector& q, int num = 0, int m_idx = -1 ) const;
  virtual real get_confidence( const vector& x, int num = 0 ) const;
  virtual real get_confidence_for_one_class( const vector& x, int num = 0, int class_idx = 0 ) const;

  virtual void save_result( referer<luaconf> res, const char* root ) const;
  virtual void save_result_file(const char *file_name) const;

  virtual void calc_errors( const keypoint_list_lvset& kpl, vector& errors ) const {};

private:
  /** Версия для тестирования или нет*/
  bool _is_test;
  /**Размерность векторов*/
  int _dim;
  /**Количество шагов*/
  int _step_num;
  double _eps;
  /**Веса для векторов первого класса*/
  vector _w1;
  /**Веса для векторов второго класса*/
  vector _w2;
  vector _alpha;

  // количесво точек попавших правильно(r)/неправильно(w) в 1 и 2й классы
  /**Количество точек, правильно попавших в 1-ый класс*/
  int_vector _n_r1;
  /**Количество точек, неправильно попавших в 1-ый класс*/
  int_vector _n_w1;
  /**Количество точек, правильно попавших во 2-ой класс*/
  int_vector _n_r2;
  /**Количество точек, неправильно попавших во 2-ой класс*/
  int_vector _n_w2;
  /** Условные вероятности того, что точка, которую мы
  классифицировали в j-й класс действительно относится к нему*/
  vector _p1, _p2;
  /** Массив простых классификаторов*/
  t_array< refowner<i_simple_classifier> > _cl;
  /**Вектор качества распознавания*/
  t_array<vector> _qualities;

  real calc_error(
    const i_vector_set& vs1, const vector& w1,
    const i_vector_set& vs2, const vector& w2,
    const i_simple_classifier* cl
  ) const;

  real recalc_weights(
    const i_vector_set& vs1,
    const i_vector_set& vs2,
    const i_simple_classifier* cl,
    real error
  );

  void mk_classifier(
    const i_simple_classifier_maker& sc_fact,
    const i_vector_set& vs1, const i_vector_set& vs2,
    int num, tick_mode tick
  );

  void calc_feature_quality( int_vector& vnums, const vector& x );
  void calc_feature_quality( const i_vector_set& vs1, const i_vector_set& vs2 );

  // вычисляется вероятность того, что точка, которая была классифицирована как класс 0/1,
  // действительно принадлежит ему
  void calc_confidences( const i_vector_set& vs1, const i_vector_set& vs2 );
  // для фиксированного вектора-признаков для всех шагов классификатора
  // вычисляется куда попал этот вектор-признаков и к соответствующему счетчику прибавляется 1
  void calc_part_confidence( const vector& x, int cl );
};

}; // namespace lwml

#endif // _ADABOOST_
