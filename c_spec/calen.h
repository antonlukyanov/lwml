// ��������� - �������� � ������ �������������� ���������
// lwml, (c) ltwood

#ifndef _CALEN_
#define _CALEN_

#include "defs.h"
#include "cstrng.h"

/*#lake:stop*/

namespace lwml {

// �������� � ������ �������������� ���������
// ������������� ������:
//   1583 <= year
//      1 <= month    <= 12
//      1 <= day      <= 28 | 29 | 30 | 31
//      1 <= week_day <= 7

const int START_YEAR = 1583;

class date : public value {
public:
  date() { set(1970, 1, 1); }
  date( int y, int m, int d ) { set(y, m, d); }

  // ���������� �������� ����
  void set( int y, int m, int d );

  int year() const { return _year; }    // �������� ����
  int month() const { return _month; }  // �������� �����
  int day() const { return _day; }      // �������� ����

  cstrng repr() const;  // �������� ��������� ������������� ����

  date& operator++();        // �������� ���� ������ �� 1 ����
  date& operator--();        // �������� ���� ����� �� 1 ����

  date& operator+=( int d ); // �������� ���� ������ �� d ����
  date& operator-=( int d ); // �������� ���� ����� �� d ����

  // ���������, ����� �� ����������� ������� ����, ��� ���� d
  bool is_later_than( const date& d ) const;

  // �������� ���� ������, ��������������� ����
  int weekday();

  // ����������� ������ ��� ������ � ������

  static bool is_leap_year( int year ); // �������� �� ��� ����������

  static int days_in_year( int year );             // ����� ���� � ����
  static int days_in_month( int month, int year ); // ����� ���� � ������

  // ���������� (� ����) ����� ����� ������ (date2 > date1)
  static int days_between( const date& date1, const date& date2 );

  // �������� ��������� ���� ������
  static int next_weekday( int wd ) { return wd % 7 + 1; }

private:
  int _day;
  int _month;
  int _year;

  static bool is_correct_year( int year );   // �������� ������������ ����
  static bool is_correct_month( int month ); // �������� ������������ ������
  static bool is_correct_weekday( int wd );  // �������� ������������ ��� ������
  static bool is_correct_date( int y, int m, int d ); // �������� ������������ ����

  // ���������� (� ����) �� ������ ���� y1 �� ������ ���� y2 (y1 <= y2)
  static int days_between_years( int y1, int y2 );

  // ���������� (� ����) �� ������ ���� �� ����
  static int days_from_ny( const date& d );
};

}; // namespace lwml

#endif // _CALEN_
