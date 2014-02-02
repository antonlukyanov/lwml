// Vector and matrix helper functions
// lwml, (c) ltwood

#ifndef _VMHF_
#define _VMHF_

#include "defs.h"
#include "mdefs.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

// ��������� ����� ��� ���������� �������� � ���������

template <typename T>
class t_minmax : public value {
public:
  t_minmax( const T& v ) : _min(v), _max(v) {}

  void put( const T& v ){
    t_minize<T>(_min, v);
    t_maxize<T>(_max, v);
  }

  const T& min() const { return _min; }
  const T& max() const { return _max; }

private:
  T _min, _max;
};

// �������� ����������� ������ � ��������.
// ������� �� ����-������� copy() � copy_sub() ������� � ��������� �����������
// �������� � ������ ��������� ����� ��� ����� ������� ����������������.
// �������������� ��������� ����
//   ��������: vector, int_vector, t_array<type>, t_membuf<type>
//   � ������: matrix, int_matrix.
// �� ���������� ����������� �������� � ������ ������� �������, ���������
// ��� ��������� ������������ ���� ������ ������� "������������" ��������
// ��� ���������� ����������� ������������.
// �� �������� ��������� ������� �������� int len() � operator[](int),
// � �� ������ -- int str(), int col(), operator()(int, int).
// ������� ����������� ����������� � ��� ������������ ������������ ����������.
// ���� �� �������� ��������� � ���������� �����������, ��������� ��� �� �����
// ��� �������� ��������� �������.
// ������������ ��� ������ �������� �������� ��� �������� ������, ��������
// �� ������� ����������.
// �������� � ��������� '_sub' �������� � �������� ����� ����������� ���������.
// ��������������, ��� ������ ��������� �� ����������� ������ ���������.
// � ���������� ���������� ������������ � ��������� ����������� � �������� �����.

template<typename DV, typename SV>
  void t_vcopy( DV& dst, const SV& src );
template<typename DV, typename SV>
  void t_vcopy_sub( DV& dst, const SV& src, int idx );

template<typename DM, typename SM>
  void t_mcopy( DM& dst, const SM& src );
template<typename DM, typename SM>
  void t_mcopy_sub( DM& dst, const SM& src, int i0, int j0 );

// implementation

template<typename DV, typename SV>
void t_vcopy( DV& dst, const SV& src )
{
  int sz = src.len();
  test_size(dst.len(), sz);
  for( int j = 0; j < sz; j++ )
    dst[j] = src[j];
}

template<typename DV, typename SV>
void t_vcopy_sub( DV& dst, const SV& src, int idx )
{
  int sz = dst.len();
  // idx + sz - 1 > src.len() - 1
  if( idx < 0 || idx + sz > src.len() )
    fail_index(idx);
  for( int j = 0; j < sz; j++ )
    dst[j] = src[idx + j];
}

template<typename DM, typename SM>
void t_mcopy( DM& dst, const SM& src )
{
  int ns = src.str();
  int nc = src.col();
  test_size2(dst.str(), ns, dst.col(), nc);
  for( int s = 0; s < ns; s++ ){
    for( int c = 0; c < nc; c++ )
      dst(s, c) = src(s, c);
  }
}

template<typename DM, typename SM>
void t_mcopy_sub( DM& dst, const SM& src, int i0, int j0 )
{
  int ns = dst.str();
  int nc = dst.col();
  // i0 + ns - 1 > src.str() - 1 || j0 + nc - 1 > src.col() - 1
  if( i0 < 0 || i0 + ns > src.str() )
    fail_index(i0);
  if( j0 < 0 || j0 + nc > src.col() )
    fail_index(j0);

  for( int i = 0; i < ns; i++ ){
    for( int j = 0; j < nc; j++ )
      dst(i, j) = src(i0 + i, j0 + j);
  }
}

}; // namespace lwml

#endif // _VMHF_
