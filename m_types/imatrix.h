// Целочисленная матрица
// lwml, (c) ltwood

#ifndef _IMATRIX_
#define _IMATRIX_

#include "defs.h"
#include "ivector.h"
#include "basex.h"
#include "lwmlconf.h"
#include "stream.h"

/*#lake:stop*/

namespace lwml {

// Исключение при ошибке интерпретации содержимого файла

DEF_EX_TYPE_MSG(ex_integrity, ex_imatrix, "error reading int_matrix");

class int_matrix : public value {
public:
  // Конструирование пустой матрицы.
  int_matrix();

  // Конструирование по размерам.
  int_matrix( int, int );

  // Конструирование по размерам и начальному значению элементов.
  int_matrix( int, int, int val );

  // Конструирование по файлу, из которого производится чтение матрицы.
  // Файл должен содержать заголовок типа LMATR.
  explicit int_matrix( referer<stream> file );

  // Конструирование по имени файла, из которого производится чтение матрицы.
  // Файл должен содержать заголовок типа LMATR.
  explicit int_matrix( const char* );

  int_matrix( const int_matrix& );

  virtual ~int_matrix();

  void resize( int m, int n );
  void resize( int m, int n, int val );

  int_matrix& operator=( const int_matrix& src );

  // Получение размеров матрицы.
  int str() const { return _str; }
  int col() const { return _col; }

  int& operator()( int i, int j ) {
    TEST_INDEX2(i, _str, j, _col);
    return _data[i][j];
  }
  const int& operator()( int i, int j ) const {
    TEST_INDEX2(i, _str, j, _col);
    return _data[i][j];
  }

  // Индексирование без проверки индексов.
  // Для применения в надежных алгоритмах.
        int& at( int i, int j )       { return _data[i][j]; }
  const int& at( int i, int j ) const { return _data[i][j]; }

  // Сохранение матрицы в текстовом файле.
  void save( referer<stream> file ) const;

  // Сохранение матрицы в текстовом файле с заданным именем.
  void save( const char *name ) const;

  // Скопировать в заданную параметрами i0, j0 позицию матрицы
  // содержимое другой матрицы, меньшего размера.
  // Оставшиеся элементы заполняются значениями, заданными в параметре val.
  void copy( const int_matrix& src, int val = -1, int i0 = 0, int j0 = 0 );

  // Вырезать из переданной метрицы прямоугольный участок, размеры которого
  // совпадают с размерами данной матрицы.
  // Параметры i0, j0 задают положение левого верхнего угла вырезаемого участка.
  void copy_sub( const int_matrix& src, int i0, int j0 );

  void set_val( int val );   // присвоить всем элементам значение val
  void set_zero();           // присвоить всем элементам нулевое значение
  void set_diag( int x );    // сделать матрицу диагональной с элементами x на диагонали
  void swap( int k, int j ); // поменять местами строки
  void tran();               // транспонировать матрицу

  int min() const;           // минимальный элемент
  int max() const;           // максимальный элемент
  int min_abs() const;       // минимальный по модулю элемент
  int max_abs() const;       // максимальный по модулю элемент
  int inner_sum() const;     // сумма элементов

  // Скопировать столбец или строку матрицы в переданный вектор.
  void get_col( int_vector&, int c ) const;
  void get_str( int_vector&, int s ) const;

  // Скопировать содержимое переданного вектора в заданный столбец или строку матрицы.
  void put_col( const int_vector&, int c );
  void put_str( const int_vector&, int s );

private:
  int _str, _col;
  int **_data;
  int *_repr;

  void alloc( int m, int n );
  void dealloc();

  void read_data( referer<stream> file, const char* fname );
};

}; // namespace lwml

#endif // _IMATRIX_
