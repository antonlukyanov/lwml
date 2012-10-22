// Поддержка многопоточности
// Содержит классы, инкапсулирующие процедуры запуска потоков и
// их синхронизации системно-независимым образом
// lwml, (c) ltwood

#ifndef _MTHREAD_
#define _MTHREAD_

#include "defs.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// locker под win32 реализуется как lightweight mutex (aka critical section).
// Позволяет пользователям захватывать ресурс монопольно.

class locker : public value {
public:
  // Создать флаг занятости ресурса с заданным начальным состоянием
  locker();

  ~locker();

  // Захватить ресурс
  void lock();

  // Попытаться захватить ресурс
  // Возвращает флаг удачи.
  bool test();

  // Освободить ресурс
  void unlock();

private:
  void* _hnd;
};

// Класс event реализует событие.
// Реализован частный случай, когда событие будит только
// один ожидающий его поток и сразу же автоматически сбрасывается.
// При этом если на момент наступления события ни один поток не ожидает
// его наступления, то событие остается во взведенном состоянии до
// появления такого потока.
// Вызов метода wait() усыпляет поток до наступления события.
// Метод set() взводит флаг события.

class event : public value {
public:
  enum state { ON, OFF };

  event( state st = ON );
  ~event();

  void set();
  void wait();

private:
  void* _hnd;
};

// Класс intex реализует безопасное взаимодействие потоков через
// разделяемую ими целочисленную переменную.
// Над разделяемой переменной можно безопасно
// выполнять операции инкремента inc() и dec().
// Текущее значение переменной можно получить вызвав метод get().
// Метод set() устанавливает новое значение переменной
// Методы inc() и dec() возвращают целое, знак которого совпадает
// со знаком переменной после выполнения операции.
// Метод set() возвращает предыдущее значение переменной.

class intex : public value {
public:
  intex( int32 val = 0 ) { _data = val; }

  int32 inc();
  int32 dec();
  int32 set( int32 val );

  int32 get() const { return _data; }

private:
  int32 _data;
};

// Обертка для независимо выполняемого потока.
// Клиентский код предоставляет реализацию интерфейса i_thread_function,
// содержащего метод func(), код которого выполняется создаваемым потоком.
// Поток создается в приостановленном состоянии и должен явно запускаться.

class i_thread_function : public interface, public refcount {
public:
  virtual void func() = 0;
};

class thread : public value {
public:
  thread( referer<i_thread_function> );
  ~thread();

  void start();     // Запуск потока
  void kill();      // Принудительно завершает поток
  bool is_active(); // Проверяет, выполняется ли поток в данный момент
  void wait();

private:
  referer<i_thread_function>    _thr_func;
  void*                         _hnd;
  unsigned                      _th_id;
  bool                          _not_yet_started;

  static unsigned __stdcall code( void* ptr );
  // Интересно, все ли компиляторы понимают спецификацию __stdcall
  // Как минимум в MinGW-GCC и VC-7 это работает...
};

}; // namespace lwml

#endif // _MTHREAD_
