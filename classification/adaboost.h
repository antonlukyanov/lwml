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
   * �������� �������������� �� ������ ����������.
   *
   * @param sc_fact
   *   ������� ������� ���������������.
   * @param vs1
   *   ����� �������� ������� ������.
   * @param vs2
   *   ����� �������� ������� ������.
   * @param num
   *   ����� ����� ��������.
   * @param tick
   *   �������� �������� ���������.
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
   * �������������� ���������� �������������� �� �������� lua-�����.
   *
   * @param cnf
   *   ���� ������������.
   * @param root
   */
  adaboost( referer<luaconf> cnf, const char* root );

  virtual int dim() const { return _dim; }
  virtual int class_num() const { return 2; }
  virtual int step_num() const { return _step_num; }

  virtual int classify( const vector&, int num = 0 ) const;
  // ��� ��������� �������-��������� ������ ������ -
  // ���������� ����� ��������������� �� ������ ���� ��������
  // ���� ����� ������� ������, �� ����������� ������� ���������
  void classify( const vector&, vector& cl_value ) const;

  // ������� calc_error(s) ����� ���������� ��� ������ ������� �����, �� ����������� ��� ��� �� ������� ���������
  real calc_error( const i_vector_set& vs1, const i_vector_set& vs2, int num = 0 ) const;
  void calc_errors( const i_vector_set& vs1, const i_vector_set& vs2, vector& errors ) const;

  /**
   * ��������� �������������, ��� ����� �� ���������� ������
   * @param cl ����� ������
   * @param idx ������ ����� � ��������� ������
   * @return
   */
  virtual real get_complexity( int cl, int idx ) const;

  /**
   * �������� ������� ��������� ��� �������������
   * @param q ����������� ������ �������� ���������
   * @param num  ���������� ����� ��������������
   * @param m_idx ����� �������������� (����� ��� �������������� �������������)
   * @return
   */
  virtual void get_feature_quality( vector& q, int num = 0, int m_idx = -1 ) const;
  virtual real get_confidence( const vector& x, int num = 0 ) const;
  virtual real get_confidence_for_one_class( const vector& x, int num = 0, int class_idx = 0 ) const;

  virtual void save_result( referer<luaconf> res, const char* root ) const;
  virtual void save_result_file(const char *file_name) const;

  virtual void calc_errors( const keypoint_list_lvset& kpl, vector& errors ) const {};

private:
  /** ������ ��� ������������ ��� ���*/
  bool _is_test;
  /**����������� ��������*/
  int _dim;
  /**���������� �����*/
  int _step_num;
  double _eps;
  /**���� ��� �������� ������� ������*/
  vector _w1;
  /**���� ��� �������� ������� ������*/
  vector _w2;
  vector _alpha;

  // ��������� ����� �������� ���������(r)/�����������(w) � 1 � 2� ������
  /**���������� �����, ��������� �������� � 1-�� �����*/
  int_vector _n_r1;
  /**���������� �����, ����������� �������� � 1-�� �����*/
  int_vector _n_w1;
  /**���������� �����, ��������� �������� �� 2-�� �����*/
  int_vector _n_r2;
  /**���������� �����, ����������� �������� �� 2-�� �����*/
  int_vector _n_w2;
  /** �������� ����������� ����, ��� �����, ������� ��
  ���������������� � j-� ����� ������������� ��������� � ����*/
  vector _p1, _p2;
  /** ������ ������� ���������������*/
  t_array< refowner<i_simple_classifier> > _cl;
  /**������ �������� �������������*/
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

  // ����������� ����������� ����, ��� �����, ������� ���� ���������������� ��� ����� 0/1,
  // ������������� ����������� ���
  void calc_confidences( const i_vector_set& vs1, const i_vector_set& vs2 );
  // ��� �������������� �������-��������� ��� ���� ����� ��������������
  // ����������� ���� ����� ���� ������-��������� � � ���������������� �������� ������������ 1
  void calc_part_confidence( const vector& x, int cl );
};

}; // namespace lwml

#endif // _ADABOOST_
