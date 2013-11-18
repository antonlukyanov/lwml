// ����������� ��������� ������ ������
// lwml, (c) ltwood

#ifndef _I_STREAM_
#define _I_STREAM_

#include "defs.h"
#include "cstrng.h"

/*#lake:stop*/

namespace lwml {

// ����� ���������� ��������� ���������� ������ ������.
// � ����� ����� ������� �������� ��������� �������, ��������� � ���� �����������.

class i_stream : public interface {
public:
  virtual void out( const strng& ) = 0;     // ����� ������
  virtual void flush() = 0;                 // ������������ ������

  // ����� ������
  i_stream& operator<<( const cstrng& s ) { 
    out(s);
    return *this;
  }
};

// ���������� ������ ������ � ������

class sstream : public i_stream, public value {
public:
  // ����� ������
  virtual void out( const strng& s ){ _str.concat(s); }

  // ������������ ������
  virtual void flush() {}

private:
  cstrng _str;
};

}; // namespace lwml

#endif // _I_STREAM_
