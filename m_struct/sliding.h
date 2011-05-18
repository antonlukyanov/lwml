// Окно, скользящее по вектору данных
// lwml, (c) ltwood

#ifndef _SLIDING_
#define _SLIDING_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"
#include "i_vset.h"

/*#lake:stop*/

namespace lwml {

// Класс sliding формализует схему вырезания окон длиной len
// из вектора, причем начало следующего окна сдвигается по отношению
// к началу предыдущего на shift отсчетов
// При конструировании указывается вектор с исходными данными и
// класс хранит указатель на этот вектор
// Метод dim() позволяет получить ширину окна
// Метод num() дает общее количество окон
// Оператор () позволяет получить отсчеты указанного окна

class sliding : public i_vector_set, public hardcore {
public:
  // Наложение на вектор окна длины len с шагом shift.
  sliding( const vector&, int len, int shift = 1 );

  virtual int len() const { return _win_num; }  // число окон
  virtual int dim() const { return _win_len; }  // длина окна

  // Получение отсчета с номером d из окна с номером j.
  virtual real get( int j, int d ) const {
    test_index2(j, _win_num, d, _win_len);
    return (*_src)[j * _shift + d];
  }

  // Получение окна с номером j.
  virtual void get( vector& v, int j ) const {
    test_size(v.len(), _win_len);
    v.copy_sub(*_src, j * _shift);
  }

private:
  const vector* _src;
  int _win_len;         // ширина окна
  int _shift;           // сдвиг окна
  int _win_num;         // число окон

  // число окон длины winlen, которое можно наложить на данные длины len
  // при сдваге следующего окна на shift отсчетов по отношению к предыдущему
  int calc_win_num( int len, int winlen, int shift ){
    return (len - winlen) / shift + 1;
  }
};

}; // namespace lwml

#endif // _SLIDING_
