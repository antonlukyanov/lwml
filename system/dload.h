// ������������ �������� ���������
// lwml, (c) ltwood

#ifndef _DLOAD_
#define _DLOAD_

#include "defs.h"
#include "mdefs.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

// ���������� ��� ��������� �������� ����������

DEF_EX_TYPE_MSG(ex_base, ex_dll, "can't load proc from dll");

// ���������, ����������� �� ������������ ����������
typedef int (*dproc)();

// �������� ������������ ����������.
// ��� ��������� �������� ���������� 0.
void* dl_load( const char* );

// �������� ��������� �� ����� ����������� ����������.
// ��� ������� ���������� ���������� assert,
// ��� ��������� �������� ���������� ���������� ex_dll.
dproc dl_proc( void*, const char* );

// ������ ��� �������� ������� ��������� ����

#define LOAD_PROC(dll_ptr, proc_name, proc_type) \
  reinterpret_cast<proc_type>(dl_proc(dll_ptr, proc_name));

}; // namespace lwml

#endif // _DLOAD_
