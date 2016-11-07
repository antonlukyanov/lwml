// Минимизация функции методом Хука-Дживса (Hooke and Jeeves)
// lwml, (c) ltwood

#ifndef _HOOKE_
#define _HOOKE_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"

#include "lwml/m_base/i_vfunc.h"

/*#lake:stop*/

namespace lwml {

/*
 * Методу не требуется непрерывность функции f(X), для нее
 * не вычисляются и не требуется предоставлять производные.
 *
 * Ссылки:
 * R.Hooke, T.A.Jeeves, "Direct Search Solution of Numerical
 * and Statistical Problems", Journal of the ACM, Vol.8,
 * April 1961, pp.212-229.
 * "Algorithm 178: Direct Search" by Arthur F. Kaupe Jr.,
 * Communications of the ACM, Vol.6, p.313 (June 1963).
 * Some improvements: Bell and Pike (CACM v.9, p.684, Sept 1966)
 * and Tomlin and Smith, "Remark on Algorithm 178" (CACM v.12).
 */

/*
 * Итерацией считается любой шаг по координатам, поэтому
 * число шагов может быть значительным.
 * Функция calc() возвращает количество сделанных шагов.
 *
 * Параметр rho используется как множитель, на который умножается шаг
 * после каждой итерации.
 * Общее число итераций определяется соотношением rho ** N = eps.
 * Большие значения параметра rho дают большую вероятность сходимость для
 * сложных функций за счет большего числа тестируемых точек.
 * Обычно лучше стартовать с агрессивного значения rho = 0.5
 * в надежде на быструю сходимость.
 * При получении неудовлетворительного результата можно увеличить
 * rho до пессимистичных значений порядка 0.85.
 */

// Хитрый прогресс индикатор для метода Хука-Дживса.

class i_hooke_progress : public interface {
public:
  // После каждого шага алгоритм вызывает функцию up(),
  // передавая ей некую степень близости вычислений
  // к концу -- число из промежутка [0, 1].
  virtual void up( real ) = 0;
};

class hooke : public scope {
public:
  // fn - минимизируемая функция
  // x0 - стартовая точка
  // x - результат оптимизации
  // dx - шаги по каждой координате
  // rho - параметр, управляющий сходимостью (0 <= rho <= 1.0)
  // eps - критерий остановки, минимальный допустимый шаг
  // maxit - максимальное число итераций
  // Возвращаемое значение -- номер шага n, на котором произошла остановка (0 <= n < maxit).
  // Если остановка произошла из-за того, что уже прошло maxit итераций,
  // то возвращается значение maxit.
  static int calc(
    const i_vfunction& fn,
    vector& x0, vector& x, vector& dx,
    double rho, double eps, int maxit,
    i_hooke_progress* prg = 0
  );

  // Эта функция отличается только тем, что вместо вектора dx шагов по каждой координате
  // принимает единственный шаг, используемый как стартовый шаг для всех координат.
  static int calc(
    const i_vfunction& fn,
    vector& x0, vector& x, real dx_val,
    double rho, double eps, int maxit,
    i_hooke_progress* prg = 0
  );

private:
  static real local_search( const i_vfunction& fn, const vector& dx, vector& x, real fmin );
};

}; // namespace lwml

#endif // _HOOKE_
