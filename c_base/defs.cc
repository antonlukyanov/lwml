#include "defs.h"

/*#lake:stop*/

namespace lwml {

// ������� strncpy(dst, src, num) �������� �� ����� num ��������,
// �������� ������ ������� ������ dst.
// ���� �������� ������ ������� num, �� ����� ����������� �����
// num �������� � �� ����� ������� ����������� ����.

char* prot_strcpy( char* dst, const char* src, int buflen )
{
  if( src ){
    strncpy(dst, src, buflen-1);
    dst[buflen-1] = 0;  // ���������� ����������� ����
  } else
    dst[0] = 0; // ��������� ������ ������� �����

  return dst;
}

// ������� snprintf ����� �������� ����� buflen �������� � ������� ����� buflen.
// � ���� ������ ���������, ��� ��������� ������������ ������ � ������� ������������
// ����� ������� �����.
// � ������� prot_vsprintf() ��� ������������ ���������� ������� ��������� ����������
// � ������������ ������� ������.
int prot_vsprintf( char* buf, int buflen, const char* fmt, va_list va )
{
  int numch;
#if defined(_MSC_VER)  
  numch = _vsnprintf(buf, buflen, fmt, va);
#else
  numch = vsnprintf(buf, buflen, fmt, va);
#endif

  if( numch >= 0 && numch < buflen )
    return numch;
  // ��� ������������ ������
  buf[buflen-1] = 0;  // ���������� ����������� ����
  return -1;          // ���������� ������� ������
}

int prot_sprintf( char* buf, int buflen, const char* fmt, ... )
{
  va_list va;
  va_start(va, fmt);
  //!! SIC: prot_vsprintf() �� ���������� ����������!
  int numch = prot_vsprintf(buf, buflen, fmt, va);
  va_end(va);
  return numch;
}

// lwml version

namespace {
  volatile char _lwml_copy[] =
    "\rlwml, ver. 8.02, (c) ltwood, 1990--2005, mailto:ltwood@gmail.com\r";

  const char _lwml_ver[] = "8.02";
};

const char* lwml_ver()
{
  _lwml_copy[0] = '\n';
  return _lwml_ver;
}

}; // namespace lwml
