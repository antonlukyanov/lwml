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

#include "keypoints.h"

namespace lwml {

/**
 * ��������� ��� ���������������.
 */
class i_classifier : public interface {
public:
  /**@return ����������� �������� */
  virtual int dim() const = 0;
  /** @return ���������� ������� */
  virtual int class_num() const = 0;
  /** @return ���������� ����� ������������� */
  virtual int step_num() const = 0;

  /**
   * ���������� ������������� ��������� �������.
   * @param ������ ��� �������������
   * @param num ���������� ����� �������������� (����� AdaBoost)
   * @return ��������� �������������
   */
  virtual int classify( const vector&, int num = 0 ) const = 0;

  /**
   * ��������� �������� ��������� �� �������� �������.
   *
   * @param q ������ � ��������� ���������
   * @param num ���������� ����� �������������� (����� AdaBoost)
   * @param m_idx ����� �������������� (����� ��� �������������� �������������)
   */
  virtual void get_feature_quality( vector& q, int num = 0, int m_idx = -1 ) const = 0;

  /**
   * �������� ������� ����������� �������������.
   * @param x ������������������ ������
   * @param num ���������� ����� ��������������
   * @return ������� �����������
   */
  virtual real get_confidence( const vector& x, int num = 0 ) const = 0;

  /**
   * �������� ������� ����������� ������������� � ������������� ������.
   * @param x ������������������ ������
   * @param num ���������� ����� �������������� (����� AdaBoost)
   * @param class_idx - ����� ������, ��� �������� ������������ ������� �����������
   * @return ������� �����������
   */
  virtual real get_confidence_for_one_class( const vector& x, int num = 0, int class_idx = 0 ) const = 0;

  /**
   * C��������� ����������� �������������.
   * @param cnf ��������� �� luaconf, ���� ��������� ����������
   * @param root ��� �������, � ������� ����� ��������� ����������
   */
  virtual void save_result( referer<luaconf> cnf, const char* root ) const = 0;

  /**
   * C��������� ����������� ������������� � ���� file_name
   * @param file_name ��� �����
   */
  virtual void save_result_file(const char *file_name) const = 0;

  /**
   * ��������� ������ ��� ������ �����
   * @param kpl ������ ����� (keypoint) ��� ������� ����� ������� ������
   * @param errors ������ ����������� ������
   */
  virtual void calc_errors( const keypoint_list_lvset& kpl, vector& errors ) const = 0;

  // ��������� �������������, ��� ����� �� ���������� ������
  //~virtual real get_complexity( int cl, int idx ) const = 0;

};

//
// ���������� ��� ������� ��������������� � ��� �� ��������.
//

/**
 * ��������� �������� �������������� (���������).
 */
class i_simple_classifier : public interface {
public:
  /**
   * ���������� ������������� ��������� ������� x.
   * @param x ������ ��� �������������
   * @return ���������� ��������� ������������� - �������� 0 ��� 1
   */
  virtual int classify( const vector& x ) const = 0;

  /**
   * ��������� ������ ���������� ����� ��������� ����������������� �������.
   * ������������� ����� ������� �� ������������ ���������� �������� ��������������.
   * @param w ������ ��� ����������.
   */
  virtual void get_weights( vector& w ) const = 0;

  /**
   * ��� ��������������.
   * @return ��� ��������������.
   */
  virtual const char* get_name() const = 0;

  /** @todo ������� ���������
   *
   * ������ � ���� ���������� ������������� � ���� lua �������.
   * @param cnf ���� ������������
   * @param root
   */
  virtual void serialize( referer<luaconf> cnf, const char* root ) const = 0;
};

/**
 * ��������� ������������ ������������ ������� ���������������.
 */
class i_simple_classifier_maker : public interface {
public:
  /**
   *  �������� �������������� �� ���� �������� ������� �������� � ������.
   * @param vs1 ����� �������� ������� ������
   * @param w1 ���� ��� �������� ������� ������
   * @param vs2 ����� �������� ������� ������
   * @param w2 ���� ��� �������� ������� ������
   * @return
   */
  virtual i_simple_classifier* create( const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2 ) const = 0;

  /**
   *
   * �������� �������������� �� �������� �����.
   * @param dim ����������� ������� ���������
   * @param cnf ���� ������������
   * @param root ��� �������, ��� ����� ������ ��������������
   * @return
   */
  virtual i_simple_classifier* create( int dim, referer<luaconf> cnf, const char* root ) const = 0;
};


/**
 * ������� ������������� �� ������ ������������ ���������� ���������� ��������.
 * @note ������ ����� �� ������������.
 */
class project_classifier : public i_simple_classifier {
public:
  /**
    * �������� �������������� �� ���� �������� ������� �������� � ������.
    * @param vs1 ����� �������� ������� ������
    * @param w1 ���� ��� �������� ������� ������
    * @param vs2 ����� �������� ������� ������
    * @param w2 ���� ��� �������� ������� ������
    * @return
    */
  project_classifier( const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2 );

  /**
   * �������� �������������� �� �������� lua-�����.
   * @param dim ����������� ��������
   * @param cnf ���� ������������
   * @param root ��� �������, ��� ����� ������ ��������������
   */
  project_classifier( int dim, referer<luaconf> cnf, const char* root );

  virtual int classify( const vector& x ) const;

  virtual void get_weights( vector& w ) const;

  virtual const char* get_name() const;

  virtual void serialize( referer<luaconf> cnf, const char* root ) const;

private:
  vector _dir;  /**< ����������� ��������, ���������� ����������� ��� ��������   */
  real _thr;
  bool _is_invers;

  void set_mid( vector& mid, const i_vector_set& vs, const vector& w ) const;
};


/**
 * ����������� ������� ��������������� ( ��������������� �� ������ ������������ ���������� ���������� ��������).
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
 * ������� ������������� �� ������ ������������ ������ ����������� ������������� ��������������.
 * @note ������ ����� �� ������������.
 */
class coord_classifier : public i_simple_classifier {
public:
  /**
    * �������� �������������� �� ���� �������� ������� �������� � ������.
    * @param vs1 ����� �������� ������� ������
    * @param w1 ���� ��� �������� ������� ������
    * @param vs2 ����� �������� ������� ������
    * @param w2 ���� ��� �������� ������� ������
    * @return
    */
  coord_classifier( const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2 );

  /**
   * �������� �������������� �� �������� lua-�����.
   * @param dim ����������� ��������
   * @param cnf ���� ������������
   * @param root ��� �������, ��� ����� ������ ��������������
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
  int _dim; /**< ����������� �������� */
  int _cidx; /**< ����� ���������� */
  real _thr;
  bool _is_invers;

  real calc_error(
    const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2, int cidx, real* thr, bool *is_invers,
    vector& buf, t_sorter<vector>& sort, vector& sum1, vector& sum2
  ) const;
};


/**
 * ����������� ������� ��������������� �� ������ ������������ ������ ����������� ������������� ��������������.
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
