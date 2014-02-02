#include "fftfilt.h"
#include "fft.h"

/*#lake:stop*/

namespace lwml {

// fft_filter

fft_filter::fft_filter( int len )
  : _dlen(len), _splen(up2pow2(len)),
    _xr(_splen), _xi(_splen)
{
}

void fft_filter::operator()(
  vector& dst, const vector& src, const i_function& fs
)
{
  test_size(dst.len(), src.len());
  if( src.len() > _dlen )
    fail_size(src.len());

  // ���������� ���������� �����������
  // ������ �� �������� ������
  real mid = src.mid();
  for( int j = 0; j < src.len(); j++ )
    _xr[j] = src[j] - mid;
  // ���������� ������
  for( int j = src.len(); j < _xr.len(); j++ )
    _xr[j] = 0.0;
  // ����� ����
  fft::disturb(_xr);
  // �������������� � ������������ �������
  _xi.set_zero();
  fft::cfft(_xr, _xi);

  // ���������� ��������� ��������������
  // ��� ���� ������������ ��������
  for( int j = 0; j < _splen; j++ ){
    real f = real_cast(j - _splen/2) / real_cast(_splen);
    real al = fs(fabs(f)); // ����� � ���������, ���������� ������������� ���
    _xr[j] *= al;
    _xi[j] *= al;
  }

  // �������������� �� ��������� �������
  fft::cifft(_xr, _xi);
  // �������� �������� ����
  fft::disturb(_xr);
  // �������������� ���������� �����������
  for( int j = 0; j < dst.len(); j++ )
    dst[j] = _xr[j] + mid;
}

// threshold_filter

threshold_filter::threshold_filter( int len )
  : _dlen(len), _splen(up2pow2(len)),
    _xr(_splen), _xi(_splen)
{
}

real threshold_filter::operator()( vector& dst, const vector& src, real tr )
{
  test_size(dst.len(), src.len());
  if( src.len() > _dlen )
    fail_size(src.len());

  // ���������� ���������� �����������
  // ������ �� �������� ������
  real mid = src.mid();
  for( int j = 0; j < src.len(); j++ )
    _xr[j] = src[j] - mid;
  // ���������� ������
  for( int j = src.len(); j < _xr.len(); j++ )
    _xr[j] = 0.0;
  // �������������� � ������������ �������
  _xi.set_zero();
  fft::cfft(_xr, _xi);

  // ���������� ��������� �������� �������
  // �������� �� ���� ������������ ��������
  real sum = 0.0;
  for( int j = 0; j < _splen; j++ )
    sum += mag(_xr[j], _xi[j]);

  // ��������� ������� (����� ������) �����������
  // ����� ������, �������� �� 0 � N/2
  real max = 0.0;
  for( int j = 1; j < _splen / 2; j++ ){
    real cur = mag(_xr[j], _xi[j]);
    if( cur > max )
      max = cur;
  }
  // max - �������� �������� ����� ������ ������������

  // ������� 0 � N/2 �������� ��������, �������� ���������� ������
  real error = mag(_xr[0], _xi[0]) + mag(_xr[_splen / 2], _xi[_splen / 2]);
  _xr[0] = _xi[0] = 0.0;
  _xr[_splen / 2] = _xi[_splen / 2] = 0.0;
  // ���������� ������� ��������� ��� ������, �������� �� 0 � N/2
  for( int j = 1; j < _splen / 2; j++ ){
    real cur = mag(_xr[j], _xi[j]); // �������� �������� ������������
    if( cur < tr * max ){
      error += 2.0 * cur;
      _xr[_splen-j] = _xr[j] = 0.0;
      _xi[_splen-j] = _xi[j] = 0.0;
    }
  }

  // �������������� �� ��������� �������
  fft::cifft(_xr, _xi);
  // �������������� ���������� �����������
  for( int j = 0; j < dst.len(); j++ )
    dst[j] = _xr[j] + mid;

  return error / sum;
}

}; // namespace lwml
