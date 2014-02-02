// Вычисление спектра мощности
// lwml, (c) ltwood

#ifndef _PWSPEST_
#define _PWSPEST_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"
#include "sliding.h"

/*#lake:stop*/

namespace lwml {

// Односторонний несглаженный спектр мощности вещественного сигнала.
// При конструировании указывается предельная длина обрабатываемых данных.
// Преобразование производит функция put().
// Данные дополняются нулями до ближайшей сверху к длине, указанной
// при конструировании, целой степени двойки.
// При этом для типа vector происходит преобразование всего содержимого вектора,
// а для типа sliding происходит преобразование указанного окна.
// После преобразования объект содержит спектр.

class pwspest : public value {
public:
  pwspest( int len );

  // Произвести обработку данных.
  void put( const vector& );
  void put( const sliding& src, int idx );

  // Получить количество спектральных отсчетов.
  int len() const { return _splen / 2 + 1; }

  // Возвращает мощность спектральной составляющей.
  real operator[]( int idx ) const;

  // Получить все спектральные отсчеты спектра мощности
  // в массив достаточно большой длины.
  void get( vector& ) const;

  // Возвращает мощность на частоте Найквиста.
  real error() const { return mag(_xr[_splen/2], _xi[_splen/2]); }

  // Возвращает полную мощность спектра.
  real sum() const;

  // Возвращает нормализованную частоту, соответствующую
  // указанному спектральному отсчету.
  real freq( int idx )   const { return real_cast(idx) / real_cast(_splen); }

  // Возвращает нормализованный период, соответствующий
  // указанному спектральному отсчету.
  real period( int idx ) const { return real_cast(_splen) / real_cast(idx); }

  // Значение шага по нормализованной частоте.
  real delta_freq()      const { return 1.0 / real_cast(_splen); }

private:
  int _dlen;     // максимальная длина данных
  int _splen;    // длина спектра
  vector _xr, _xi;
};

inline real pwspest::operator[]( int idx ) const {
  test_index(idx, len());
  if( idx == 0 || idx == len()-1 )
    return mag(_xr[idx], _xi[idx]);
  else
    return 2.0 * mag(_xr[idx], _xi[idx]);
}

}; // namespace lwml

#endif // _PWSPEST_
