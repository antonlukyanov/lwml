// Решение систем линейных уравнений.
// lwml, (c) ltwood

#ifndef _LUD_
#define _LUD_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/m_types/vector.h"
#include "lwml/m_types/matrix.h"
#include "lwml/m_types/ivector.h"

/*#lake:stop*/

namespace lwml {

// Класс реализует вычисление LU-разложения матрицы и решение
// систем линейных уравнений.
// Также вычисляются обратная матрица и оценка числа обусловленности.

class lud : public value {
public:
  // Создает объект заданного размера
  // Созданный объект можно в дальнейшем многократно
  // использовать для вычисления LU-разложения различных матриц.
  lud( int size );

  enum scaling { NOSCALE, SCALE };

  // Построить LU-разложение по заданной матрице
  // Матрица копируется во внутреннюю структуру и
  // вычисляется ее LU-разложение.
  // При необходимости выполняется автоматическое масштабирование
  // строк исходной матрицы.
  // do_scale - флаг необходимости автоматического масштабирования
  void put( const matrix& m, scaling sc = NOSCALE );

  // Решение системы уравнений с заданной правой частью
  // Вычисленное решение помещается на место правой части.
  void solve( vector& b ) const;

  // Решение системы уравнений с заданной правой частью
  // Здесь правая часть представлена матрицей, что позволяет
  // одновременно решать набор систем линейных уравнений,
  // правые части которых записаны в столбцах матрицы.
  // Вычисленное решение помещается на место правой части.
  void solve( matrix& b ) const;

  // Вычисление оценки числа обусловленности
  real cond_est() const;

  // Вычисление точного значения числа обусловленности
  real cond() const;

  // Вычисление определителя
  real det() const;

  // Вычисление логарифма определителя
  // Возвращает значение натурального логарифма
  // модуля определителя исходной матрицы.
  real logdet() const;

  // Вычисление знака определителя.
  // Используется совместно с методом logdet().
  real detsgn() const;

  // Заполнение матрицы элементами матрицы Гильберта
  // Заполняет квадратную матрицу элементами матрицы Гильберта.
  // Матрица Гильберта имеет очень высокое число обусловленности.
  static void set_hilbert( matrix& m );

private:
  matrix _data;         // LU-разложение
  vector _scale;        // масштабные коэффициенты
  vector _tst;          // тест обусловленности
  real   _src_norm;     // норма исходной матрицы

  int permsgn;          // (-1)**(число нетривиальных перестановок)
  int_vector perm;      // массив перестановок (len = str()-1)

  void set_test();
  void set_scale( bool do_scale );
  void gjdc();
};

}; // namespace lwml

#endif // _LUD_
