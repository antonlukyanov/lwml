// ������������ endian-��������������� �����.
// lwml, (c) ltwood

#ifndef _ENDIANS_
#define _ENDIANS_

#include "defs.h"

/*#lake:stop*/

namespace lwml {

// ��������� ����������� ������� ��� ������ � ��������� �������,
// ����������� �� �������� � ���������� ������������ � ������� ������.
// ������������ ��� ������������ �������� �������� ������.

// ������ �������������� 2 �������� ������� ������������ ������ ���
// ������ � ������: Big Endian (BE) � Little Endian (LE) � ��� ����
// ������������� ������������ ������������� �� ������� ����� DEC VAX
// ������ Middle Endian (ME).
// ���������� intel ���������� ������� Little Endian,
// � ���������� motorola - Big Endian.
// ��� Big Endian ������������ �� ������� ������� ������ ������������
// �������� �������� �����, � ��� Little Endian - ��������.
// ������� Xe2Ye_nn ������������ ����� �� ������������ X � ������������ Y.
// ������� to_Xe_nn ������������ ����� �� �������� ������������ � ������������ X.
// ������� from_Xe_nn ������������ ����� �� ������������ X � ������� ������������.
// ������� nn ���������� ����� ����� � �������������� �����.

class endian : public scope {
public:
  // ��������������� 32-������ �����
  static uint32 le2be_32( uint32 );
  static uint32 be2le_32( uint32 );
  static uint32 le2me_32( uint32 );
  static uint32 me2le_32( uint32 );
  static uint32 me2be_32( uint32 );
  static uint32 be2me_32( uint32 );

  static uint32 to_le_32( uint32 );
  static uint32 to_be_32( uint32 );
  static uint32 to_me_32( uint32 );

  static uint32 from_le_32( uint32 );
  static uint32 from_be_32( uint32 );
  static uint32 from_me_32( uint32 );

  // ��������������� 16-������ �����
  static uint16 le2be_16( uint16 x ) { return swab(x); }
  static uint16 be2le_16( uint16 x ) { return swab(x); }
  static uint16 le2me_16( uint16 x ) { return x; }
  static uint16 me2le_16( uint16 x ) { return x; }
  static uint16 me2be_16( uint16 x ) { return swab(x); }
  static uint16 be2me_16( uint16 x ) { return swab(x); }

  static uint16 to_le_16( uint16 );
  static uint16 to_be_16( uint16 );
  static uint16 to_me_16( uint16 );

  static uint16 from_le_16( uint16 );
  static uint16 from_be_16( uint16 );
  static uint16 from_me_16( uint16 );

private:
  static uint16 swab( uint16 );
};

}; // namespace lwml

#endif // _ENDIANS_
