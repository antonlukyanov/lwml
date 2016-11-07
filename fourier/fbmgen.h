// Генератор фрактального броуновского процесса

#ifndef _FBMGEN_
#define _FBMGEN_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"
#include "lwml/m_types/vector.h"
#include "lwml/random/frand.h"

/*#lake:stop*/

namespace lwml {

// Класс fbmgen генерирует последовательность фрактальных
// броуновских процессов заданной длины
// Задавая для объектов класса различные значения параметра seed,
// можно получать различные последовательности процессов
// Параметр len задает длину генерируемого процесса,
// но реально генерируется процесс большей длины
// Истинную длину процесса можно получить, вызвав метод len()
// Каждый вызов функции generate() приводит к генерации нового процесса
// с заданным параметром h
// Отсчеты сгенерированного процесса можно получать по одному с помощью
// оператора индексирования или целиком в массив с помощью функции get()
// Функция get() заполняет массив отсчетами процесса
// и требует, чтобы массив имел длину не более чем len()

// Параметр h задает сложность процесса
// При h=0.5 получаем гауссов броуновский процесс
// При h>0.5 приращение значения процесса обычно сохраняет знак
// и график процесса оказывается более плавным
// При h<0.5 приращение значения процесса обычно меняет знак
// и график процесса оказывается более изрезанным
// Размерность графика равна d=2-h

class fbmgen : public value {
public:
  fbmgen( int len, uint seed = 1 );

  void generate( real h = 0.5 );

  int len() const { return _len; }

  real operator[]( int j );

  void get( vector& );

private:
  int _len;
  vector _xr, _xi;
  frand _rnd;
};

inline real fbmgen::operator[]( int j ){
  test_index(j, _len);
  return _xr[j];
}

}; // namespace lwml

#endif // _FBMGEN_
