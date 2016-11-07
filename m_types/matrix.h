// Вещественная матрица
// lwml, (c) ltwood

#ifndef _MATRIX_
#define _MATRIX_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/m_types/vector.h"
#include "lwml/base/basex.h"
#include "lwml/m_base/i_vset.h"
#include "lwml/lwmlconf.h"
#include "lwml/io/stream.h"

/*#lake:stop*/

namespace lwml {

// Исключение при ошибке интерпретации содержимого файла

DEF_EX_TYPE_MSG(ex_integrity, ex_matrix, "error reading matrix");

// Матрица вещественных чисел.

typedef class real_matrix matrix;

class real_matrix : public value {
public:
  // Конструирование пустой матрицы.
  real_matrix();

  // Конструирование по размерам.
  real_matrix( int, int );

  // Конструирование по размерам и начальному значению.
  real_matrix( int, int, real val );

  // Конструирование по файлу, из которого производится чтение матрицы.
  // Файл должен содержать заголовок типа LMATR или RMATR.
  explicit real_matrix( referer<stream> file );

  // Конструирование по имени файла, из которого производится чтение матрицы.
  // Файл должен содержать заголовок типа LMATR или RMATR.
  explicit real_matrix( const char* );

  real_matrix( const real_matrix& );

  virtual ~real_matrix();

  void resize( int m, int n );
  void resize( int m, int n, real val );

  real_matrix& operator=( const real_matrix& src );

  // Получение размеров матрицы.
  int str() const { return _str; }
  int col() const { return _col; }

  // Индексирование элементов матрицы.
  real& operator()( int i, int j ) {
    TEST_INDEX2(i, _str, j, _col);
    return _data[i][j];
  }
  const real& operator()( int i, int j ) const {
    TEST_INDEX2(i, _str, j, _col);
    return _data[i][j];
  }

  // Индексирование без проверки индексов.
  // Для применения в надежных алгоритмах.
        real& at( int i, int j )       { return _data[i][j]; }
  const real& at( int i, int j ) const { return _data[i][j]; }

  // Сохранение матрицы в текстовом файле.
  // В первой строке сохраняется число строк и столбцов,
  // а далее - значения элементов матрицы по строкам.
  // Можно задать точность представления чисел (prec).
  // Параметр nf задает форму записи чисел при выводе.
  // При выводе в формате nfSCNT точность трактуется как число значащих цирф,
  // а при выводе в формате nfFIXED -- как число цифр после запятой.

  // Сохранение матрицы в текстовом файле.
  void save( referer<stream> file, int prec = 6, real_format nf = nfSCNT ) const;

  // Сохранение матрицы в текстовом файле с заданным именем.
  void save( const char *name, int prec = 6, real_format nf = nfSCNT ) const;

  // Скопировать в заданную параметрами i0, j0 позицию матрицы
  // содержимое другой матрицы, меньшего размера.
  // Оставшиеся элементы заполняются значениями, заданными в параметре val.
  void copy( const real_matrix& src, real val = 0.0, int i0 = 0, int j0 = 0 );

  // Вырезать из переданной матрицы прямоугольный участок,
  // размеры которого совпадают с размерами данной матрицы.
  // Параметры функции задают положение левого верхнего угла вырезаемого участка.
  void copy_sub( const real_matrix& src, int, int );

  void set_val( real val );  // присвоить всем элементам значение val
  void set_zero();           // присвоить всем элементам нулевое значение
  void set_diag( real x );   // сделать матрицу диагональной с элементами x на диагонали
  void swap( int k, int j ); // поменять местами строки
  void tran();               // транспонировать матрицу
  void center();             // центрировать элементы матрицы

  real mag() const;          // сумма квадратов элементов
  real norm() const;         // корень из суммы квадратов элементов
  real norm_manh() const;    // норма, согласованная с манхэттенской для векторов
  real trace() const;        // след матрицы (сумма диагональных элементов)
  real min() const;          // минимальный элемент
  real max() const;          // максимальный элемент
  real min_abs() const;      // минимальный по модулю элемент
  real max_abs() const;      // максимальный по модулю элемент
  real inner_sum() const;    // сумма элементов
  real mid() const;          // среднее значение элементов

  // Евклидово расстояние между двумя матрицами.
  static real dist( const real_matrix&, const real_matrix& );

  // простая покоординатная векторная арифметика
  real_matrix& operator+=( const real_matrix& );
  real_matrix& operator-=( const real_matrix& );
  real_matrix& operator*=( real );
  real_matrix& operator/=( real );

  // Копирование столбцов и строк между векторами и матрицами.
  void get_col( vector&, int c ) const;
  void get_str( vector&, int s ) const;
  void put_col( const vector&, int c );
  void put_str( const vector&, int s );

  // Домножение матрицы на матрицу-аргумент.
  void inc_mul( const real_matrix& );

  // Домножение матрицы на транспонированную матрицу-аргумент.
  void inc_mul_tran( const real_matrix& );

  // Перемножение двух матриц.
  static void mul( real_matrix& dst, const real_matrix& src1, const real_matrix& src2 );

  // умножение матрицы на транспонированную матрицу.
  static void mul_tran( real_matrix& dst, const real_matrix& src1, const real_matrix& src2 );

  // Возвращает скалярное произведение заданных строк матрицы.
  real inner_mul( int j1, int j2 );

  // Ортогонализация строк матрицы.
  static void gramm( real_matrix& a );

private:
  int _str, _col;
  real **_data;
  real *_repr;

  void alloc( int, int );
  void dealloc();

  void read_data( referer<stream> file, const char* fname );
};

// Реализация интерфейса i_vector_set на базе матрицы
// Считается, что каждая строка матрицы хранит один вектор

class matrix_vector_set : public i_vector_set, public hardcore {
public:
  matrix_vector_set( const matrix& lbl ){
    _data = &lbl;
  }

  virtual int len() const { return _data->str(); }
  virtual int dim() const { return _data->col(); }
  virtual real get( int j, int d ) const { return (*_data)(j, d); }
  virtual void get( vector& v, int j ) const { _data->get_str(v, j); }

private:
  const matrix *_data;
};

//!! TODO: пересмотреть

// void minittakens( real_matrix*, vector&, int winlen, int shift );

// real_matrix::gramm():
// матричный процесс ортогонализации Грамма-Шмидта
// k- строка исходной матрицы представлет k-й вектор
//
// e_k = f_k - sum_{j=1}^{n-1} L_j e_j
// L_j = (f_k,e_j)/(e_j,e_j)

}; // namespace lwml

#endif // _MATRIX_
