// Iterative statistic
// lwml, (c) ltwood

#ifndef _ISTAT_
#define _ISTAT_

#include "defs.h"
#include "mdefs.h"

#include "vector.h"
#include "t_array.h"

/*#lake:stop*/

namespace lwml {

/*
 * »теративное оценивание среднего и вариации (ср-кв. уклонение).
 * —начала все точки считаютс€ "хорошими".
 * Ќа каждом шаге среднее и вариаци€ вычисл€ютс€ только по "хорошим" точкам.
 * ѕосле этого все точки, не попавшие в alpha*dev-окрестность среднего,
 * исключаютс€ из множества "хороших".
 * ¬ычисление повтор€етс€ до стабилизации множества "хороших" точек.
 * ‘ункции возвращают количество точек, участвовавших
 * в вычислении результирующих оценок mid и dev.
 */

class iter_stat : public scope {
public:
  // ¬ычисление дл€ равновесных значений.
  static int calc( const vector& d, real alpha, real* mid, real* dev );

  // ¬ычисление дл€ значений с заданными весами.
  static int calc( const vector& d, const vector& w, real alpha, real* mid, real* dev );

private:
  static void calc_mid( real* mid, real* dev, const vector& d, const t_array<bool>& fl, const vector* w);

  static int common_calc( real* mid, real* dev, real alpha, const vector& d, const vector* w = 0 );
};

}; // namespace lwml

#endif // _ISTAT_
