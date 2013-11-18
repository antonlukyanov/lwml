#include "calen.h"

#include "basex.h"

/*#lake:stop*/

namespace lwml {

bool date::is_correct_year( int year )
{
  return year >= START_YEAR;
}

bool date::is_correct_month( int month )
{
  return 1 <= month && month <= 12;
}

bool date::is_correct_weekday( int wd )
{
  return 1 <= wd && wd <= 7;
}

bool date::is_leap_year( int year )
{
  if( !is_correct_year(year) )
    runtime("incorrect year (%d)", year);
  return (year % 100 != 0 && year % 4 == 0) || year % 400 == 0;
}

int date::days_in_year( int year )
{
  if( !is_correct_year(year) )
    runtime("incorrect year (%d)", year);
  return is_leap_year(year) ? 366 : 365;
}

int date::days_in_month( int month, int year )
{
  if( !is_correct_year(year) || !is_correct_month(month) )
    runtime("incorrect month or year (%d.%d)", year, month);

  switch( month ){
    case 4:         // April
    case 6:         // June
    case 9:         // September
    case 11:        // November
      return 30;
    case 2:         // February
      if( is_leap_year(year) )
        return 29;
      else
        return 28;
    default:        // January, March, May, July, August, October, December
      return 31;
   }
}

bool date::is_correct_date( int y, int m, int d )
{
  return is_correct_year(y) && is_correct_month(m) &&
         1 <= d && d <= days_in_month(m, y);
}

void date::set( int y, int m, int d )
{
  if( !is_correct_date(y, m, d) )
    runtime("incorrect date (%d.%d.%d)", y, m, d);
  _year = y;
  _month = m;
  _day = d;
}

// ������ ������ ��� ������ ����
// ������ ������ ������� ������, ������� ����� ��� ����������� ����.

const int DATE_BUFLEN = 64;

cstrng date::repr() const
{
  // ��������� ����� ������-����������
  char buf[DATE_BUFLEN]; // dd.mm.yyyy\0

  prot_sprintf(buf, DATE_BUFLEN, "%02d.%02d.%d", _day, _month, _year);
  return strng(buf);
}

date& date::operator++()
{
  // � �������� ������ ���������� ������ ���������� �� ����
  if( _day != days_in_month(_month, _year) )
    ++_day;
  // � ����� ������������ ������ ��������� �� 1-� ����� ���������� ������
  else if( _month != 12 ){
    ++_month;
    _day = 1;
  // � ��������� ���� ���� ��������� �� ������ ���� ���������� ����
  }else{
    ++_year;
    _month = 1;
    _day = 1;
  }
  return *this;
}

date& date::operator--()
{
  // � �������� ������ ���������� ������ ���������� �� ����
  if( _day != 1 )
    --_day;
  // � ������ ��������� ������ ��������� �� ��������� ����� ����������� ������
  else if( _month != 1 ){
    --_month;
    _day = days_in_month(_month, _year);
  // � ������ ���� ��������� �� ��������� ���� ����������� ����
  }else{
    --_year;
    _month = 12;
    _day = 31; // � ������� ������ 31 ����
  }
  return *this;
}

date& date::operator+=( int d )
{
  if( d == 0 )
    return *this;

  if( d < 0 )
    return operator-=(-d);

  // ��������� � ������ �������� ���� �������� (��������) ���
  // ��� ����� ������� �������� �� ���� <dd.mm.yyyy> � ���� <dd.01.yyyy>
  for( int mn = 1; mn < _month; mn++ )
    d += days_in_month(mn, _year);
  _month = 1;
  // � ������ � � ���� <01.01.yyyy>
  d += _day;
  _day = 1;

  // ����������� ������ ����, ������� ���
  while( d > days_in_year(_year) ){
    d -= days_in_year(_year);
    ++_year;
  }

  // ����������� ������ ������ ������� ���
  while( d > days_in_month(_month, _year) ){
    d -= days_in_month(_month, _year);
    ++_month;
  }

  // ������ �������� ������ ���
  _day = d;

  return *this;
}

date& date::operator-=( int d )
{
  if( d == 0 )
    return *this;

  if( d < 0 )
    return operator+=(-d);

  // ��������� � ����� �������� ���� �������� (��������) ���
  // ��� ����� ������� �������� �� ���� <dd.mm.yyyy> � ���� <dd.12.yyyy>
  for( int mn = _month + 1; mn <= 12; mn++ )
    d += days_in_month(mn, _year);
  _month = 12;
  // � ������ � � ���� <31.12.yyyy>
  d += days_in_month(_month, _year) - _day;
  _day = 31;

  // ����������� ������ ���� ������� ���
  while( d > days_in_year(_year) ){
    d -= days_in_year(_year);
    --_year;
  }

  // ����������� ������ ������ ������� ���
  while( d > days_in_month(_month, _year) ){
    d -= days_in_month(_month, _year);
    --_month;
  }

  // ������ �������� ������ ���
  _day = days_in_month(_month, _year) - d;

  return *this;
}

bool date::is_later_than( const date& d ) const
{
  if( _year > d.year() )
    return true;
  else if( _year == d.year() && _month > d.month() )
    return true;
  else if( _year > d.year() && _month == d.month() && _day > d.day() )
    return true;

  return false;
}

int date::days_between_years( int y1, int y2 )
{
  int days = 0;
  for( int y = y1; y < y2; y++ )
    days += days_in_year(y);
  return days;
}

int date::days_from_ny( const date& d )
{
  int days = 0;
  for( int m = 1; m < d.month(); m++ )
    days += days_in_month(m, d.year());
  return days + d.day();
}

int date::days_between( const date& date1, const date& date2 )
{
  int yd = days_between_years(date1.year(), date2.year());
  return yd - days_from_ny(date1) + days_from_ny(date2);
}

// ����� ������������ ������������ ����� ������� - 01.01.1996 (��),
// �� ����� ����� ���� ������ ������������
int date::weekday()
{
  date d0(1996, 1, 1);

  if( is_later_than(d0) )
    return days_between(d0, *this) % 7 + 1;
  else
    return (7 - days_between(*this, d0) % 7) % 7 + 1;
}

}; // namespace lwml
