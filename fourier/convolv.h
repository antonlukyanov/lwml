// Реализация свертки в спектральной области

#ifndef _CONVOLV_
#define _CONVOLV_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/m_types/vector.h"

/*#lake:stop*/

namespace lwml {

// класс реализует спектральный алгоритм вычисления свертки
// При конструировании указывается максимальная длина обрабатываемого сигнала
// и отдельно части импульсной характеристики, соответствующие
// неотрицательным и отрицательным значениям индекса
// Второй конструктор принимает половину импульсной характеристики,
// соответствующую только неотрицательным значениям индекса
// в предположении ее симметричности
// Свертку осуществляет оператор (), который сворачивает вектор src
// с указанной при конструировании ИХ и кладет результат в dst
// Векторы src и dst должны быть одинакового размера

class convolver : public value {
public:
  convolver( int len, const vector& hp, const vector& hn );
  convolver( int len, const vector& hp );

  void operator()( vector& dst, const vector& src );

private:
  int _uplen;   // буфер против затягивания
  int _dlen;    // максимальная длина данных
  int _splen;   // полный размер массивов
  vector _hr, _hi;
  vector _xr, _xi;
};

// класс реализует спектральный алгоритм оценивания кросскорреляции
// Вычисляемая кросскорреляция отличается от классического определения
// отсутствием нормирующего множителя (1/N)
// При конструировании указывается максимальная длина обрабатываемого сигнала
// и длина clen вычисляемой оценки ККФ
// При этом вычисляются отсчеты ККФ при -clen <= j <= clen
// Метод len() возвращает длину хвоста из неотрицательных отсчетов ККФ
// Вычисление ККФ реализует оператор (), получающий на вход массивы исходных данных
// Отсчеты ККФ можно получить, вызвав метод []

class crosscorr : public value {
public:
  crosscorr( int len, int clen );

  int len() const { return _clen + 1; }

  void operator()( const vector& src1, const vector& src2 );

  real operator[]( int idx ) const;

private:
  int _dlen;  // максимальная длина данных
  int _clen;  // максимальная длина корреляционной функции
  int _splen; // полный размер массивов
  vector _xr, _xi;
  vector _xxr, _xxi;
};

inline real crosscorr::operator[]( int idx ) const {
  test_index(int_cast(labs(idx)), len());
  return (idx >= 0) ? _xr[idx] : _xr[_splen+idx];
}

// класс реализует спектральный алгоритм оценивания автокорреляции
// При конструировании указывается максимальная длина обрабатываемого сигнала
// и длина clen вычисляемой оценки ККФ
// При этом вычисляются отсчеты ККФ при -clen <= j <= clen
// Метод len() возвращает длину хвоста из неотрицательных отсчетов ККФ
// Вычисление ККФ реализует оператор (), получающий на вход массивы исходных данных
// Отсчеты ККФ можно получить, вызвав метод []

class autocorr : public value {
public:
  autocorr( int len, int clen );

  int len() const { return _clen + 1; }

  void operator()( const vector& src );

  real operator[]( int idx ) const;

private:
  int _dlen;    // максимальная длина данных                
  int _clen;    // максимальная длина корреляционной функции
  int _splen;   // полный размер массивов                   
  vector _xr, _xi;
};

inline real autocorr::operator[]( int idx ) const {
  test_index(int_cast(labs(idx)), len());
  return _xr[labs(idx)];
}

}; // namespace lwml

#endif // _CONVOLV_
