#include "lwml/m_alg/spline.h"
#include "lwml/alg/search.h"

/*#lake:stop*/

// Интерполяция кубическим сплайном с "естественным" условием на концах (f"=0).
// Калиткин Н.Н. Численные методы.
// Глава 2, параграф 1, пункт 9. Интерполяция сплайнами.
//
// В книге описан расчет коэффициентов для "естественного" условия равенства
// нулю второй производной на концах отрезка интреполирования.
// Здесь возможно явно задать значение второй производной на концах. Для этого
// в формулы расчета внесены соответсвующие поправки.
//
// На каждом интервале многочлен задан в виде
// f_i(x) = a_i + b_i * (x-x_i) + c_i * (x-x_i)^2 + d_i * (x-x_i)^3
//
// При sz точках получаем sz-1 интервалов и 4*(sz-1) коэффициентов всего

namespace lwml {

spline::spline( int sz )
  : _szcheck(sz), _size(sz), _x(sz), _y(sz), _coeff(4*(sz-1)), _gj(sz-2)
{
}

void spline::put( const vector& x, const vector& y, real der_l, real der_r )
{
  test_size2(x.len(), _size, y.len(), _size);

  der_l /= 2.0; // для упрощения использования в формулах
  der_r /= 2.0;

  // диагонали матрицы и правая часть
  vector a(_size - 3), b(_size - 2), c(_size - 3), d(_size - 2);
  for( int j = 0; j < _size - 2; j++ ){
    b[j] = 2.0 * (x[j+2] - x[j]);
    d[j] = 3.0 * ((y[j+2]-y[j+1])/(x[j+2]-x[j+1]) - (y[j+1]-y[j])/(x[j+1]-x[j]));
  }
  // добавка к правой части первого уравнения
  d[0] -= der_l * (x[1]-x[0]);
  // добавка к правой части последнего уравнения
  d[d.len()-1] -= der_r * (x[_size-1]-x[_size-2]);

  for( int j = 0; j < _size - 3; j++ )
    c[j] = a[j] = x[j+2] - x[j+1];

  vector cc(_size-2);
  _gj.calc(cc, a, b, c, d);
  vector cx(_size-1); // временный вектор под коэффициенты c_i
  cx.copy(cc, 0, 1);
  cx[0] = der_l;
  _x = x;
  _y = y;

  // расчет коэффициентов
  for( int i = 0; i < _size-1; ++i ){
    real aa = y[i];
    real cc = cx[i];
    real bb, dd;
    if( i == _size - 2 ){
      dd = (der_r-cx[_size-2]) / (3.0*(x[_size-1]-x[_size-2]));
      bb = (y[_size-1]-y[_size-2]) / (x[_size-1]-x[_size-2]) -
           (der_r+2.0*cx[_size-2]) * (x[_size-1]-x[_size-2])/3.0;
    }else{
      dd = (cx[i+1]-cc) / (3.0*(x[i+1]-x[i]));
      bb = (y[i+1]-y[i]) / (x[i+1]-x[i]) -
           (cx[i+1]+2.0*cc) * (x[i+1]-x[i]) / 3.0;
    }
    int idx = 4*i;
    _coeff[idx  ] = aa;
    _coeff[idx+1] = bb;
    _coeff[idx+2] = cc;
    _coeff[idx+3] = dd;
  }
}

real spline::calc( const vector& x, const vector& y, const vector& coeff, real xx )
{
  test_size(x.len(), y.len());
  test_size(coeff.len(), 4*(x.len()-1));
  return calc_imp(x, y, coeff, xx);
}

// функция для внутреннего использования.
// Не проверяет соотвествие размеров векторов.
real spline::calc_imp( const vector& x, const vector& y, const vector& coeff, real xx )
{
  int pos = search::interval(x, xx);
  if( pos == -1 )
    fail_assert("spline argument");

  real cxx = xx - x[pos];

  pos *= 4; // переход к месту в массиве коэффициентов
  real a = coeff[pos];
  real b = coeff[pos+1];
  real c = coeff[pos+2];
  real d = coeff[pos+3];

  return a + cxx*( b + cxx*( c + d*cxx ) );
}

// spline2d
spline2d::spline2d( int str, int col )
  : _str(str), _col(col),
    _x(col), _y(str), _z(str, col), _col_cs(col, 4*(str-1)),
    _buf_str(col), _buf_col(str), _buf_col_cs(4*(str-1)),
    _sp_str(col)
{
  // в _col_cs в строках хранятся коэффициента сплайнов, посчитанным по столбцам
  // заданной функции
}

void spline2d::put( const vector& x, const vector& y, const matrix& z )
{
  test_size2(x.len(), _col, y.len(), _str);
  test_size2(x.len(), z.col(), y.len(), z.str());
  _x = x;
  _y = y;
  _z = z;

  // считаем сплайны по столбцам
  spline sp(_str);
  vector col(_str);
  for( int c = 0; c < _col; ++c ){
    _z.get_col(col, c);
    sp.put(_y, col, 0.0, 0.0); // посчитать по столбцу
    _col_cs.put_str(sp.coeff(), c);
  }
}

// прямая, но неэффективная реализация
real spline2d::calc( real xx, real yy ) const
{
  // вычисляем значения в каждом столбце для заданного yy
  for( int c = 0; c < _col; ++c ){
    _z.get_col(_buf_col, c);
    _col_cs.get_str(_buf_col_cs, c);
    _buf_str[c] = spline::calc(_y, _buf_col, _buf_col_cs, yy);
  }
  // строим сплайн по найденной строке
  _sp_str.put(_x, _buf_str, 0.0, 0.0);
  // вычисляем в нужной точке xx
  return _sp_str.calc(xx);
}

}; // namespace lwml
