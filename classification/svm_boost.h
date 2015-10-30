#ifndef _SVMBOOST_
#define _SVMBOOST_

#include "adaboost.h"
#include "mboost.h"
#include "vector.h"
#include "defs.h"
#include "svm.h"

namespace lwml {

class svm: public i_classifier {
private:
  struct scaling_params;
public:
  /**
   * To train a classifier for a given sample.
   * @param kpl - training sample.
   * @param train_immediately - if true - run training immediately without \
   * configuration, else start trainig only after start_train() function called.
   */
  svm(const keypoint_list_lvset& kpl, bool train_immediately = true);
  /**
   * Load model in libsvm's format (Загрузить модель из текстовых файлов в формате чтения libsvm)
   * (http://www.csie.ntu.edu.tw/~cjlin/libsvm/)
   *
   * file_name_prefix+"_model.txt" - name of file which contains model data.
   * file_name_prefix+"_range.txt" - name of file which contains scaling params (out of svm-scale command).
   */
  svm(const char* file_name_prefix = "");
  // Load model from luaconf (Загрузить модель из файла конфигурации в формате luaconf)
  svm(referer<luaconf> cnf, const char* root);
  ~svm();

  /**
   * Start train svm on current parameters.
   * Should be called only in the design of the training set
   * and train_immediately = false,
   * else do not need to call.
   */
  void start_train();

  virtual int dim() const;
  virtual int class_num() const;
  virtual int step_num() const;

  /**
   * Классифицировать вектор признаков (получить класс)
   */
  virtual int classify(const vector&, int num = 0) const;

  // трудность классификации, для точек из обучающего набора
  //~virtual real get_complexity( int cl, int idx ) const = 0;

  /**
   * Оценивает качество признаков на заданной тестовой выборке.
   */
  virtual void get_feature_quality(vector& q, int num = 0, int m_idx = -1) const;

  /**
   * Get confidence of classification
   */
  virtual real get_confidence(const vector& x, int num = 0) const;

  virtual real get_confidence_for_one_class(const vector& x, int num = 0, int class_idx = 0) const;

  /**
   * Сохранение обученного классификатора в формате luaconf
   */
  virtual void save_result(referer<luaconf> cnf, const char* root) const;
  /**
   * Сохранение обученного классификатора в формате libsvm
   */
  virtual void save_result_file(const char *file_name_prefix = "") const;

  virtual void calc_errors( const keypoint_list_lvset& kpl, vector& errors ) const
  {};

  // Set type of svm:

  /**
   * Set type of SVM as C-SVC
   *
   * nr_weight, weight_label, and weight are used to change the penalty
   * for some classes (If the weight for a class is not changed, it is
   * set to 1). This is useful for training classifier using unbalanced
   * input data or with asymmetric misclassification cost.
   * @param C is the cost of constraints violation.
   * @param nr_weight
   * @param weight_label
   * @param weight
   */
  void set_svm_type_c_svc(double C = 1, int nr_weight = 0, int *weight_label = NULL, double* weight = NULL);
  void set_svm_type_nu_svc(double nu = 0.5);

  /**
   * SVM for regression task
   */
  void set_svm_type_one_class(double nu = 0.5);

  /**
   * @param C is the cost of constraints violation.
   * @param p is the epsilon in epsilon-insensitive loss function
   * of epsilon-SVM regression.
   */
  void set_svm_type_epsilon_svr(double C = 1, double p = 0.1);
  /**
   * SVM for regression task
   */
  void set_svm_type_nu_svr(double C = 1, double nu = 0.5);

  // Set kernel
  /**
   * u'*v
   */
  void set_kernel_linear();
  /**
   * (gamma*u'*v + coef0)^degree
   * @param degree
   * @param gamma
   * @param coef0
   */
  void set_kernel_poly(int degree = 3, double gamma = 0.5, double coef0 = 0);
  /**
   * exp(-gamma*|u-v|^2)
   * @param gamma
   */
  void set_kernel_rbf(double gamma = 0.5);
  /**
   * tanh(gamma*u'*v + coef0)
   * @param gamma
   * @param coef0
   */
  void set_kernel_sigmoid(double gamma = 0.5, double coef0 = 0);

  // Set some other parameters

  /**
   * @param eps is the stopping criterion. (we usually use 0.00001 in nu-SVC,
   * 0.001 in others).
   */
  void set_eps(double eps = 1e-3);
  /**
   * @param cache_size is the size of the kernel cache, specified in megabytes.
   */
  void set_cache_size(double cache_size = 300);

  /**
   * @param shrinking - shrinking = 1 means shrinking is conducted;
   * = 0 otherwise.
   */
  void set_shrinking(int shrinking = 1);
  /**
   * @param probability - probability = 1 means model with probability
   * information is obtained; = 0 otherwise.
   */
  void set_probability(int probability = 1);

private:
  /**
   * Модель SVM (support vectors, kernel, etc.)
   */
  svm_model *_model;
  /**
   * Параметры-настройки для обучения (параметры ядра и пр.)s
   */
  svm_parameter *_params;
  /**
   * Размерность вектора признаков
   */
  int _dim;
  /**
   * Обучающая выборка
   */
  svm_problem *_svm_problem;

  /**
   * Значения для масштабирования(нормализации) параметров
   */
  scaling_params* _scaling_params;
  /**
   * Нижняя и верхняя граница для параметров при маштабировании.
   */
  real _lower, _upper;
  /**
   * Get default svm parameters (for training).
   * num_features - num of features in training set.
   */
  static svm_parameter* get_default_params(int num_features = 2);
  /**
   * Вычисление _scaling_params по обучающей выборке
   */
  void init_scaling_params();
  /**
   * Освобождение памяти для _scaling_params
   */
  void free_scaling_params();
  /**
   * Нормализовать вектор, используя _scaling_params
   */
  void scale_vector(vector& vec) const;
  /**
   * Нормализовать обучающую выборку
   */
  void scale_svm_problem();
  /**
   * Нормализовать признак
   * @param feature_value - изначальное значение пизнака
   * @param ind - номер признака в векторе признаков
   */
  real scale_feature(real feature_value, int ind) const;
  /**
   * Загрузить параметры нормализации из файла в формате libsvm
   */
  void restore_scaling_params(const char* fname);
  /**
   * Сохранить параметры нормализации в файл в формате libsvm
   */
  void save_scaling_params(const char* fname) const;
  /**
   * Структура для хранения параметров нормализации для одного признака
   */
  struct scaling_params {
    real min_value;
    real max_value;
  };

  /**
   * Нужно ли обучаться (защита от повторного обучения)
   */
  bool _need_learn;

};
} // endo

#endif // _SVMBOOST_
