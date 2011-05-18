// Календарь - операции с датами григорианского календаря
// lwml, (c) ltwood

#ifndef _CALEN_
#define _CALEN_

#include "defs.h"
#include "cstrng.h"

/*#lake:stop*/

namespace lwml {

// Операции с датами григорианского календаря
// Представление данных:
//   1583 <= year
//      1 <= month    <= 12
//      1 <= day      <= 28 | 29 | 30 | 31
//      1 <= week_day <= 7

const int START_YEAR = 1583;

class date : public value {
public:
  date() { set(1970, 1, 1); }
  date( int y, int m, int d ) { set(y, m, d); }

  // установить значение даты
  void set( int y, int m, int d );

  int year() const { return _year; }    // получить года
  int month() const { return _month; }  // получить месяц
  int day() const { return _day; }      // получить день

  cstrng repr() const;  // получить текстовое представление даты

  date& operator++();        // изменить дату вперед на 1 день
  date& operator--();        // изменить дату назад на 1 день

  date& operator+=( int d ); // изменить дату вперед на d дней
  date& operator-=( int d ); // изменить дату назад на d дней

  // проверить, позже ли расположена текущая дата, чем дата d
  bool is_later_than( const date& d ) const;

  // получить день недели, соответствующий дате
  int weekday();

  // статические методы для работы с датами

  static bool is_leap_year( int year ); // является ли год високосным

  static int days_in_year( int year );             // число дней в году
  static int days_in_month( int month, int year ); // число дней в месяце

  // промежуток (в днях) между двумя датами (date2 > date1)
  static int days_between( const date& date1, const date& date2 );

  // получить следующий день недели
  static int next_weekday( int wd ) { return wd % 7 + 1; }

private:
  int _day;
  int _month;
  int _year;

  static bool is_correct_year( int year );   // проверка корректности года
  static bool is_correct_month( int month ); // проверка корректности месяца
  static bool is_correct_weekday( int wd );  // проверка корректности дня недели
  static bool is_correct_date( int y, int m, int d ); // проверка корректности даты

  // промежуток (в днях) от начала года y1 до начала года y2 (y1 <= y2)
  static int days_between_years( int y1, int y2 );

  // промежуток (в днях) от начала года до даты
  static int days_from_ny( const date& d );
};

}; // namespace lwml

#endif // _CALEN_
