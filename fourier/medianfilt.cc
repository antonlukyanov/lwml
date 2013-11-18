#include "medianfilt.h"
#include "t_sel_sort.h"

/*#lake:stop*/

namespace lwml {

namespace{
  int wrap_idx( int idx, int n ){
    //!! SIC: n > 0
    while( idx < 0 )
      idx += n;
    return idx % n;
  }
};

bool medianfilt::calc( vector& dst, const vector& src, int rank, topology top )
{
  test_size(dst.len(), src.len());
  int n = src.len();
  if( n == 0 )
    fail_assert("empty source vector");
  vector win(2 * rank + 1);

  bool ret = false;

  t_sel_sort<vector> sort;
  for( int i = 0; i < n; ++i ){
    for( int j = -rank; j <= rank; ++j ){
      int idx = i + j;
      if( idx >= 0 && idx < n )
        win[rank+j] = src[idx];
      else
        win[rank+j] = (top == CIRCLE) ? src[wrap_idx(idx, n)] : (idx < 0 ? src[0] : src[n-1]);
    }
    sort.put(win);
    dst[i] = win[sort[rank]];
    if( dst[i] != src[i] )
      ret = true;
  }
  return ret;
}

// 2d median filter

namespace {
  // Инициализация гистограммы по участку изображения.
  // Дополнительно вычисляет медиану по гистограмме.
  // В переменную *num помещает количество обработанных точек.
  // В переменную *less помещает количество точек, лежащих левее медианы.
  // Возвращает значение индекса, соответствующее медиане.
  int set_hist(
    int_vector& hst, const int_matrix& data, int x0, int y0, int apt_x, int apt_y, int min,
    int* less, int* num
  )
  {
    int lx = data.col();
    int ly = data.str();

    // построение гистограммы
    hst.set_val(0);
    *num = 0;
    for( int dy = -apt_y; dy <= apt_y; ++dy ){
      int y = y0 + dy;
      for( int dx = -apt_x; dx <= apt_x; ++dx ){
        int x = x0 + dx;
        if( x >= 0 && x < lx && y >= 0 && y < ly ){
          ++hst[data(y, x) - min];
          ++(*num);
        }
      }
    }

    // вычисление медианы
    int num2 = (*num-1)/2;
    int med_idx = 0;
    *less = 0;
    while( *less + hst[med_idx] <= num2 ){
      *less += hst[med_idx];
      ++med_idx;
    }
    return med_idx + min;
  }
}

void medianfilt::calc_test( int_matrix& dst, const int_matrix& src, int apt )
{
  int lx = src.col();
  int ly = src.str();
  int min = src.min();
  int max = src.max();
  int len = max - min + 1;

  int_vector hst(len);
  int less, num;

  for( int y = 0; y < ly; ++y ){
    for( int x = 0; x < lx; ++x ){
      dst(y, x) = set_hist(hst, src, x, y, apt, apt, min, &less, &num);
    }
  }
}

void medianfilt::calc_test( int_matrix& dst, const int_matrix& src, int apt_x, int apt_y )
{
  int lx = src.col();
  int ly = src.str();
  int min = src.min();
  int max = src.max();
  int len = max - min + 1;

  int_vector hst(len);
  int less, num;

  for( int y = 0; y < ly; ++y ){
    for( int x = 0; x < lx; ++x ){
      dst(y, x) = set_hist(hst, src, x, y, apt_x, apt_y, min, &less, &num);
    }
  }
}

int medianfilt::calc( int_matrix& dst, const int_matrix& src, int apt )
{
  int lx = src.col();
  int ly = src.str();
  int min = src.min();
  int max = src.max();
  int len = max - min + 1;

  int_vector hst(len);
  int chnum = 0;
  for( int y = 0; y < ly; ++y ){
    int less, num;
    int med = set_hist(hst, src, 0, y, apt, apt, min, &less, &num);
    if( med != dst(y, 0) ){
      ++chnum;
      dst(y, 0) = med;
    }
    for( int x = 1; x < lx; ++x ){
      if( x-apt-1 >= 0 ){ // удаление вертикальной линии
        for( int yy = y - apt ; yy <= y + apt; ++yy ){
          if( yy >= 0 && yy < ly ){
            --num;
            int v = src(yy, x-apt-1);
            --hst[v - min];
            if( v < med ) --less;
          }
        }
      }
      if( x+apt < lx ){ // добавление вертикальной линии
        for( int yy = y - apt ; yy <= y + apt; ++yy ){
          if( yy >= 0 && yy < ly ){
            ++num;
            int v = src(yy, x+apt);
            ++hst[v - min];
            if( v < med ) ++less;
          }
        }
      }
      // корректируем медиану
      int num2 = (num-1)/2;
      if( less > num2 ){
        while( less > num2 ){
          --med;
          less -= hst[med - min];
        }
      } else {
        while( less + hst[med - min] <= num2 ){
          less += hst[med - min];
          ++med;
        }
      }
      if( med != dst(y, x) ){
        ++chnum;
        dst(y, x) = med;
      }

    }
  }

  return chnum;
}

int medianfilt::calc( int_matrix& dst, const int_matrix& src, int apt_x, int apt_y )
{
  int lx = src.col();
  int ly = src.str();
  int min = src.min();
  int max = src.max();
  int len = max - min + 1;

  int_vector hst(len);
  int chnum = 0;
  for( int y = 0; y < ly; ++y ){
    int less, num;
    int med = set_hist(hst, src, 0, y, apt_x, apt_y, min, &less, &num);
    if( med != dst(y, 0) ){
      ++chnum;
      dst(y, 0) = med;
    }
    for( int x = 1; x < lx; ++x ){
      if( x-apt_x-1 >= 0 ){ // удаление вертикальной линии
        for( int yy = y - apt_y; yy <= y + apt_y; ++yy ){
          if( yy >= 0 && yy < ly ){
            --num;
            int v = src(yy, x-apt_x-1);
            --hst[v - min];
            if( v < med ) --less;
          }
        }
      }
      if( x+apt_x < lx ){ // добавление вертикальной линии
        for( int yy = y - apt_y; yy <= y + apt_y; ++yy ){
          if( yy >= 0 && yy < ly ){
            ++num;
            int v = src(yy, x+apt_x);
            ++hst[v - min];
            if( v < med ) ++less;
          }
        }
      }
      // корректируем медиану
      int num2 = (num-1)/2;
      if( less > num2 ){
        while( less > num2 ){
          --med;
          less -= hst[med - min];
        }
      } else {
        while( less + hst[med - min] <= num2 ){
          less += hst[med - min];
          ++med;
        }
      }
      if( med != dst(y, x) ){
        ++chnum;
        dst(y, x) = med;
      }

    }
  }

  return chnum;
}

}; // namespace lwml
