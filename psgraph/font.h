// ��������� CHR-������

#ifndef _FONT_
#define _FONT_

#include "defs.h"
#include "canvas.h"
#include "refcount.h"

/*#lake:stop*/

// ����� ������������ ��������� ��������� ��������
// �� CHR-������ � ����������� ����������� �����
// ������� ����� ����������� � ������ ���������.
// ����� width() ��������� ��������� ������ ������ ��������
// �� �������� ������ ������
// ������ puts() ������� ������ � ������� �������
// ������� �������� ������
// ����������� ����� �������������� ������������ (��. canvas.h)
// � ���� �������� ������ (������� �������������� ������������
// ����� ��������, ���������� �������������)

namespace lwml {

class font : public value {
public:
  font();

  real width( const char *str, real height ) const;

  void puts(
    referer<i_canvas> pcv,
    real x, real y, const char *str, real height,
    const align& ta = align(), real alpha = 0.0
  );

private:
  int  _top, _bot;   // ������ ������ � �������� ������
  real _x0, _y0;     // ����� �������� ������
  real _cx, _cy;     // ������� ����� ���������

  real mult( real height ) const;

  void moveto( real x, real y );
  void lineto( referer<i_canvas> pcv, real x, real y );

  real width( char, real height ) const;

  void put_char( referer<i_canvas> pcv, char ch, real height, real alpha );
};

}; // namespace lwml

#endif // _FONT_
