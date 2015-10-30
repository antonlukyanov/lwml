// AdaBoost
// lwml, (c) ltwood

#ifndef _ADABOOST_
#define _ADABOOST_

#include "iclassifier.h"
#include "keypoints.h"

/*#build_stop*/

namespace lwml {

/**
 * Интерфейс для классификаторов семейства AdaBoost.
 */
class i_classifier : public interface {
public:
  /**@return Размерность векторов */
  virtual int dim() const = 0;
  /** @return Количество классов */
  virtual int class_num() const = 0;
  /** @return Количество шагов классификации */
  virtual int step_num() const = 0;

  /**
   * Выполнение классификации заданного вектора.
   * @param Вектор для классификации
   * @param num
   * @return Результат классификации
   */
  virtual int classify( const vector&, int num = 0 ) const = 0;

  /**
   * Оценивает качество признаков на заданной тестовой выборке.
   * @todo describe parameters
   *
   * @param q Классифицированный вектор
   * @param num
   * @param m_idx
   */
  virtual void get_feature_quality( vector& q, int num = 0, int m_idx = -1 ) const = 0;

  /**
   * Получить степень уверенности классификации.
   * @param x Классифицированный вектор
   * @param num Количество шагов классификатора
   * @return Степень уверенности
   */
  virtual real get_confidence( const vector& x, int num = 0 ) const = 0;

  /**
   * Получить степень уверенности классификации к определенному классу.
   * @param x Классифицированный вектор
   * @param num Количество шагов классификатора
   * @param class_idx - номер класса, для которого определяется степень уверенности
   * @return Степень уверенности
   */
  virtual real get_confidence_for_one_class( const vector& x, int num = 0, int class_idx = 0 ) const = 0;

  /**
   * Cохранение результатов классификации.
   * @param cnf
   * @param root
   */
  virtual void save_result( referer<luaconf> cnf, const char* root ) const = 0;

  /**
   * Cохранение результатов классификации в файл file_name
   * @param file_name Имя файла
   */
  virtual void save_result_file(const char *file_name) const = 0;

  /**
   * Calculate the errors vertor for keypoint list
   * @param kpl for this keypoint list we calculate errors
   * @param errors vector of errors for each step of classifier
   */
  virtual void calc_errors( const keypoint_list_lvset& kpl, vector& errors ) const = 0;

  // трудность классификации, для точек из обучающего набора
  //~virtual real get_complexity( int cl, int idx ) const = 0;

};

/**
 * @short Discrete AdaBoost
 */
class adaboost : public i_classifier {
public:
  /**
   * Создание классификатора по набору параметров.
   * @param sc_fact Конструктор простого классификатора
   * @param vs1 Набор векторов первого класса
   * @param vs2 Набор векторов второго класса
   * @param num
   * @param tick Параметр прогресс индикации
   */
  adaboost( const i_simple_classifier_maker& sc_fact, const i_vector_set& vs1, const i_vector_set& vs2, int num, tick_mode tick = tmOFF )
: _step_num(0), _w1(vs1.len()), _w2(vs2.len()), _alpha(num), _n_r1(num, 0), _n_w1(num, 0), _n_r2(num, 0), _n_w2(num, 0),
  _p1(num, 0.5), _p2(num, 0.5), _cl(num), _qualities(num, vector(vs1.dim(), 0.0)
  )
{
    test_size(vs1.dim(), vs2.dim());
    _dim = vs1.dim();
    mk_classifier(sc_fact, vs1, vs2, num, tick);
    calc_feature_quality(vs1, vs2);
    calc_confidences(vs1, vs2);
}
  /**
   * Создание классификатора по внешнему lua-файлу.
   * @param cnf Файл конфигурации
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
   * @param cl
   * @param idx
   * @return
   */
  virtual real get_complexity( int cl, int idx ) const;
  virtual void get_feature_quality( vector& q, int num = 0, int m_idx = -1 ) const;
  virtual real get_confidence( const vector& x, int num = 0 ) const;
  virtual real get_confidence_for_one_class( const vector& x, int num = 0, int class_idx = 0 ) const;

  virtual void save_result( referer<luaconf> res, const char* root ) const;
  virtual void save_result_file(const char *file_name) const;

  virtual void calc_errors( const keypoint_list_lvset& kpl, vector& errors ) const {};

private:
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
  vector _p1, _p2;
  t_array< refowner<i_simple_classifier> > _cl;
  /**Вектор качества распознавания*/
  t_array<vector> _qualities;

  real calc_error( const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2, const i_simple_classifier* cl ) const;
  real recalc_weights( const i_vector_set& vs1, const i_vector_set& vs2, const i_simple_classifier* cl, real error );
  void mk_classifier( const i_simple_classifier_maker& sc_fact, const i_vector_set& vs1, const i_vector_set& vs2, int num, tick_mode tick );

  void calc_feature_quality( int_vector& vnums, const vector& x, int cl );
  void calc_feature_quality( const i_vector_set& vs1, const i_vector_set& vs2 );

  // вычисляется вероятность того, что точка, которая была классифицирована как класс 0/1, действительно принадлежит ему
  void calc_confidences( const i_vector_set& vs1, const i_vector_set& vs2 );
  // для фиксированного вектора-признаков для всех шагов классификатора
  // вычисляется куда попал этот вектор-признаков и к соответствующему счетчику прибавляется 1
  void calc_part_confidence( const vector& x, int cl );
};

}; // namespace lwml

#endif // _ADABOOST_
