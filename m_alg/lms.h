// Различные МНК
// lwml, (c) ltwood

#ifndef _LMS_
#define _LMS_

//!! TODO: добавить уравнивание

#include "mdefs.h"
#include "polyn.h"
#include "vector.h"
#include "matrix.h"
#include "lud.h"

/*#lake:stop*/

namespace lwml {

// Полиномиальная аппроксимация по МНК
// Класс содержит только одну статическую функцию calc(),
// вычисляющую аппроксимирующий полином
// x, y - векторы, содержащие отсчеты точек
// c    - полином, в который помещаются вычисленные коэффициенты
// Возвращает относительную ошибку аппроксимации.

class lmspoly : public scope {
public:
  static real calc( const vector& x, const vector& y, polyn& c );
};

// Уравнивание переопределенной СЛАУ по МНК.
// Решается переопределенная система Ax=b, в которой каждому уравнению приписывается вес p[j].
// (Функция, не принимающая аргумент p решает ту же задачу для равновесных измерений.)
// Если матрица A имеет размер [mxn], то вектор x имеет размер [n], а вектора b и p имеют размер [m].
// Возвращает относительную ошибку аппроксимации.

class lmsleq : public value {
public:
  lmsleq( int n );

  real calc( vector& x, const matrix& a, const vector& b, const vector& p );
  real calc( vector& x, const matrix& a, const vector& b );

private:
  int _n;
  matrix _a;
  vector _b;
  lud _lu;
};

}; // namespace lwml

#endif // _LMS_
