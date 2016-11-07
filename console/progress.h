// Прогресс-индикатор
// lwml, (c) ltwood

#ifndef _PROGRESS_
#define _PROGRESS_

#include "lwml/base/defs.h"
#include "lwml/base/basex.h"
#include "lwml/base/refcount.h"

/*#lake:stop*/

namespace lwml {

// Исключение рендерера при получении сигнала остановки

DEF_EX_TYPE(ex_base, ex_userabort, "user abort");

// Интерфейс получателя сообщений от прегресс-индикатора.
// Отвечает за визуализацию прогресс-индикатора.

class i_progress_renderer : public interface, public refcount {
public:
  // nm - название процессаm, sz - ожидаемое число шагов.
  // Если передано отрицательное число шагов, то это означает невозможность
  // заранее оценить число шагов обработки.
  // В этом случае рекомендуется произвольным образом визуализировать
  // факт прихода сообщения up().
  virtual void start( const char* nm, int sz ) = 0;

  // Завершение процесса вычислений.
  virtual void finish() = 0;

  // Сообщение процесса о своем текущем состоянии.
  // Значение true, возвращаемое этой функцией,
  // сигнализирует о необходимости прервать обработку.
  // Аргумент st - текущее состояние процесса (от 0 до size-1).
  virtual bool up( int st ) = 0;
};

// Служба вывода сообщений о прогрессе вычислений.
// Пользователь этой службы может также получить сигнал
// о необходимости прервать обработку.

class progress : public scope {
public:

  // Сообщения о прогрессе вычислений, описание см. в i_progress_renderer

  static void start( const char* nm, int sz ) { if( _pr.is_ok() ) _pr->start(nm, sz); }

  static void finish() { if( _pr.is_ok() ) _pr->finish(); }

  static void up( int st ) {
    if( _pr.is_ok() ){
      if( _pr->up(st) )
        throw ex_userabort();
    }
  }

  // Зарегистрировать получателя сообщений о прогрессе вычислений.
  static void set_renderer( referer<i_progress_renderer> pr ){ _pr = pr; }

private:
  static referer<i_progress_renderer> _pr;
};

}; // namespace lwml

#endif // _PROGRESS_
