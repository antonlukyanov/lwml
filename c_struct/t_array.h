// ���������� ������
// lwml, (c) ltwood

#ifndef _T_ARRAY_
#define _T_ARRAY_

#include "defs.h"
#include "stdmem.h"
#include "basex.h"
#include "lwmlconf.h"

/*#lake:stop*/

namespace lwml {

// ����� ��������� ������� ���������� ������ ��������.
template<typename T>
class t_array : public value {
public:
  // ��������������� ������ �������� ����� ��� ������ ������.
  explicit t_array( int len = 0 )
  {
    _data = raw_alloc<T>(_len = len);
    for( int j = 0; j < len; j++ )
      new(_data + j) T();
  }

  // ��������������� ������ �������� ����� � �������� ���������� ���������.
  t_array( int len, const T& val )
  {
    _data = raw_alloc<T>(_len = len);
    for( int j = 0; j < _len; j++ )
      new(_data + j) T(val);
  }

  // ��������������� ����� ��������� �������.
  t_array( const t_array<T>& src )
  {
    _data = raw_alloc<T>(_len = src._len);
    for( int j = 0; j < _len; j++ )
      new(_data + j) T(src[j]);
  }

  // ���������� �������.
  ~t_array()
  {
    destroy(_data, _len);
    raw_free<T>(_data);
  }

  // ����������� �������� (����� �� ������� ���������).
  t_array<T>& operator=( const t_array<T>& src )
  {
    if( &src == this )
      return *this;

    destroy(_data, _len);
    raw_free<T>(_data);
    _data = raw_alloc<T>(_len = src._len);
    for( int j = 0; j < _len; j++ )
      new(_data + j) T(src[j]);
    return *this;
  }

  // ��������� ������� �������.
  // � ������ ���������� ������� ������� �� ������ ���������
  // ����� ������ ��������� ��������� ����������� ���������.
  void resize( int new_len, const T& t0 = T() )
  {
    T* old_data = _data;
    int old_len = _len;

    _data = raw_alloc<T>(_len = new_len);
    int copy_len = t_min<int>(old_len, new_len);
    for( int j = 0; j < copy_len; j++ )
      new(_data + j) T(old_data[j]);
    for( int j = copy_len; j < new_len; j++ )
      new(_data + j) T(t0);

    destroy(old_data, old_len);
    raw_free<T>(old_data);
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

  // ��������� �������������� ��� �������� ��������.
  T& at( int j ) { return _data[j]; }
  const T& at( int j ) const { return _data[j]; }

  // �������������� ������ � "�����" ������.
        T* access_raw_data()       { return _data; }
  const T* access_raw_data() const { return _data; }

  int comp( int i, int j ) const {
    return (_data[i] < _data[j]) ? -1 : (_data[i] > _data[j]) ? 1 : 0;
  }

private:
  int   _len;
  T*    _data;

  void destroy( T* x, int n ){
    for( int j = 0; j < n; j++ )
      (x + j)->~T();
  }
};

}; // namespace lwml

#endif // _T_ARRAY_
