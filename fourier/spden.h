#ifndef _SPDEN_
#define _SPDEN_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"

/*#lake:stop*/

namespace lwml {

// Вычисление сглаженной оценки одностороннего спектра.
// При конструировании указывается длина окна, длина хвоста из нулей и сдвиг окна.
// По умолчанию длина хвоста из нулей равна нулю, а сдвиг окна
// равен половине длины окна (сингулярная оценка спектрльной плотности).
// Преобразование происходит при применении к данным метода put().
// После преобразования объект содержит спектр.

class spdenest : public value {
  friend class spacorr;

public:
  spdenest( int winlen, int uplen = 0, int winshift = 0 );

  // Обработать данные.
  void put( const vector& src );

  // Получить число окон, по которым был вычислен спектр.
  int win_num() const { return _win_num; }

  // Получить количество спектральных отсчетов.
  int len() const { return _len; }

  // Возвращает мощность спектральной составляющей.
  real operator[]( int idx ) const;

  // Получить все спектральные отсчеты спектра
  // мощности в массив достаточно большой длины.
  void get( vector& ) const;

  // Получить мощность на частоте Найквиста.
  real error() const { return _p[_len-1]; }

  // Возвращает полную мощность спектра.
  real sum() const   { return _p.inner_sum(); }

  // Возвращает нормализованную частоту, соответствующую
  // указанному спектральному отсчету.
  real freq( int idx )   const { return real_cast(idx) / real_cast(_splen); }

  // Возвращает нормализованный период, соответствующий
  // указанному спектральному отсчету.
  real period( int idx ) const { return real_cast(_splen) / real_cast(idx); }

  // Дает значение шага по нормализованной частоте.
  real delta_freq()      const { return 1.0 / real_cast(_splen); }

private:
  int _winlen;    // длина окна
  int _uplen;     // длина хвоста из нулей
  int _splen;     // длина спектра
  int _winshift;  // сдвиг окна
  int _len;       // длина спектра мощности
  vector _p;      // спектр мощности
  int _win_num;
};

inline real spdenest::operator[]( int idx ) const {
  test_index(idx, _len);
  return _p[idx];
}

class spacorr : public value {
public:
  spacorr( const spdenest& );

  void operator()( const spdenest& );

  int len() const { return _clen; }

  real operator[]( int idx ) const;

private:
  int _splen;
  int _clen;
  vector _xr, _xi;
};

inline real spacorr::operator[]( int idx ) const {
  test_index(idx, len());
  return _xr[idx];
}

}; // namespace lwml

#endif // _SPDEN_
