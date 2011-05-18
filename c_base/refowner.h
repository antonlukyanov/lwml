// ���������� �������������� ��������� ���������
// (c) ltwood

#ifndef _REFOWNER_
#define _REFOWNER_

#include "defs.h"

/*#lake:stop*/

namespace lwml {

// �������������� ��������� ��������� ����������� �����
// ����������� ��� ������ �� ������� ���������.
// ��������� ���������� ��������� ��� ��������������� ��� � ������� ������� reset().
// � ������� �� auto_ptr �� ������������ �������� �������������
// ����� ����������� ������������ ����������� � ��������� �����������.
// ������ ����� ����������� � �������� ����������� ������� ����������.

template <typename T>
class refowner : public hardcore {
public:
  explicit refowner( T* ptr = 0 ) : _ptr(ptr) {}

  ~refowner() { delete _ptr; }

  // ������������ ������� ��������� ����������
  //!! SIC: ��� ��������� ���������� ������������� ������, ������ ���� ������������!
  T& operator*()  const { return *_ptr; }
  T* operator->() const { return _ptr; }

  // ���� ����� ������ � ���������
  T* get() const { return _ptr; }

  // ����� �������� ������ � ������� �����
  void reset( T* ptr = 0 ) {
    if( ptr != _ptr ){
      delete _ptr;
      _ptr = ptr;
    }
  }

  // ������ �������� ������ (�������� ��������)
  T* release() {
    T* tmp = _ptr;
    _ptr = 0;
    return tmp;
  }

  // �������� �������������������� ���������.
  bool is_ok() const { return _ptr != 0; }

private:
  T* _ptr;
};

}; // namespace lwml

#endif // _REFOWNER_
