// ������� � ������������ �������

#ifndef _FFTFILT_
#define _FFTFILT_

#include "defs.h"
#include "mdefs.h"
#include "vector.h"
#include "i_func.h"

/*#lake:stop*/

namespace lwml {

// �������� ������ � ������������ �������
// ���������� �������� ���������� ��������� ���������������
// ��� ��������������� ����������� ������������ ����� �������������� ������
// ���������� ������������ �������� operator()
// ������ �������� - ������, � ������� ���������� ��������� ����������,
// ������ �������� - ������ � ��������� �������
// ��� ������� ������ ����� ���������� �����
// ������ �������� - �������� ������� - ��� �������
// ��� ���� ��� ������� ��������� �������, ��� ������ ���� ������������
// �������� ������� - ��������������� �������, ������������ �� -1/2 �� 1/2

class fft_filter : public value {
public:
  fft_filter( int len );

  void operator()( vector& dst, const vector& src, const i_function& );

private:
  int _dlen;    // ������������ ����� ������
  int _splen;   // ����� �������            
  vector _xr, _xi;
};

// ��������� ������ � ������������ �������
// ���������� ���������� ��������� ���������� � �������
// ��� ��������������� ����������� ������������ ����� �������������� ������
// ���������� ������������ �������� operator()
// ������ �������� - ������, � ������� ���������� ��������� ����������,
// ������ �������� - ������ � ��������� �������
// ��� ������� ������ ����� ���������� �����
// ������ �������� - �������� ������� - ����� ���������
// ����� ���������������� ��� ���� �� �������� ����� ������ ������������ �����������,
// ��� ���������� ������� ������������ ������������ ������������ ��������
// ������������ �������� ����� ���� ����������� �������� �� ����� �������� �������

class threshold_filter : public value {
public:
  threshold_filter( int len );

  real operator()( vector& dst, const vector& src, real );

private:
  int _dlen;    // ������������ ����� ������
  int _splen;   // ����� �������
  vector _xr, _xi;
};

}; // namespace lwml

#endif // _FFTFILT_
