// ������������� �� ���������� �������.
// lwml, (c) ltwood

#ifndef _MBOOST_
#define _MBOOST_

#include "defs.h"
#include "mdefs.h"
#include "ivector.h"
#include "randcomb.h"
#include "debug.h"
#include "vbmp.h"

#include "adaboost.h"
#include "msg_receiver.h"
#include "keypoints.h"

/*#build_stop*/

namespace lwml {

//
// Multiclass AdaBoost
//

class mult_adaboost : public i_classifier {
public:
  enum mboost_type { ONE_VS_ALL = 0, ONE_VS_ONE = 1, RAND_HALF = 2 };

  /**
   * �������� �������������� �� ������ ���������� � ��� ��������.
   *
   * @param m_type
   *   ��� mult_adaboost: ONE_VS_ALL, ONE_VS_ONE ��� RAND_HALF.
   * @param steps_num
   *   ����� ����� ������� �������������� adaboost �� ����� ��������.
   * @param sc_fact
   *   ������� ������� ���������������.
   * @param kpl
   *   ������ ��������.
   * @param weights
   *   ������ ����� ��������.
   * @param classes_num
   *   ����� �������.
   * @param msg
   *   ������.
   * @param tick
   */
  mult_adaboost(
    // ��������� mult_adaboost.
    mboost_type m_type,
    int steps_num,
    const i_simple_classifier_maker& sc_fact,
    // ��������� �������.
    const keypoint_list_lvset& kpl,
    int classes_num,
    // ������ � ��������-���������.
    const i_msg_receiver& log,
    tick_mode tick = tmOFF
  );

  mult_adaboost( referer<luaconf> cnf, const char* root );

  ~mult_adaboost(){
    for( int i = 0; i < _m_ab.len(); i++ )
      delete _m_ab[i];
  }

  virtual int dim() const { return _dim; }
  virtual int class_num() const { return _class_num; }
  virtual int step_num() const { return _max_step_num; }

  virtual int classify( const vector& x, int num = 0 ) const;

  virtual void get_feature_quality( vector& q, int num = 0, int m_idx = -1 ) const;
  virtual real get_confidence( const vector& x, int num = 0 ) const;
  virtual real get_confidence_for_one_class( const vector& x, int num = 0, int class_idx = 0 ) const;
  //~   virtual real get_complexity( int cl, int idx ) const;
  virtual void save_result( referer<luaconf> res, const char* root ) const;
  virtual void save_result_file(const char *file_name) const;

  // ������� calc_error(s) ����� ���������� ��� ������ ����� �� �� ������� ���������
  real calc_error( const keypoint_list_lvset& kpl, int num ) const;
  virtual void calc_errors( const keypoint_list_lvset& kpl, vector& errors ) const;
  // ������� ������� ������������ �������������� ����� � ������, ��� ��������� ������� ���������������
  void dump_probabilities( int num );

private:
  int _dim;
  int _max_step_num;
  int _class_num, _adaboost_num;
  int_matrix _coding_matrix;
  t_array<adaboost*> _m_ab;
  t_array<matrix> _probabilities_of_classes;
  vector _cl_probability;

  int calc_adabost_num( int class_num, mboost_type m_type ) const;

  void calc_confidence( const keypoint_list_lvset& kpl );
  int calc_best_class( const vector x, int num, real* confidence ) const;
  // ������� ������ ������������ �������������� � ������ ��� ������� ��������� ������� ������� �������� ���������������
  // ���������� ����������� ������� ������
  real calc_conditional_prob( int num, const int_vector& ab_answer, vector& conditional_prob ) const;

  // ������� ��� ��������� coding_matrix
  static void gen_one_vs_all( int_matrix &dst )
  {
    dst.set_val(-1);
    for( int i = 0; i < dst.col(); i++ )
      dst(i, i) = 1;
  }

  static void gen_one_vs_one( int_matrix &dst )
  {
    dst.set_val(0);
    int col = 0;
    for( int i = 0; i < dst.str(); i++ ){
      for( int j = i+1; j < dst.str(); j++ ){
        dst(i, col) = 1;
        dst(j, col) = -1;
        col++;
        if( col > dst.col() )
          fail_assert("mult_adaboost gen_one_vs_one::incorrect coding_matrix");
      }
    }
    if( col != dst.col() )
      fail_assert("mult_adaboost gen_one_vs_one::incorrect coding_matrix");
  }

  static void gen_rand_half( int_matrix &dst )
  {
    int cl_num = dst.str();
    int half = cl_num / 2;

    dst.set_val(-1);
    randselect set(half, cl_num);
    for( int i = 0; i < dst.col(); i++ ){
      set.next();
      for( int k = 0; k < half; k++ ){
        dst(set[k], i) = 1;
      }
    }
  }

};

}; // namespace lwml

#endif // _MBOOST_
