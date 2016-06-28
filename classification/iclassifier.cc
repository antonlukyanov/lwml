#include "debug.h"
#include "ivector.h"
#include "progress.h"

#include "iclassifier.h"

/*#build_stop*/

namespace lwml {

/**
 * ��������� ���������� ����������� ������ �� ��������, ����������� � ��������� ������.
 * @param mid ���������� ���������� ������� � �������� ������
 * @param vs ����� �������� ������� ������
 * @param w ���� ��������
 */
void project_classifier::set_mid( vector& mid, const i_vector_set& vs, const vector& w ) const
{
  vector v(vs.dim());
  mid.set_zero();
  for( int j = 0; j < vs.len(); j++ ){
    real ww = w[j];
    vs.get(v, j);
    for( int k = 0; k < mid.len(); k++ )
      mid[k] += ww * v[k];
  }
  mid /= w.inner_sum();
}

project_classifier::project_classifier( int dim, referer<luaconf> cnf, const char* root )
  : _dir(dim)
{
  for( int i = 0; i < _dir.len(); i++ )
    _dir[i] = cnf->get_real("%s.direction[%d]", root, i+1);

  _thr = cnf->get_real("%s.threshold", root);
  _is_invers = cnf->get_bool("%s.is_invers", root);
}

project_classifier::project_classifier( const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2 )
  : _dir(vs1.dim()), _is_invers(false)
{
  test_size(vs1.dim(), vs2.dim());
  test_size(vs1.len(), w1.len());
  test_size(vs2.len(), w2.len());
  int dim = vs1.dim();

  vector mid1(dim), mid2(dim);
  set_mid(mid1, vs1, w1);
  set_mid(mid2, vs2, w2);

  for( int j = 0; j < dim; j++ )
    _dir[j] = mid2[j] - mid1[j];

  // ��������� ������ ��������
  int slen = vs1.len() + vs2.len();
  vector p(slen);
  vector x(dim);
  for( int j = 0; j < vs1.len(); j++ ){
    vs1.get(x, j);
    p[j] = vector::inner_mul(x, _dir);
  }
  int sh = vs1.len();
  for( int j = 0; j < vs2.len(); j++ ){
    vs2.get(x, j);
    p[sh + j] = vector::inner_mul(x, _dir);
  }
  t_sorter<vector> sort(p);

  // ��������� �������, �������� ��������� ��� �����,
  // ������� ����� (��������) ������ ����� � ������ �� ��������
  vector sum1(slen), sum2(slen);
  sum1[0] = sum2[0] = 0.0;
  if( sort[0] < sh ) // ������ ����� ����� � ������ ���������
    sum1[0] = w1[sort[0]];
  else // ������ ����� ����� �� ������ ���������
    sum2[0] = w2[sort[0] - sh];
  for( int j = 1; j < slen; j++ ){
    sum1[j] = sum1[j-1];
    sum2[j] = sum2[j-1];
    if( sort[j] < sh ) // ��������� ����� ����� � ������ ���������
      sum1[j] += w1[sort[j]];
    else // ��������� ����� ����� �� ������ ���������
      sum2[j] += w2[sort[j] - sh];
  }

  real gsum1 = w1.inner_sum();
  real min_v = 1;
  int min_j = -1;
  int is_ok = 0;
  for( int j = 0; j < slen - 1; j++ ){
    // ������ ��������� - ����� ����� �����, ������� ������ ������ ������
    real error = (gsum1 - sum1[j]) + sum2[j];
    // �� ����� ���� ����� �� ���� ���������� �� ��������
    if( p[sort[j]] == p[sort[j+1]] ){
      zzz("project_classifier: can't separate points p[%d]=%f and p[%d]=%f", j, p[sort[j]], j+1, p[sort[j+1]]);
      continue;
    }
    bool invers = false;
    if( error > 0.5 ){
      error = 1 - error;
      invers = true;
    }

    // ��� �� ���� ����������� �������������� ��� ����, �� �� ����� ������ ������
    if( is_ok == 0 || (is_ok == 1 && error < min_v) ){
      is_ok = 1;
      min_v = error;
      min_j = j;
      _is_invers = invers;
    }
  }

  // ����� �� ��������
  if( min_j == -1 ){
    zzz("project_classifier: can't separate projections");
    _thr = p[sort[min_j+1]];
    return;
  }
  if( min_j == slen-1 )
    runtime("adaboost::mk_simp_classifier: incorrect minimum index");

  _thr = 0.5 * (p[sort[min_j]] + p[sort[min_j+1]]);
}

int project_classifier::classify( const vector& x ) const
{
  test_size(x.len(), _dir.len());

  return _is_invers^(vector::inner_mul(x, _dir) > _thr);
}

void project_classifier::get_weights( vector& w ) const
{
  test_size(w.len(), _dir.len());
  w = _dir;
  w /= _dir.norm();
}

void project_classifier::serialize( referer<luaconf> cnf, const char* root ) const
{
  cnf->exec("%s = {}", root);
  cnf->exec("%s.direction = {}", root);
  for( int i = 0; i < _dir.len(); i++ )
    cnf->exec("%s.direction[%d] = %.12g", root, i+1, _dir[i]);
  cnf->exec("%s.threshold = %.12g", root, _thr);
  if( _is_invers )
    cnf->exec("%s.is_invers = true", root);
  else
    cnf->exec("%s.is_invers = false", root);
}

const char* project_classifier::get_name() const
{
  return "project_classifier";
}

/**
 * ������� ������.
 *
 * @Todo: �������� "real*" � "bool*" �� ������.
 *
 * @param vs1 ����� �������� ������� ������
 * @param w1 ���� ��� �������� ������� ������
 * @param vs2 ����� �������� ������� ������
 * @param w2 ���� ��� �������� ������� ������
 * @param cidx
 * @param thr
 * @param is_invers
 * @param buf
 * @param sort ��������������� ������ ��������
 * @param sum1 ��������� ��� ����� ������� ������
 * @param sum2 ��������� ��� ����� ������� ������
 * @return
 */
real coord_classifier::calc_error(
  const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2, int cidx, real* thr, bool * is_invers,
  vector& buf, t_sorter<vector>& sort, vector& sum1, vector& sum2
) const
{
  // ��������� � ��������� ������ ��������.
  for( int j = 0; j < vs1.len(); j++ )
    buf[j] = vs1.get(j, cidx);
  int sh = vs1.len();
  for( int j = 0; j < vs2.len(); j++ )
    buf[sh + j] = vs2.get(j, cidx);
  sort.put(buf);

  // ��������� �������, �������� ��������� ��� �����,
  // ������� ����� (��������) ������ ����� � ������ �� ��������.
  sum1[0] = sum2[0] = 0.0;
  // ������ ����� ����� � ������ ���������.
  if( sort[0] < sh )
    sum1[0] = w1[sort[0]];
  // ������ ����� ����� �� ������ ���������.
  else
    sum2[0] = w2[sort[0] - sh];

  int slen = vs1.len() + vs2.len();
  for( int j = 1; j < slen; j++ ){
    sum1[j] = sum1[j-1];
    sum2[j] = sum2[j-1];
    // ��������� ����� ����� � ������ ���������.
    if( sort[j] < sh )
      sum1[j] += w1[sort[j]];
    // ��������� ����� ����� �� ������ ���������.
    else
      sum2[j] += w2[sort[j] - sh];
  }

  // ���������� ����������� �����.
  // gsum1 ����� ����� ��������� ������� w1, �.�. sum1[k] = sum1[k-1] + w1[sort[k]], ��� sort[k] -
  // ������������ � sum1[0] = w1[sort[0]].
  real gsum1 = sum1[slen - 1];
  real min_error = 1.0;
  int min_j = -1;
  int is_ok = 0;

  *is_invers = false;
  for( int j = 0; j < slen-1; j++ ){
    // ������ ��������� - ����� ����� �����, ������� ������ ������ ������.
    real error = (gsum1 - sum1[j]) + sum2[j];

    // ���� ������ ������ 0.5, �� ����� ������ ������ ������� � �������� ������ ������ 0.5.
    bool invers = false;
    if( error > 0.5 ){
      error = 1.0 - error;
      invers = true;
    }

    // �� ����� ���� ����� �� ���� ���������� �� ��������.
    if( buf[sort[j]] == buf[sort[j+1]] )
      continue;

    // ��� �� ���� ����������� �������������� ��� ����, �� �� ����� ������ ������.
    if( is_ok == 0 || (is_ok == 1 && error < min_error) ){
      min_error = error;
      min_j = j;
      is_ok = 1;
      *is_invers = invers;
    }
  }

  // min_j �.�. ����� -1 ������ ����� � buf ���� � �� �� ��������.
  if( min_j == -1 ){
    *thr = buf[sort[min_j+1]];
    // ���a ��� ����� ������ �� ����.
    // �� ���� ���������� ������ �� ��������, 1 - ������������ �������� ������.
    return 1.0;
  }

  // ����� �������� ������ � ��������, ����� ������ ��������� �����������.
  // �������� �������� �� ��������� ������� ����������.
  if( min_j == slen-1 )
    runtime("adaboost::mk_simp_classifier: incorrect minimum index");

  *thr = 0.5 * (buf[sort[min_j]] + buf[sort[min_j+1]]);

  return min_error;
}

coord_classifier::coord_classifier( int dim, referer<luaconf> cnf, const char* root )
{
  _dim = dim;
  _cidx = cnf->get_int("%s.coord_idx", root);
  _thr = cnf->get_real("%s.threshold", root);
  _is_invers = cnf->get_bool("%s.is_invers", root);
}

coord_classifier::coord_classifier( const i_vector_set& vs1, const vector& w1, const i_vector_set& vs2, const vector& w2 )
{
  test_size(vs1.dim(), vs2.dim());
  test_size(vs1.len(), w1.len());
  test_size(vs2.len(), w2.len());
  _dim = vs1.dim();

  int slen = vs1.len() + vs2.len();
  vector buf(slen), sum1(slen), sum2(slen);
  t_sorter<vector> sort(slen);

  real opt_thr;
  int opt_cidx = 0;
  bool is_invers = false;
  real opt_error = calc_error(vs1, w1, vs2, w2, opt_cidx, &opt_thr, &is_invers, buf, sort, sum1, sum2);
  for( int j = 1; j < _dim; j++ ){
    real thr;
    bool invers;
    real error = calc_error(vs1, w1, vs2, w2, j, &thr, &invers, buf, sort, sum1, sum2);
    if( error < opt_error ){
      opt_error = error;
      opt_thr = thr;
      opt_cidx = j;
      is_invers = invers;
    }
  }
  _cidx = opt_cidx;
  _thr = opt_thr;
  _is_invers = is_invers;
  //zzz("coord_classifier: opt_cidx=%d _thr=%f _is_invers=%d", _cidx, _thr, _is_invers);
}

void coord_classifier::get_weights( vector& w ) const
{
  test_size(w.len(), _dim);
  w.set_zero();
  w[_cidx] = 1.0;
}

void coord_classifier::serialize( referer<luaconf> cnf, const char* root ) const
{
  cnf->exec("%s = {}", root);
  cnf->exec("%s.coord_idx = %d", root, _cidx);
  cnf->exec("%s.threshold = %.12g", root, _thr);
  if( _is_invers )
    cnf->exec("%s.is_invers = true", root);
  else
    cnf->exec("%s.is_invers = false", root);
}

const char* coord_classifier::get_name() const
{
  return "coord_classifier";
}


} /* namespace lwml */
