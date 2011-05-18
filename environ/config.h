// ���������������� ����� ����������
// lwml, (c) ltwood

#ifndef _CONFIG_
#define _CONFIG_

#include "defs.h"

#include "luaconfig.h"

/*#lake:stop*/

namespace lwml {

// ������� ��������� ������ �� ���������������� ���� ����������.
// ���� � ������ config ������ ������� � ������� ��������, ����� � �������� ���������� ����������.

referer<luaconf> config();

// ��������������� ����������������� �����.
// ���������� ����� �������������� ������.
// ���� ������� ������� ��� ����������, �� ����� ����������������� ����� 
// ������������ ������� � ������� ��������, ����� � �������� ���������� ����������.
// ���� � ��������� ������� ���� � �����, �� ������������ ������� ������������
// ���� ���� � �������� �����������������.
// ������ ������� ���������������� ������������ �� ����������� ������,
// ����������� ��� ���������������� ����.

void reset_config( const char* fname = 0 );
void reset_config( const char* cfg, int len );

}; // namespace lwml

#endif // _CONFIG_
