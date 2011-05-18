// ���������� ����������� ����� CRC32
// lwml, (c) ltwood

#ifndef _CRC32_
#define _CRC32_

#include "defs.h"

/*#lake:stop*/

namespace lwml {

// ����� ��������� ������������ ���������� ����������� �����.
// �������� ����� ������������ ��� ��� ������ �������� ������,
// ��� � ��� ��������������� ����������� ������.

class crc32 : public value {
public:
  // ������������� ������������� ������� ����������� �����
  crc32() { _crc = static_cast<uint32>(-1); }

  // ���������� ����������� ����� �� ���������� ����� ������ �������� ������.
  void put( char ch ){
    uchar idx = static_cast<uchar>(_crc) ^ static_cast<uchar>(ch);
    _crc = CRC32table[idx] ^ (_crc >> 8);
  }

  // ���������� ����������� ����� �� ���������� ����� �������� ������
  // ptr - ��������� �� ���� ������
  // size - ������ ����� ������
  void put( const char *ptr, int size );

  // �������� ������� �������� ����������� �����
  // ���������� ������� �������� ����������� �����
  uint32 get() const { return ~_crc; }

private:
  uint32 _crc;

  static const int32 CRC32table[256];
};

}; // namespace lwml

#endif // _CRC32_
