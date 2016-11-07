// keypoints.cc, (c) Anton Lukyanov

#include "lwml/classification/keypoints.h"

/*#build_stop*/

namespace lwml {

void rand_partition(
  const keypoint_list& src_kpl,
  keypoint_list& training_kpl,
  keypoint_list& test_kpl,
  real proportion,
  uint rand_seed
)
{
  int p_num = src_kpl.len() * proportion;
  int_vector mark(src_kpl.len(), 0);
  randselect set(p_num, src_kpl.len(), rand_seed);
  set.next();
  for( int i = 0; i < p_num; i++ )
    mark[set[i]] = 1;
  for( int i = 0; i < mark.len(); i++ ){
    int_point p = src_kpl.get_point(i);
    vector d = src_kpl.get_descr(i);
    int id = src_kpl.get_id(i);
    int cl = src_kpl.get_class(i);
    if( mark[i] == 0 )
      training_kpl.put(p, d, id, cl);
    else
      test_kpl.put(p, d, id, cl);
  }
}

}; // namespace lwml
