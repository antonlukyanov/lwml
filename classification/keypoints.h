// List of keypoints.
// lwml, (c) ltwood

#ifndef _KEYPOINTS_
#define _KEYPOINTS_

#include "defs.h"
#include "mdefs.h"
#include "i_vset.h"

#include "igeom.h"
#include "t_list.h"
#include "ivector.h"
#include "vector.h"
#include "randcomb.h"

/*#build_stop*/

namespace lwml {

// Контейнер ключевых точек картинок и их дескрипторов.

class keypoint_list : public value {
public:
  void put( const int_point& p, const vector& d, int id = -1, int cl = -1 ){
    _pt_list.put(p);
    _dsc_list.put(d);
    _id_list.put(id);
    _class_list.put(cl);
  }

  void put( const int_point& p, int id = -1, int cl = -1 ){
    _pt_list.put(p);
    _dsc_list.put(vector());
    _id_list.put(id);
    _class_list.put(cl);
  }

  int len() const { return _pt_list.len(); }
  const int_point& get_point( int idx ) const { return _pt_list[idx]; }
  const vector& get_descr( int idx ) const { return _dsc_list[idx]; }
  int get_id( int idx ) const { return _id_list[idx]; }
  int get_class( int idx ) const { return _class_list[idx]; }

  void del( int j ){
    _pt_list.del(j);
    _dsc_list.del(j);
    _id_list.del(j);
    _class_list.del(j);
  }

  void add_descr( int j, const vector d ){
    vector& v = _dsc_list[j];
    int len0 = v.len();
    v.resize(len0 + d.len());
    for( int j = 0; j < d.len(); j++ )
      v[len0 + j] = d[j];
  }

private:
  t_list<int_point> _pt_list;         // список ключевых точек
  t_list<vector> _dsc_list;           // список дескрипторов
  t_list<int> _id_list, _class_list;  // списки id и классов точек
};

// Интерфейс объекта, хранящего нагруженный набор векторов.
// Обычно используется в алгоритмах кластеризации.
//
// Потомки переопределяют следующие виртуальные функции:
//
//   len() - возвращает число векторов
//   dim() - возвращает размерность векторов
//   get(j) - возвращает d-ю компоненту вектора с номером j
//   get( vector&, j ) - помещает в переданный хранимый вектор с номером j
//
//   ldim() - возвращает число значений нагружения
//   loading(j) - возвращает нагружение вектора с номером j
// Контракт: нагружение принимает значения из диапазона [0, ldim()-1].

class i_loaded_vector_set : public interface {
public:
  virtual int len() const = 0;
  virtual int dim() const = 0;
  virtual real get( int j, int d ) const = 0;
  virtual void get( vector&, int j ) const = 0;

  virtual int ldim() const = 0;
  virtual int loading( int j ) const = 0;

  real operator()( int j, int d ) const { return get(j, d); }
};

// Реализация интерфейса i_loaded_vector_set поверх keypoint_list.

class keypoint_list_lvset : public i_loaded_vector_set {
public:
  keypoint_list_lvset( const keypoint_list* kpl ) : _kpl(kpl) {
    for( int j = 0; j < kpl->len(); j++ ){
      int dim = kpl->get_descr(j).len();
      if( j == 0 )
        _dim = dim;
      else{
        if( dim != _dim )
          fail_assert("unequal descriptor length in keypoint_vset");
      }
    }
  }

  virtual int len() const { return _kpl->len(); }
  virtual int dim() const { return _dim; }
  virtual real get( int j, int d ) const { return _kpl->get_descr(j)[d]; }
  virtual void get( vector& dst, int j ) const { dst = _kpl->get_descr(j); }

  virtual int ldim() const {
    fail_assert("method keypoint_list_lvset::ldim() not implemented");
    return -1;
  }
  virtual int loading( int j ) const { return _kpl->get_class(j); }



private:
  const keypoint_list* _kpl;
  int _dim;
};

class keypoint_vset : public i_vector_set {
public:
  keypoint_vset( const keypoint_list* kpl, int cl, bool sel )  : _kpl(kpl) {
    for( int j = 0; j < kpl->len(); j++ ){
      int kp_cl = kpl->get_class(j);
      bool do_inc = sel ? (kp_cl == cl) : (kp_cl != cl);
      if( do_inc )
        _index.put(j);
      int dim = kpl->get_descr(j).len();
      if( j == 0 )
        _dim = dim;
      else{
        if( dim != _dim )
          fail_assert("unequal descriptor length in keypoint_vset");
      }
    }
  }

  virtual int len() const { return _index.len(); }
  virtual int dim() const { return _dim; }
  virtual real get( int j, int d ) const {
    int idx = _index[j];
    return _kpl->get_descr(idx)[d];
  }
  virtual void get( vector& dst, int j ) const {
    int idx = _index[j];
    dst = _kpl->get_descr(idx);
  }

  int get_id( int j ) const { return _kpl->get_id(_index[j]); }
  int_point get_point( int j ) const { return _kpl->get_point(_index[j]); }

private:
  const keypoint_list* _kpl;
  t_list<int> _index;
  int _dim;
};

// Реализация интерфейса i_vector_set поверх keypoint_list_lvset.
// Разбиение задается вектором pattern и значением cl.
// Из i_loaded_vector_set отбираются элементы, для которых
// соответствующий элемент векторв pattern равен cl.

class mkeypoint_vset : public i_vector_set {
public:
  mkeypoint_vset( const i_loaded_vector_set* kpl, int_vector& pattern, int cl_sel ) : _kpl(kpl) {
    for( int j = 0; j < kpl->len(); j++ ){
      int cl = kpl->loading(j);
      bool do_inc = (pattern[cl] == cl_sel);
      if( do_inc ){
        _index.put(j);
      }
    }
  }

  virtual int len() const { return _index.len(); }
  virtual int dim() const { return _kpl->dim(); }
  virtual real get( int j, int d ) const {
    int idx = _index[j];
    return _kpl->get(idx, d);
  }
  virtual void get( vector& dst, int j ) const {
    int idx = _index[j];
    _kpl->get(dst, idx);
  }

private:
  const i_loaded_vector_set* _kpl;
  t_list<int> _index;
};

// proportion точек из src_kpl помещает в test_kpl, остальные помещает в training_kpl
void rand_partition(
  const keypoint_list& src_kpl,
  keypoint_list& training_kpl,
  keypoint_list& test_kpl,
  real proportion,
  uint rand_seed
);

}; // namespace lwml

#endif // _KEYPOINTS_
