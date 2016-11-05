// ��������� ���������� limlib
// lwml, (c) ltwood

#ifndef _LIMLIB_
#define _LIMLIB_

#include "defs.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

// ������� ��� ����������� limlib.dll, ��������������
// �������� ����������� ����������� � �������� tiff � jpeg
// � ���������� ������������ ����������� � ������� jpeg.
// ������������ �������������� �������� ���������� ��� ������ ������.
// ���������� �������� ����� ��������� �������� is_active(),
// ������� ��� ������������� ���� �������� ��������� ����������.
// ���� ���������� �� �����������, �� ����� ����� ������� ���������� assert.
// ����� �� ������������ ������, ������������ �����������.

class limlib : public scope {
public:
  static bool is_active() { return init(); }

  static bool getsize( const char* fn, int& lx, int& ly );
  static bool load( const char* fn, uchar* data );
  static bool load_rgb( const char* fn, uchar* data );
  static bool save( const char* fn, int lx, int ly, const uchar* data );

  static const char* errmsg();

private:
  static bool init();
  static void test();
};

}; // namespace lwml

#endif // _LIMLIB_
