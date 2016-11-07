// FFT for real matrix
// lwml, (c) ltwood

#ifndef _RFFT2D_
#define _RFFT2D_

#include "lwml/base/defs.h"
#include "lwml/m_base/mdefs.h"

#include "lwml/m_types/matrix.h"

/*#lake:stop*/

namespace lwml {

// Прямое и обратное двумерное быстрое преобразование Фурье для вещественной матрицы.
// Реализован расчет с замещением, когда спектр помещается на место исходных данных.
// Порядок отсчетов в двумерном спектре общего вида описан в fft2d.h.
// Спектр размещается в исходной вещественной матрице специфическим образом:
// 1. Вычисляются и сохраняются только отсчеты спектра X[n][m] для
//    0 <= n <= N/2, 0 <= m < M (т.е. по строкам сохраняется половина спектра).
//    Остальные отсчеты могут быть получены из следущих соотношений:
//    X[N - n][m] = X[-n][m], (справедливо всегда, см. fft2d.h)
//    X[-n][-m] = X*[n][m] (справедливо для вещественной матрицы).
// 2. Для 0 < n < N/2, 0 <= m < M (т.е. для всех n кроме 0 и N/2)
//    отсчеты спектра X[n][m] сохраняются в матрице следующим образом:
//    matr[2*n][m] = re(X[n][m]), matr[2*n+1][m] = im(X[n][m]).
// 3. Для n=0 и n=N/2 отсчеты спектра X[n][m] сохраняются в первых
//    двух строках матрицы следующим образом:
//      j          : 0       1     ... M/2-1     M/2       ... M-2   M-1
//      matr[0][j] : Xr[0]   Xr[1] ... Xr[M/2-1] Yr[M/2-1] ... Yr[1] Yr[0]
//      matr[1][j] : Xr[M/2] Xi[1] ... Xi[M/2-1] Yi[M/2-1] ... Yi[1] Yr[M/2]
//    где
//      Xr[j] = re(X[0][j]),
//      Xi[j] = im(X[0][j]),
//      Yr[j] = re(X[N/2][j]),
//      Yi[j] = im(X[N/2][j]).

class rfft2d : public scope {
public:
  // Прямое двумерное быстрое преобразование Фурье вещественной матрицы.
  // Описание ядра и нормировки см. fft2d::cfft().
  // Аргумент tick позволяет включить вывод сообщений и прогрессе.
  static void rcfft( matrix& m, tick_mode tick = tmOFF );

  // Обратное двумерное быстрое преобразование Фурье спектра вещественной матрицы.
  // Описание ядра и нормировки см. fft2d::cifft().
  // Аргумент tick позволяет включить вывод сообщений и прогрессе.
  static void crifft( matrix& m, tick_mode tick = tmOFF );
};

}; // namespace lwml

#endif // _RFFT2D_
