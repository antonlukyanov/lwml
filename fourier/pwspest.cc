#include "pwspest.h"
#include "fft.h"
#include "spwin.h"

/*#lake:stop*/

namespace lwml {

// pwspest

pwspest::pwspest( int len )
  : _dlen(len), _splen(up2pow2(len)),
    _xr(_splen), _xi(_splen)
{
  _xr.set_zero();
  _xi.set_zero();
}

void pwspest::put( const vector& src )
{
  if( src.len() > _dlen )
    fail_size(src.len());

  int src_len = src.len();
  // ���������� ���������� ����������� � ��������� �� ����
  // ������� ������ ������ � �������� ������
  real mid = src.mid();
  for( int j = 0; j < src_len; j++ )
    _xr[j] = spwin::asymgannw(j, src_len-1) * (src[j] - mid);
  // ���������� ������
  for( int j = src_len; j < _splen; j++ )
    _xr[j] = 0.0;
  // �������������� � ������������ �������
  _xi.set_zero();
  fft::cfft(_xr, _xi);
}

void pwspest::put( const sliding& src, int idx )
{
  if( src.dim() > _dlen )
    fail_size(src.dim());

  int src_len = src.dim();
  // ���������� ���������� ����������� � ��������� �� ����
  // ������� ������ ������ � �������� ������
  real mid = 0;
  for( int j = 0; j < src_len; j++ )
    mid += src(idx, j) / src_len;
  for( int j = 0; j < src_len; j++ )
    _xr[j] = spwin::asymgannw(j, src_len-1) * (src(idx, j) - mid);
  // ���������� ������
  for( int j = src_len; j < _splen; j++ )
    _xr[j] = 0.0;
  // �������������� � ������������ �������
  _xi.set_zero();
  fft::cfft(_xr, _xi);
}

void pwspest::get( vector& dst ) const
{
  if( dst.len() < len() )
    fail_size(dst.len());
  dst[0] = mag(_xr[0], _xi[0]);
  for( int j = 1; j < len(); j++ )
    dst[j] = 2.0 * mag(_xr[j], _xi[j]);
  dst[len()-1] = mag(_xr[len()-1], _xi[len()-1]);
  for( int j = len(); j < dst.len(); j++ )
    dst[j] = 0.0;
}

real pwspest::sum() const
{
  // ����� ������ ������������ ��������� -
  // ��� ��� ������ ������� ����������� ������,
  // � �������� ������� 0 � N/2 ������ 1 ���
  real sum = 0.0;
  for( int j = 0; j < _splen; j++ )
    sum += mag(_xr[j], _xi[j]);
  return sum;
}

}; // namespace lwml
