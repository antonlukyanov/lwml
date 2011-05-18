// Вещественный вектор
// lwml, (c) ltwood

#ifndef _VECTOR_
#define _VECTOR_

#include "defs.h"
#include "mdefs.h"
#include "basex.h"
#include "lwmlconf.h"
#include "stream.h"

/*#lake:stop*/

namespace lwml {

// Исключение при ошибке интерпретации содержимого файла

DEF_EX_TYPE_MSG(ex_integrity, ex_vector, "error reading vector");

typedef class real_vector vector;

class real_vector : public value {
public:
  // Конструирование по длине.
  explicit real_vector( int len = 0 );

  // Конструирование по длине и начальному значению элементов.
  real_vector( int len, real val );

  // Конструирование по длине и статическому массиву, из которого
  // будут взяты значения отсчетов вектора.
  real_vector( int len, const real* );

  // Конструирование по файлу, из которого производится чтение вектора.
  // Файл должен содержать заголовок типа RVECT или LVECT.
  explicit real_vector( referer<stream> file );

  // Конструирование по имени файла, из которого производится чтение вектора.
  // При отсутствии заголовка типа RVECT или LVECT производится чтение "сырого" текстового файла.
  explicit real_vector( const char* );

  real_vector( const real_vector& src );
  virtual ~real_vector();

  real_vector& operator=( const real_vector& src );

  int len() const { return _len; }              // Получение длины вектора.

  void resize( int len ) { realloc(len); }

  real& operator[]( int j ) {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[j];
  }
  const real& operator[]( int j ) const {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[j];
  }

  // Индексирование без проверки индекса.
  // Для применения в надежных алгоритмах.
        real& at( int j )       { return _data[j]; }
  const real& at( int j ) const { return _data[j]; }

  // Сохранение отсчетов вектора в текстовом файле.
  // Можно задать точность prec представления чисел.
  // Параметр nf задает форму записи чисел при выводе.
  // При выводе в формате nfSCNT точность трактуется как число значащих цирф,
  // а при выводе в формате nfFIXED -- как число цифр после запятой.

  // Сохранение вектора в текстовом файле.
  void save( referer<stream> file, int prec = 6, real_format nf = nfSCNT ) const;

  // Сохранение вектора в текстовом файле с заданным именем.
  void save( const char *name, int prec = 6, real_format nf = nfSCNT ) const;

  // Сохранение вектора в текстовом файле с заданным именем без заголовка.
  void export_as_text( const char *name, int prec = 6, real_format nf = nfSCNT ) const;

  // Скопировать в заданную параметром idx0 позицию вектора
  // содержимое другого вектора меньшей длины.
  // При этом оставшиеся отсчеты вектора заполняются значениями,
  // заданными в параметре val.
  void copy( const real_vector& src, real val = 0.0, int idx0 = 0 );

  // Вырезать из переданного вектора участок длины len().
  // Параметр idx задает положение начала вырезаемого участка.
  void copy_sub( const real_vector& src, int idx );

  void set_val( real val );      // присвоить всем отсчетам значение val
  void set_zero();               // обнулить все отсчеты
  void swap( int k, int j );     // поменять k-й и j-й элементы местами
  void revers();                 // изменить порядок элементов на обратный
  void center();                 // центрировать отсчеты вектора

  real mag() const;              // сумма квадратов элементов
  real norm() const;             // корень из суммы квадратов элементов
  real norm_manh() const;        // манхэттенская норма
  real min() const;              // минимальный элемент
  real max() const;              // максимальный элемент
  int  min_idx() const;          // индекс минимального элемента
  int  max_idx() const;          // индекс максимального элемента
  real min_abs() const;          // минимальный по модулю элемент
  real max_abs() const;          // максимальный по модулю элемент
  int  min_abs_idx() const;      // индекс минимального по модулю элемента
  int  max_abs_idx() const;      // индекс максимального по модулю элемента
  real inner_sum() const;        // сумма элементов
  real mid() const;              // среднее значение элементов
  real disp() const;             // дисперсия элементов

  // Вычисление k-го отсчета автокорреляционной функции, равного
  // среднему значению произведения центрированных отсчетов,
  // отстящих друг от друга на k отсчетов.
  // При этом для всех k знаменатель при усреднении равен len().
  real acorr( int k ) const;

  // Вычисление значения кусочно-линейной интерполяции.
  // Элементы вектора, рассматриваются как отсчеты функции в целочисленных точках
  // и вычисляет значение полученной функции в точке t.
  // При этом значение t должно лежать на отрезке [0, len()-1].
  real interpol( real t ) const; // значение в точке t при интерполяции

  // Евклидово расстояние между двумя векторами.
  static real dist( const real_vector&, const real_vector& );

  // Скалярное произведение двух векторов.
  static real inner_mul( const real_vector&, const real_vector& );

  // Коэффициент корреляции для двух векторов.
  static real corr( const real_vector&, const real_vector& );

  // k-й отсчет кроссковариации двух векторов.
  // При вычислении кроссковариации находится сумма произведениий центрированных
  // отсчетов из векторов, для отсчетов, отстящих друг от друга на k отсчетов.
  static real ccorr( const real_vector&, const real_vector&, int );

  // Длина проекции второго вектора (src) на первый (dst).
  static real project( const real_vector& dst, const real_vector& src );

  // простая покоординатная векторная арифметика
  real_vector& operator+=( const real_vector& );
  real_vector& operator-=( const real_vector& );
  real_vector& operator*=( real );
  real_vector& operator/=( real );

  int comp( int i, int j ) const {
    return (_data[i] < _data[j]) ? -1 : (_data[i] > _data[j]) ? 1 : 0;
  }

private:
  int   _len;
  real *_data;

  void dealloc();
  void alloc( int len );
  void realloc( int len );

  void read_raw_data( const char* fname );
  bool read_data( referer<stream> file, const char* fname );
};

}; // namespace lwml

#endif // _VECTOR_
