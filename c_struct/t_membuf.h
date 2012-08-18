// ���������� ������ POD-���������
// lwml, (c) ltwood

#ifndef _T_MEMBUF_
#define _T_MEMBUF_

#include "defs.h"
#include "stdmem.h"
#include "basex.h"
#include "lwmlconf.h"

/*#lake:stop*/

namespace lwml {

// ����� ��������� ������� ���������� ������.
// ����� ������ ����� ��������������� ������ �������� POD-������,
// �� ���������� �������������� ��������������� � �����������.
// �����������, ����������� ����������� � ���������� ���
// ��������� �� ����������.

template<typename T>
class t_membuf : public value {
public:
  // ��������������� ������ �������� ����� ��� ������ ������.
  explicit t_membuf( int len = 0 )
  {
    _data = raw_alloc<T>(_len = len);
  }

  // ��������������� ������ �������� ����� � �������� ���������� ���������.
  t_membuf( int len, const T& val )
  {
    _data = raw_alloc<T>(_len = len);
    for( int j = 0; j < _len; j++ )
      _data[j] = val;
  }

  t_membuf( const t_membuf<T>& src )
  {
    _data = raw_alloc<T>(_len = src._len);
    memcpy(_data, src._data, _len * sizeof(T));
  }

  // ���������� �������.
  ~t_membuf()
  {
    raw_free<T>(_data);
  }

  // ����� ����� �������� �� ������� ���������.
  t_membuf<T>& operator=( const t_membuf<T>& src )
  {
    if( &src == this )
      return *this;
    _data = raw_realloc<T>(_data, _len = src._len);
    memcpy(_data, src._data, _len * sizeof(T));
    return *this;
  }

  // ��������� ������� �������.
  void resize( int new_len )
  {
    _data = raw_realloc<T>(_data, _len = new_len);
  }

  // ��������� �������� ���� ��������� �������.
  void set_val( const T& val )
  {
    for( int j = 0; j < _len; j++ )
      _data[j] = val;
  }

  // ��������� ������� �������.
  int len() const { return _len; }

  // ��������� ��������������.
  T& operator[]( int j ) {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[j];
  }
  const T& operator[]( int j ) const {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[j];
  }

  // �������������� ������ � "�����" ������.
        T* access_raw_data()       { return _data; }
  const T* access_raw_data() const { return _data; }

  int comp( int i, int j ) const {
    return (_data[i] < _data[j]) ? -1 : (_data[i] > _data[j]) ? 1 : 0;
  }

private:
  int   _len;
  T*    _data;
};

}; // namespace lwml

#endif // _T_MEMBUF_
