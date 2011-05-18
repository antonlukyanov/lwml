// Утилиты обработки изображений
// lwml, (c) ltwood

#ifndef _BMPUTL_
#define _BMPUTL_

#include "defs.h"
#include "mdefs.h"
#include "matrix.h"
#include "imatrix.h"

/*#lake:stop*/

namespace lwml {

// Класс реализует функции продвинутой локальной обработки точечных изображений.
// Эти методы относятся к "лабораторным" методам "признаковой" обработки
// полутоновых изображений, представленных матрицами яркостей.

// Стандартное значение для числа интервалов гистограммы
// Используется при выравнивании гистограммы полутонового изображения.

const int STD_BMP_HST_LEN = 64;

class bmputil : public scope {
public:
  // Выравнивание гистограммы изображения
  // hstlen - число интервалов гистограммы
  static void equalization( matrix& dst, const matrix& src, int hstlen );
  static void equalization( matrix& src, int hstlen );

  // Приведение изображения к заданному числу уровней яркости
  // lev - число уровней в результирующем изображении
  static void leveling( int_matrix& dst, const matrix& src, int lev );

  // Прореживание изображения.
  // Размеры матрицы src должны нацело делиться на
  // соответствующие размеры матрицы dst.
  static void decimation( matrix& dst, const matrix& src );

  // Преобразование матрицы к бинарному виду путем обрезания по порогу.
  // Порог задается из отрезка [0,1].
  static void thresholding( int_matrix& dst, const matrix& src, real val );

  // Бинаризация матрицы с помощью кластеризатора.
  static void clust_thresholding( int_matrix& dst, const matrix& src, int seed = 1 );

  // Фильтрация изображения по Собелу.
  static void sobel( matrix& dst, const matrix& src );

  // Простая фильтрация по окрестности 3x3.
  static void filter( matrix& dst, const matrix& src, real cnt, real cross, real diag );

  // Получение разности изображений.
  static void diff( matrix& dst, const matrix& src1, const matrix& src2 );

  // Построение гистограммы.
  static void hist( vector& dst, const matrix& src, int hst_len );

  // Вычисление матрицы признаков смежности цветов
  // Элемент результирующей матрицы равен 0,1,2 в зависимости от того,
  // сколько из следующих условий выполнено (здесь 'цвет' = 'яркость'):
  // - цвет точки равен c1 и цвет ее правого соседа равен c2
  // - цвет точки равен c1 и цвет ее нижнего соседа равен c2
  // c1 - цвет центральной точки
  // c2 - цвет соседней точки
  static void cooccuring( int_matrix& dst, const int_matrix& src, int c1, int c2 );

  // Вычисление диагональных элементов матрицы признаков смежности цветов
  // Элемент результирующей матрицы равен 0,1,2 в зависимости от того,
  // сколько из следующих условий выполнено:
  // - перепад яркости при переходе к правому соседу равен заданному числу
  // - перепад яркости при переходе к нижнему соседу равен заданному числу
  // dc - перепад яркости
  static void cooccuring( int_matrix& dst, const int_matrix& src, int dc );

  // Cуммирование по окрестности заданного радиуса для вещественной матрицы
  // Элемент результирующей матрицы равен сумме элементов
  // исходной матрицы, расположенных в заданной окрестности
  // nbh_size - радиус окрестности
  // prg - прогресс-индикатор
  static void counting( matrix& dst, const matrix& src, int nbh_size, tick_mode tick = tmOFF );

  // Cуммирование по окрестности заданного радиуса для целочисленной матрицы
  // Элемент результирующей матрицы равен сумме элементов
  // исходной матрицы, расположенных в заданной окрестности
  // nbh_size - радиус окрестности
  // prg - прогресс-индикатор
  static void counting( int_matrix& dst, const int_matrix& src, int nbh_size, tick_mode tick = tmOFF );

  // Вращение изображения вокруг центра
  static void rot( int_matrix& dst, const int_matrix& src, real angle );
};

}; // namespace lwml

#endif // _BMPUTL_
