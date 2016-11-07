// Гистограммы
// lwml, (c) ltwood

//!! TODO: согласовать с herst

#ifndef _HISTOGRAM_
#define _HISTOGRAM_

#include "lwml/base/defs.h"
#include "lwml/m_types/ivector.h"

/*#lake:stop*/

namespace lwml {

// Класс histogram поддерживает построение гистограмм.
// При конструировании указывается число отрезков гистограммы (len)
// и отрезок [ax, bx], соответствующий гистограмме.

class histogram : public value {
public:
  histogram( int len, real ax, real bx );

  // сбросить гистограмму
  void clear();

  real ax() const { return _ax; }  // получить левую границу гистограммы
  real bx() const { return _bx; }  // получить правую границу гистограммы

  int len() const { return _data.len(); }  // получить число отрезков
  int total() const { return _total; }    // получить общее число занесенных точек

  int  val2idx( real val ) const; // преобразовать число в номер отрезка
  real idx2val( int idx ) const;  // получить по номеру отрезка его левую границу
  real idx2cnt( int idx ) const;  // получить по номеру отрезка его середину

  histogram& put( real x );          // добавить точку в гистограмму

  int max_idx() const; // получить номер отрезка с максимальным количеством точек
  int max_val() const; // получить число точек в самом населенном отрезке

  int operator[]( int j ) const { return _data[j]; } // получить число точек в j-м отрезке

  // построить функцию распределения
  // массив должен иметь длину, равную числу отрезков гистограммы
  // в j-й элемент массива заносится суммарное число точек
  // в отрезках с номерами k <= j
  void set_integral( int_vector& );

  // вычисление статистики хи-квадрат для гипотезы о равномерном распределении
  real chisq_val();  // значение статистики хи-квадрат S
  real chisq_prob(); // веротность того, что s <= S
  int  chisq_susp(); // оценка подозрительности S (0..3)

private:
  real _ax, _bx;
  int_vector _data;
  int _total;
};

}; // namespace lwml

#endif // _HISTOGRAM_
