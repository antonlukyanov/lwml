#include "rfft2d.h"

#include "progress.h"
#include "fft.h"
#include "rfft.h"

/*#lake:stop*/

namespace lwml {

void rfft2d::rcfft( matrix& m, tick_mode tick )
{
  int ns = m.str();
  int nc = m.col();

  int steps = nc/2 + ns/2;
  if( tick == tmON )
    progress::start("rfft2d::rcfft", steps);

  // ��������� ��������
  vector ca(ns);
  vector cb(ns);
  for( int c = 0; c < nc/2; c++ ){
    int c1 = 2 * c;
    int c2 = c1 + 1;
    // ������������ ����������� ��� �������
    m.get_col(ca, c1);
    m.get_col(cb, c2);
    rfft::rcfft2(ca, cb);
    // ��������� ��������� � �������
    m(0, c1) = ca[0];
    m(1, c1) = cb[0];
    m(0, c2) = ca[ns-1];
    m(1, c2) = cb[ns-1];
    for( int j = 1; j < ns/2; j++ ){
      int idx = 2 * j;
      m(idx, c1) = ca[j];
      m(idx+1, c1) = cb[j];
      m(idx, c2) = ca[ns-1 - j];
      m(idx+1, c2) = cb[ns-1 - j];
    }
    // ��� ��������-����������
    if( tick == tmON )
      progress::up(c);
  }

  // ��������� �����
  vector sa(nc);
  vector sb(nc);
  // ������ ��� ������ �������������� ����������
  m.get_str(sa, 0);
  m.get_str(sb, 1);
  rfft::rcfft2(sa, sb);
  m.put_str(sa, 0);
  m.put_str(sb, 1);
  // ��������� ��������� �����
  for( int s = 1; s < ns/2; s++ ){
    int s1 = 2 * s;
    int s2 = s1 + 1;
    // �������� ������ � ����������� fft
    m.get_str(sa, s1);
    m.get_str(sb, s2);
    fft::cfft(sa, sb);
    // ��������� ��������� � �������
    m.put_str(sa, s1);
    m.put_str(sb, s2);
    // ��� ��������-����������
    if( tick == tmON )
      progress::up(nc/2 + s);
  }

  if( tick == tmON )
    progress::finish();
}

void rfft2d::crifft(  matrix& m, tick_mode tick )
{
  int ns = m.str();
  int nc = m.col();

  int steps = ns/2 + nc/2;
  if( tick == tmON )
    progress::start("rfft2d::crifft", steps);

  // ��������� �����
  vector sa(nc);
  vector sb(nc);
  // ������ ��� ������ �������������� ����������
  m.get_str(sa, 0);
  m.get_str(sb, 1);
  rfft::crifft2(sa, sb);
  m.put_str(sa, 0);
  m.put_str(sb, 1);
  // ��������� ��������� �����
  for( int s = 1; s < ns/2; s++ ){
    int s1 = 2 * s;
    int s2 = s1 + 1;
    // �������� ������ � ����������� fft
    m.get_str(sa, s1);
    m.get_str(sb, s2);
    fft::cifft(sa, sb);
    // ��������� ��������� � �������
    m.put_str(sa, s1);
    m.put_str(sb, s2);
    // ��� ��������-����������
    if( tick == tmON )
      progress::up(s);
  }

  // ��������� ��������
  vector ca(ns);
  vector cb(ns);
  for( int c = 0; c < nc/2; c++ ){
    int c1 = 2 * c;
    int c2 = c1 + 1;
    // �������� ��� ������� ��� ��������������
    ca[0] = m(0, c1);
    cb[0] = m(1, c1);
    ca[ns-1] = m(0, c2);
    cb[ns-1] = m(1, c2);
    for( int j = 1; j < ns/2; j++ ){
      int idx = 2 * j;
      ca[j] = m(idx, c1);
      cb[j] = m(idx+1, c1);
      ca[ns-1 - j] = m(idx, c2);
      cb[ns-1 - j] = m(idx+1, c2);
    }
    rfft::crifft2(ca, cb);
    // ��������� ��������� � �������
    m.put_col(ca, c1);
    m.put_col(cb, c2);
    // ��� ��������-����������
    if( tick == tmON )
      progress::up(ns + c);
  }

  if( tick == tmON )
    progress::finish();
}

}; // namespace lwml
