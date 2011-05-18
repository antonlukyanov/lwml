// Вычисление корней многочлена.
// lwml, (c) ltwood

#ifndef _PROOTS_
#define _PROOTS_

//!! TODO: проверить работу с абсолютной ошибкой

#include "defs.h"
#include "cmplx.h"
#include "polyn.h"
#include "vector.h"

namespace lwml {

const int PROOTS_MR    = 8;  // we try to break (rare) limit cycles with MR
const int PROOTS_MT    = 10; // different fractional values, once every MT steps
const int PROOTS_MAXIT = (PROOTS_MT*PROOTS_MR); // total allowed iterations

const real std_laguer_poly_eps = 1.0e-7; // ошибка вычисления значения многочлена
const real std_laguer_root_eps = 2.0e-6; // ошибка определения корня

// Класс proots реализует метод Лагуэрра с принудительным
// прерыванием предельных циклов.
// Содержит только статические методы

class proots : public scope {
public:
  // вычисление корней многочлена с вещественными коэффициентами, заданного как polyn
  // вычисляются комплексные значения корней
  // вещественные и мнимые части корней помещаются отдельно в векторы rroots и iroots
  // длины векторов rroots и iroots должны быть одинаковыми и равными степени многочлена
  // poly_eps задает ошибку вычисления значения многочлена
  // root_eps задает ошибку вычисления корней многочлена
  static void calc(
    const polyn& a,
    vector& rroots, vector& iroots,
    real poly_eps = std_laguer_poly_eps,
    real root_eps = std_laguer_root_eps
  );

private:
  static void laguer( const polyn& ar, const polyn& ai, int m, cmplx& x, real poly_eps );

  static const real _frac[PROOTS_MR+1];
};

}; // namespace lwml

#endif // _PROOTS_
