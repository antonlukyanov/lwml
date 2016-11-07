// Доступ к базе данных вызванных потенциалов
// Для локального использования с генератором procep
// lwml, (c) ltwood

#ifndef _EPLIB_
#define _EPLIB_

#include "lwml/base/defs.h"
#include "lwml/m_types/vector.h"
#include "lwml/m_types/matrix.h"
#include "lwml/m_base/i_vset.h"
#include "lwml/types/cstrng.h"
#include "lwml/types/t_array.h"
#include "lwml/types/t_membuf.h"

/*#lake:stop*/

namespace lwml {

// Исключение при ошибке интерпретации содержимого файла

DEF_EX_TYPE_MSG(ex_integrity, ex_eplib, "error reading eplib");

// epinfo - запись базы данных вызванных потенциалов (ВП)
// Каждому ВП соответствует имя файла с данными (fname),
// имя испытуемого (name), цвет световой стимуляции (color),
// номер отведения (line), позиция первого отсчета в файле (start)
// и длина записи ВП (length)
// Функции is_name(), is_color(), is_line() проверяют равенство
// соответствующего поля переданному значению
// Функция is() проверяет равенство всех полей переданным значениям
// Функция init() инициализирует все поля записи переданными значениями

class epinfo : public value {
public:
  void init( const char *fnm, const char *nm, const char *col, int ln, int st, int len );

  strng fname() const { return _fname; }
  strng name() const { return _name; }
  strng color() const { return _color; }
  int line() const { return _line; }
  int start() const { return _start; }
  int len() const { return _length; }

  bool is_name( const char* ) const;
  bool is_color( const char* ) const;
  bool is_line( int ) const;

  bool is( const char*, const char*, int ) const;

private:
  strng _fname;
  strng _name, _color;
  int _line;
  int _start, _length;
};

// Класс реализует интерфейс базы данных вызванных потенциалов
// При конструировании указывается файл базы данных
// size() дает суммарное число ВП в базе данных
// Функции min_len() и max_len() дают длины самой короткой
// и самой длинной записи из базы данных
// Функции name(), color(), line() и len() возвращают соответственно
// имя, цвет, отведение и длину записи для ВП с переданным номером
// Функция vget() помещает в переданный вектор запись с указанным номером
// Длина переданного вектора должна быть достаточной
// Функция count() считает число ВП, удовлетворющих "шаблону"
// Функция mget() помещает ВП, удовлетворющие "шаблону" в строки матрицы
//   Шаблон определет имя испытуемого, цвет стимула и номер отведения
//   В имени испытуемого и цвете стимула метасимволу '*' соответствует
//   нулевой указатель
//   Для номера отведения метасимволу '*' соответствует значение -1

class eplib : public value {
public:
  eplib( const char *libname );

  int size() const { return _size; }
  int min_len() const { return _minlen; }
  int max_len() const { return _maxlen; }

  strng name( int k ) const {
    test_index(k, _size);
    return _info[k].name();
  }
  strng color( int k ) const {
    test_index(k, _size);
    return _info[k].color();
  }

  int line( int k ) const {
    test_index(k, _size);
    return _info[k].line();
  }
  int len( int k ) const {
    test_index(k, _size);
    return _info[k].len();
  }

  void vget( vector& dst, int num );

  int  count( const char *name, const char* color, int line );
  void mget( matrix& dst, const char *name, const char *color, int line );

private:
  int                   _size;
  t_array<epinfo>       _info;
  int                   _maxlen, _minlen;
  t_membuf<int16>       _idata; // internal data representation

  void setminmaxlen();
  void seek( int num );
};

// Класс реализует интерфейс i_vector_set
// через концепцию прямоугольного окна, наложенного на матрицу
// В строках матрицы размещены отобранные записи ВП
// и окно синхронно вырезает из всех ВП участки с одинаковым
// запаздыванием по отношению к стимулу
// При конструировании передается матрица, в строках которой
// размещены ВП и длина окна
// Метод seek() перемещает окно в указанную позицию

class epwindow : public i_vector_set, public hardcore {
public:
  epwindow( const matrix& src, int len );

  virtual int len() const { return _src->str(); }
  virtual int dim() const { return _len; }
  virtual real get( int j, int d ) const {
    return (*_src)(j, _pos + d);
  }
  virtual void get( vector& v, int j ) const {
    _src->get_str(v, j);
  }

  void seek( int pos );

private:
  int _len;             // длина окна
  const matrix *_src;   // матрица-источник
  int _pos;             // позиция окна
};

}; // namespace lwml

#endif // _EPLIB_
