// Iterative statistic
// lwml, (c) ltwood

#ifndef _ISTAT_
#define _ISTAT_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"

#include "lwml/m_types/vector.h"
#include "lwml/types/t_array.h"

/*#lake:stop*/

namespace lwml {

/*
 * Итеративное оценивание среднего и вариации (ср-кв. уклонение).
 * Сначала все точки считаются "хорошими".
 * На каждом шаге среднее и вариация вычисляются только по "хорошим" точкам.
 * После этого все точки, не попавшие в alpha*dev-окрестность среднего,
 * исключаются из множества "хороших".
 * Вычисление повторяется до стабилизации множества "хороших" точек.
 * Функции возвращают количество точек, участвовавших
 * в вычислении результирующих оценок mid и dev.
 */

class iter_stat : public scope {
public:
  // Вычисление для равновесных значений.
  static int calc( const vector& d, real alpha, real* mid, real* dev );

  // Вычисление для значений с заданными весами.
  static int calc( const vector& d, const vector& w, real alpha, real* mid, real* dev );

private:
  static void calc_mid( real* mid, real* dev, const vector& d, const t_array<bool>& fl, const vector* w);

  static int common_calc( real* mid, real* dev, real alpha, const vector& d, const vector* w = 0 );
};

}; // namespace lwml

#endif // _ISTAT_
