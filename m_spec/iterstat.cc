#include "iterstat.h"

/*#lake:stop*/

namespace lwml {

// вычисление среднего и вариации по данным и флагам
void iter_stat::calc_mid(
  real* mid, real* dev, const vector& d, const t_array<bool>& fl, const vector* w
)
{
  summer sx, sxx;
  real num = 0;
  for( int j = 0; j < d.len(); j++ ){
    if( fl[j] ){
      real wj = (w == 0) ? 1.0 : (*w)[j];
      num += wj;
      real v = d[j];
      sx.add(v * wj);
      sxx.add(v * v * wj);
    }
  }

  *mid = sx.get() / num;
  *dev = sqrt(sxx.get() / num - fsqr(*mid));
}

int iter_stat::common_calc( real* mid, real* dev, real alpha, const vector& d, const vector* w )
{
  int len = d.len();

  t_array<bool> fl(len, true);
  int del_num = 0;
  while( true ){
    calc_mid(mid, dev, d, fl, w);
    int chnum = 0;
    for( int j = 0; j < len; j++ ){
      if( fl[j] ){
        bool nf = (fabs(d[j] - (*mid)) <= alpha * (*dev));
        fl[j] = nf;
        chnum += (!nf);
        del_num += (!nf);
      }
    }
    if( chnum == 0 )
      break;
  }

  return len - del_num;
}

int iter_stat::calc( const vector& d, real alpha, real* mid, real* dev )
{
  return common_calc(mid, dev, alpha, d);
}

int iter_stat::calc( const vector& d, const vector& w, real alpha, real* mid, real* dev )
{
  test_size(w.len(), d.len());
  return common_calc(mid, dev, alpha, d, &w);
}

}; // namespace lwml
