#ifndef _ICLASSIFIER_H_
#define _ICLASSIFIER_H_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/lua/luaconfig.h"

#include "lwml/m_base/i_vset.h"
#include "lwml/m_types/vector.h"
#include "lwml/types/t_array.h"
#include "lwml/base/refowner.h"
#include "lwml/alg/t_sorter.h"

#include "lwml/classification/keypoints.h"

namespace lwml {

/**
 * Интерфейс для классификаторов.
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
   * @param num Количество шагов классификатора (нужно AdaBoost)
   * @return Результат классификации
   */
  virtual int classify( const vector&, int num = 0 ) const = 0;

  /**
   * Оценивает качество признаков на тестовой выборке.
   *
   * @param q Вектор с качеством признаков
   * @param num Количество шагов классификатора (нужно AdaBoost)
   * @param m_idx Номер классификатора (нужно для многоклассовой классификации)
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
   * @param num Количество шагов классификатора (нужно AdaBoost)
   * @param class_idx - номер класса, для которого определяется степень уверенности
   * @return Степень уверенности
   */
  virtual real get_confidence_for_one_class( const vector& x, int num = 0, int class_idx = 0 ) const = 0;

  /**
   * Cохранение результатов классификации.
   * @param cnf Указатель на luaconf, куда созранять результаты
   * @param root Имя таблицы, в которую будут сохнанены результаты
   */
  virtual void save_result( referer<luaconf> cnf, const char* root ) const = 0;

  /**
   * Cохранение результатов классификации в файл file_name
   * @param file_name Имя файла
   */
  virtual void save_result_file(const char *file_name) const = 0;

  /**
   * Посчиталь ошибки для списка точек
   * @param kpl Список точек (keypoint) для которых будем считать ошибки
   * @param errors Вектор посчитннных ошибок
   */
  virtual void calc_errors( const keypoint_list_lvset& kpl, vector& errors ) const = 0;

  // трудность классификации, для точек из обучающего набора
  //~virtual real get_complexity( int cl, int idx ) const = 0;

};

//
// Интерфейсы для простых классификаторов и для их создания.
//

/**
 * Интерфейс простого классификатора (стратегия).
 */
class i_simple_classifier : public interface {
public:
  /**
   * Выполнение классификации заданного вектора x.
   * @param x Вектор для классификации
   * @return Возвращает результат классификации - значение 0 или 1
   */
  virtual int classify( const vector& x ) const = 0;

  /**
   * Заполняет вектор значениями весов координат классифицируемого вектора.
   * Интерпретация весов зависит от используемой реализации простого классификатора.
   * @param w Вектор для заполнения.
   */
  virtual void get_weights( vector& w ) const = 0;

  /**
   * Тип классификатора.
   * @return Тип классификатора.
   */
  virtual const char* get_name() const = 0;

  /** @todo описать параметры
   *
   * Запись в файл параметров класификатора в виде lua таблицы.
   * @param cnf Файл конфигурации
   * @param root
   */
  virtual void serialize( referer<luaconf> cnf, const char* root ) const = 0;
};

/**
 * Интерфейс виртуального конструктора простых классификаторов.
 */
class i_simple_classifier_maker : public interface {
public:
  /**
   *  Создание классификатора по двум заданным наборам векторов с весами.
   * @param vs1 Набор векторов первого класса
   * @param w1 Веса для векторов первого класса
   * @param vs2 Набор векторов второго класса
   * @param w2 Веса для векторов второго класса
   * @return
   */
  virtual i_simple_classifier* create( const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2 ) const = 0;

  /**
   *
   * Создание классификатора по внешнему файлу.
   * @param dim Размерность вектора признаков
   * @param cnf Файл конфигурации
   * @param root Имя таблицы, где лежат данные классификатора
   * @return
   */
  virtual i_simple_classifier* create( int dim, referer<luaconf> cnf, const char* root ) const = 0;
};


/**
 * Простой классификатор на основе оптимального разделения одномерных проекций.
 * @note Сейчас нигде не используется.
 */
class project_classifier : public i_simple_classifier {
public:
  /**
    * Создание классификатора по двум заданным наборам векторов с весами.
    * @param vs1 Набор векторов первого класса
    * @param w1 Веса для векторов первого класса
    * @param vs2 Набор векторов второго класса
    * @param w2 Веса для векторов второго класса
    * @return
    */
  project_classifier( const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2 );

  /**
   * Создание классификатора по внешнему lua-файлу.
   * @param dim Размерность векторов
   * @param cnf Файл конфигурации
   * @param root Имя таблицы, где лежат данные классификатора
   */
  project_classifier( int dim, referer<luaconf> cnf, const char* root );

  virtual int classify( const vector& x ) const;

  virtual void get_weights( vector& w ) const;

  virtual const char* get_name() const;

  virtual void serialize( referer<luaconf> cnf, const char* root ) const;

private:
  vector _dir;  /**< Размерность векторов, количество направлений для проекций   */
  real _thr;
  bool _is_invers;

  void set_mid( vector& mid, const i_vector_set& vs, const vector& w ) const;
};


/**
 * Конструктор простых классификаторов ( классификаторов на основе оптимального разделения одномерных проекций).
 */
class project_classifier_maker : public i_simple_classifier_maker {
  virtual i_simple_classifier* create( const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2 ) const
  {
    return new(lwml_alloc) project_classifier(vs1, w1, vs2, w2);
  }

  virtual i_simple_classifier* create( int dim, referer<luaconf> cnf, const char* root ) const
  {
    return new(lwml_alloc) project_classifier(dim, cnf, root);
  }
};

/**
 * Простой классификатор на основе оптимального выбора одномерного координатного классификатора.
 * @note Сейчас нигде не используется.
 */
class coord_classifier : public i_simple_classifier {
public:
  /**
    * Создание классификатора по двум заданным наборам векторов с весами.
    * @param vs1 Набор векторов первого класса
    * @param w1 Веса для векторов первого класса
    * @param vs2 Набор векторов второго класса
    * @param w2 Веса для векторов второго класса
    * @return
    */
  coord_classifier( const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2 );

  /**
   * Создание классификатора по внешнему lua-файлу.
   * @param dim Размерность векторов
   * @param cnf Файл конфигурации
   * @param root Имя таблицы, где лежат данные классификатора
   */
  coord_classifier( int dim, referer<luaconf> cnf, const char* root );

  virtual int classify( const vector& x ) const {
    test_size(x.len(), _dim);
    return _is_invers ^ (x[_cidx] > _thr);
  }

  virtual void get_weights( vector& w ) const;

  virtual const char* get_name() const;

  virtual void serialize( referer<luaconf> cnf, const char* root ) const;

private:
  int _dim; /**< Размерность векторов */
  int _cidx; /**< Номер координаты */
  real _thr;
  bool _is_invers;

  real calc_error(
    const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2, int cidx, real* thr, bool *is_invers,
    vector& buf, t_sorter<vector>& sort, vector& sum1, vector& sum2
  ) const;
};


/**
 * Конструктор простых классификаторов на основе оптимального выбора одномерного координатного классификатора.
 */
class coord_classifier_maker : public i_simple_classifier_maker {
  virtual i_simple_classifier* create( const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2 ) const
  {
    return new(lwml_alloc) coord_classifier(vs1, w1, vs2, w2);
  }

  virtual i_simple_classifier* create( int dim, referer<luaconf> cnf, const char* root ) const
  {
    return new(lwml_alloc) coord_classifier(dim, cnf, root);
  }

};

} /* namespace lwml */

#endif /* _ICLASSIFIER_H_ */
