// ����������� ��������� ������
// lwml, (c) ltwood

#ifndef _STDMEM_
#define _STDMEM_

#include "defs.h"
#include "basex.h"
#include "platforms.h"

/*#lake:stop*/

namespace lwml {

// �������� ������

DEF_EX_TYPE(ex_base, ex_mem, "no more memory");

// ANSI-C ������������ ��������� ������� ���������� �������:
//   malloc(0) = 0
//   free(0) = <do nothing>
//   realloc(0, size) = malloc(size)
//   realloc(mem, 0) = free(0), and returns 0

// ����� mem �������� ����������� �������, ����������
// ����������� ��������� ������� ��������� ������.
// ������������ ������� �� ��������� ������� - � ���, ��� �����
// ������������ �������� ���������� � ��� ������ ������������
// ���������� ��� lwml ���������� lex_mem ������ bad_alloc.
// �������������� ��� �������� ������������ ���������, ����������
// � �������� �������� ���������, ���������� ��� ���������.
// ��� -- ������� ��������� ����� ������ ������ ������� ������.
// �������� �������������� -- ������������� ���������� ��������� ������
// � ���� ����������� ����������� ��������� � ������������ ������ ��� �������.

class mem : public scope {
public:
  static void* alloc( int );
  static void  free( void* );
  static void* realloc( void*, int );

  static char* strdup( const char* );

  // ������� vform() ������������ ������ �� ������ ������� � ������ ����������.
  // ������������� ���� ������� ���������� ������� vprintf().
  // ������� ������������� ��������� ����� ����������� ����� � ���������� ��������� �� ����.
  // ���� ����� ���������� ����� ���������� ������� ������� free().
  static char* vform( const char* fmt, va_list va );
  static char* form( const char* fmt, ... ) PRINTF(1,2);
};

// ������� ��������� ����� ������ ����� �������� ������.
// ����� ��������� ������ ������������ �������������� �����.
// �������:
//   int* ptr = raw_alloc<int>();
//     - ������� � ������������ ������ ���� ������ ���� int
//   char* cptr = raw_alloc<char>(10);
//     - ������� � ������������ ������ ������ �� 10 �������� ���� char
//   cptr = raw_realloc(cptr, 15);
//     - �������� ������ ���������� ������
//   raw_free(cptr)
//     - ������������ ���������� ������
// ��� ������ ������� raw_realloc() ��� ������������� ��������� ��� ��������,
// ��������� ��� ��������� �� ���� ��������� ptr.

template <typename T>
inline T* raw_alloc( int len = 1 )
{
  return static_cast<T*>(mem::alloc(len * sizeof(T)));
}

template <typename T>
inline T* raw_realloc( T* ptr, int len )
{
  return static_cast<T*>(mem::realloc(ptr, len * sizeof(T)));
}

template <typename T>
inline void raw_free( T* ptr )
{
  mem::free(ptr);
}

}; // namespace lwml

// ����� �� �������� ������� ���������� ������� ���������!

// ����� ��� �����������, ����������� ��� lwml.

struct lwml_alloc_t {};

extern const lwml_alloc_t lwml_alloc;

// ��������� ��������� ������ ���������� � ���������� ������� ���������.
// ��� ��������� �� ��������� ������� ���������� ����������,
// ��������� ��� �� ������������� ����� ���� ��������� "�����" lwml_alloc.
// ��� ��������� ���������� �� ����������� ���, ��� �� ������
// ��� ���������� ���������� ��� lwml ���������� ������ bad_alloc.
// ���������� ����������� ����� ����� ������� lwml::mem::alloc().

inline void* operator new( size_t sz, const lwml_alloc_t& )
{
  return lwml::mem::alloc(sz);
}

inline void* operator new[]( size_t sz, const lwml_alloc_t& )
{
  return lwml::mem::alloc(sz);
}

inline void operator delete( void* ptr, const lwml_alloc_t& )
{
  lwml::mem::free(ptr);
}

inline void operator delete[]( void* ptr, const lwml_alloc_t& )
{
  lwml::mem::free(ptr);
}

#endif // _STDMEM_
