// ������� �������� ����������� ����������� ������.
// lwml, (c) ltwood

#ifndef _DEBUG_
#define _DEBUG_

#include "defs.h"
#include "cstrng.h"
#include "i_stream.h"
#include "platforms.h"

/*#lake:stop*/

namespace lwml {

// �������� ������ ���������� ���������

class zzz_ctx_holder : public value {
public:
  zzz_ctx_holder( const char* ctx );
  ~zzz_ctx_holder();
};

#define zzz_ctx( nm ) zzz_ctx_holder zzz_ctx_object(nm)

// ������� ����������� ������ � ������ ��� ���������� ���� ����.
// ���� ���� ����������� �� ������ ��� ������ ������� �������.

void zzz_ex_v( const char* asp, const char* msg, va_list va );
void zzz_ex( const char* asp, const char* msg, ... ) PRINTF(2,3);
void zzz( const char *fmt, ... ) PRINTF(1,2);

// ������� zzz_dump() ��������� ������������ ����� �������� llogsrv ��� ����������
// ���������� ������� ���������� �������� ������ ���������� � ����� ������.
// ������� ���������� ������, ���� ������� ��������� ���������� llogsrv
// � � �������� ���������� ���������� ������������ ���������� dump.
// ������ �������� ���������� ������������� ����������� �������������� ���������
// ���� ��� ������ �����.

bool zzz_dump();

// ������� zzz_dump_name() �� �������������� ����� �����
// (�.�. ����� � ���������� ��� ����) �������� ������ ��� ����� �����

strng zzz_dump_name( const char* fmt, ... ) PRINTF(1,2);

// ���������� ������ ������ ������ ������ ����������� ������

class zstream : public i_stream, public value {
public:
  zstream( const char* asp = 0 ) : _asp( asp ? asp : "") {}

  // ����� ������
  virtual void out( const strng& s );

  // ������������ ������
  virtual void flush() {}

private:
  cstrng _asp;
};

}; // namespace lwml

#endif // _DEBUG_
