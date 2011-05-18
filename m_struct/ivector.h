// Целочисленный вектор
// lwml, (c) ltwood

#ifndef _IVECTOR_
#define _IVECTOR_

#include "defs.h"
#include "basex.h"
#include "lwmlconf.h"
#include "stream.h"

/*#lake:stop*/

namespace lwml {

// Исключение при ошибке интерпретации содержимого файла

DEF_EX_TYPE_MSG(ex_integrity, ex_ivector, "error reading int_vector");

class int_vector : public value {
public:
  // Конструирование по длине.
  explicit int_vector( int len = 0 );

  // Конструирование по длине и начальному значению элементов.
  int_vector( int len, int val );

  // Конструирование по длине и статическому массиву, из которого
  // будут взяты значения отсчетов вектора.
  int_vector( int len, const int* );

  // Конструирование по файлу, из которого производится чтение вектора.
  // Файл должен содержать заголовок типа LVECT.
  explicit int_vector( referer<stream> file );

  // Конструирование по имени файла, из которого производится чтение вектора.
  // При отсутствии заголовка типа LVECT производится чтение "сырого" текстового файла.
  explicit int_vector( const char* );

  int_vector( const int_vector& );
  virtual ~int_vector();

  int_vector& operator=( const int_vector& src );

  int len() const { return _len; }              // Получение длины вектора.

  void resize( int len ) { realloc(len); }

  int& operator[]( int j ) {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[j];
  }
  const int& operator[]( int j ) const {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[j];
  }

  // Сохранение вектора в текстовом файле.
  void save( referer<stream> file ) const;

  // Сохранение вектора в текстовом файле с заданным именем.
  void save( const char *name ) const;

  // Сохранение вектора в текстовом файле с заданным именем без заголовка.
  void export_as_text( const char *name ) const;

  // Скопировать в заданную параметром idx0 позицию вектора
  // содержимое другого вектора меньшей длины.
  // При этом оставшиеся отсчеты вектора заполняются значениями,
  // заданными в параметре val.
  void copy( const int_vector& src, int val = -1, int idx0 = 0 );

  // Вырезать из переданного вектора участок длины len().
  // Параметр idx задает положение начала вырезаемого участка.
  void copy_sub( const int_vector& src, int idx );

  void set_val( int val );     // присвоить всем отсчетам значение val
  void set_zero();             // обнулить все элементы
  void swap( int k, int j );   // поменять k-й и j-й элементы местами
  void revers();               // изменить порядок элементов на обратный

  int min() const;             // найти минимальный элемент
  int max() const;             // найти максимальный элемент
  int min_idx() const;         // найти индекс минимального элемента
  int max_idx() const;         // найти индекс максимального элемента
  int min_abs() const;         // минимальный по модулю элемент
  int max_abs() const;         // максимальный по модулю элемент
  int min_abs_idx() const;     // индекс минимального по модулю элемента
  int max_abs_idx() const;     // индекс максимального по модулю элемента
  int inner_sum() const;       // сумма элементов

  int comp( int i, int j ) const 
  {
    return _data[i] - _data[j];
  }

private:
  int _len;
  int *_data;

  void alloc( int );
  void dealloc();
  void realloc( int len );

  void read_raw_data( const char* fname );
  bool read_data( referer<stream> file, const char* fname );
};

}; // namespace lwml

#endif // _IVECTOR_
