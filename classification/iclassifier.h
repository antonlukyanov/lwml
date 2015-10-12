#ifndef _ICLASSIFIER_H_
#define _ICLASSIFIER_H_

#include "defs.h"
#include "mdefs.h"
#include "luaconfig.h"

#include "i_vset.h"
#include "vector.h"
#include "t_array.h"
#include "refowner.h"
#include "t_sorter.h"

namespace lwml {

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

  /**  @todo описать параметры
   *
   * Создание классификатора по внешнему файлу.
   * @param dim
   * @param cnf Файл конфигурации
   * @param root
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
   * @param dim размерность векторов
   * @param cnf Файл конфигурации
   * @param root
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
   * @param dim
   * @param cnf Файл конфигурации
   * @param root
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
  int _dim; /**<Размерность векторов*/
  int _cidx; /**< Номер координаты*/
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
