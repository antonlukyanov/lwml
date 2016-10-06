// Reference counting
// lwml, (c) ltwood

#ifndef _REFCOUNT_
#define _REFCOUNT_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

/*
  �� �����.

  1. ������� ������� �� ����� �������� � ��������� ������������ �������,
     ���������� ������������� �� refcount - ������ ������ � ������������.
  2. ����� referer �������� � �������. ��������� referrer.
  3. if( _cnt == 0 ) delete this; --- �������������� ������, ���� ������ ���
     �������������� �� �����-���� ����� (��������, lwml_alloc). C++ �� �����
     ����������� ����� ������� ��� ������ ������, ������� �������, ������� ����
     ������� �� �����-�� ����� ������ �������� ��������������� ���� ����� ����������,
     � ����� ��������� ������ ���������� ���������� (Bjarne Stroustrup - The C++
     Programming Language, 4th Edition, 11.2.4).
  4. operator delete( void* ptr, const lwml_alloc_t& ) ���������� ������ � ������
     ������������� ��������� ������ (��������, ��� ������� ���������� �� ������������).
  5. ���� ������, ���������� � referer ��� ������ � �����-�� ���� ������, �� �����
     delete ����� ���� ��� �� ����� ��������� ���������, ������� ��� ����� ����.
  6. ����� ����� operator delete(this, lwml_alloc) �� ����� ���������������.
  7. ���� � referer �������� ������� Tp[], �� ������ ���������� delete[] this, � �� ������
     delete this.

  � �������� ������ �� ������� (��� � STL), ��� ��������������� referer ������ ���������
  ���������� ����� ���������� deleter<X>, ������� � ����� ������������ ��������
  ������� ���������� �������. ��������� deleter ����� ��������� ��������� ����������
  �� ���������.
*/

// ������ �������, �������������� ������� ������.
// ����� ����������� ���������� �������� ����� ������� �� ������ �������� ������
// ����������� ������� referer (refcount.h).
// ����� ������ �� ������ ������������� ����������� ����������������� ��������
// ���������� �.�. ������ ����� ��������� �����������.
//
// ����� refcount ����������� �� refcount_imp ����������
// � ����� ���������� ����������� ������������� �� ����
// ����������, ������ �� ������� ������� �������� ������,
// �.�. ����� ����������� �� refcount.

class refcount_imp {
public:
  refcount_imp() : _cnt(0) {}

  virtual ~refcount_imp() {}

  void inc_ref() const {
    ++_cnt;
  }

  void dec_ref() const {
    if( _cnt > 0 ){
      --_cnt;
      if( _cnt == 0 ) delete this;
    }
  }

private:
  mutable int _cnt;

  // no copy operations
  refcount_imp( const refcount_imp& );
  refcount_imp& operator=( const refcount_imp& );
};

class refcount : virtual public refcount_imp {
};

// �����-������ �� ������� ������ � ��������� ������.

template<typename T>
class referer : public value {
public:
  referer( T* ptr = 0 )             : _ptr(ptr)       { inc_ref(); }
  referer( const referer<T>& obj )  : _ptr(obj._ptr)  { inc_ref(); }

  ~referer() { dec_ref(); }

  referer<T>& operator=( const referer<T>& ref ){
    if( &ref != this ){
      dec_ref();
      _ptr = ref._ptr;
      inc_ref();
    }
    return *this;
  }

  // ������������� ����������� � �������� ����������� ��� �����������
  // ����������� ���������� ������ �� ������� ����� ������ �� �����-���������.
  // ��� ������� �� �������� ����� ������� ����������� � �������� �����������
  // ��-�� ������������� ������ ������ ������������ � ��������� ����������� � C++.
  // �� ����� ���� � ������� ������ ��� ���� �� �������� ������������� � ����������
  // �����������, � ������������ ������������� �������� �������������� ����.

  template<typename T2>
    referer( const referer<T2>& obj ) : _ptr(obj.capture_raw_pointer())  {}

  template<typename T2>
  referer<T>& operator=( const referer<T2>& ref ){
    dec_ref();
    _ptr = ref.capture_raw_pointer();
    return *this;
  }

  // �������� �������������������� ���������.
  bool is_ok() const    { return _ptr != 0; }

  //!! SIC: ��� ��������� ���������� ������������� ������, ������ ���� ������������!
  T& operator*()  const { return *_ptr; }
  T* operator->() const { return _ptr; }

  // �������������� ������ � ������������ ���������.
  T* capture_raw_pointer() const    { inc_ref(); return _ptr; }
  void  release_raw_pointer()       { dec_ref(); }

private:
  T* _ptr;

  void inc_ref() const { if( _ptr ) _ptr->inc_ref(); }
  void dec_ref() const { if( _ptr ) _ptr->dec_ref(); }
};

}; // namespace lwml

#endif // _REFCOUNT_
