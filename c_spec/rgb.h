// Цветовые схемы
// lwml, (c) ltwood

#ifndef _RGB_
#define _RGB_

#include "defs.h"
#include "mdefs.h"

/*#lake:stop*/

namespace lwml {

// Класс rgbcolor представляет цветовую схему RGB,
// причем интенсивности цветовых компонент представлены
// вещественными числами из интервала [0,1]
// Операции изменения значения цветовых компонент пытаются
// следить за выполнением этого условия

class rgbcolor : public value {
public:
  rgbcolor(){
    _red = 0.0;
    _green = 0.0;
    _blue = 0.0;
  }
  rgbcolor( real r, real g, real b ){
    _red = correct(r);
    _green = correct(g);
    _blue = correct(b);
  }

  // установить значения цветовых компонентов
  void set( real r, real g, real b ){
    _red = correct(r);
    _green = correct(g);
    _blue = correct(b);
  }

  // получить значения цветовых компонентов
  real red() const    { return _red; }
  real green() const  { return _green; }
  real blue() const   { return _blue; }

  // изменить цветовую компоненту
  void set_red( real r )   { _red = correct(r); }
  void set_green( real g ) { _green = correct(g); }
  void set_blue( real b )  { _blue = correct(b); }

  // получить эффективную яркость цвета
  real gray() const;

private:
  real _red, _green, _blue;

  real correct( real x ) const { return (x < 0) ? 0.0 : ((x > 1.0) ? 1.0 : x); }
};

// Класс hsbcolor представляет цветовую схему HSB/HSV
// В цветовой модели HSB цветовыми компонентами являются
// тон (hue), насышенность (saturation) и яркость (brightness)
// Все цветовые компоненты представлены вещественными
// числами из отрезка [0,1]
// Тон определет цвет в цветовом круге
// Если измерять угол в градусах, то соответствие между секторами
// цветового круга и цветами следующее:
//   330-30    - красный,
//   30-90  - желтый,
// и так далее по 60 на зеленый, голубой, синий, фиолетовый.
// например, см. ссылку https://www.development-tutorial.com/how-to-code-color/
// Насыщенность - это чистота цвета (серый-чистый)
// Яркость - интенсивность цвета (черный-яркий)

class hsbcolor : public value {
public:
  hsbcolor(){
    _hue = 0.0;
    _saturation = 0.0;
    _brightness = 0.0;
  }
  hsbcolor( real h, real s, real b ){
    _hue = correct(h);
    _saturation = correct(s);
    _brightness = correct(b);
  }

  // установить значения цветовых компонентов
  void set( real h, real s, real b ){
    _hue = correct(h);
    _saturation = correct(s);
    _brightness = correct(b);
  }

  // получить значения цветовых компонентов
  real hue()        const { return _hue; }
  real saturation() const { return _saturation; }
  real brightness() const { return _brightness; }

  // изменить цветовую компоненту
  void set_hue( real h )        { _hue = correct(h); }
  void set_saturation( real s ) { _saturation = correct(s); }
  void set_brightness( real b ) { _brightness = correct(b); }

  // получить эффективную яркость цвета
  real gray() const;

private:
  real _hue, _saturation, _brightness;

  real correct( real x ) const { return (x < 0) ? 0.0 : ((x > 1.0) ? 1.0 : x); }
};

// Класс color_conv содержит статические функции-члены,
// позволяющие конвертировать цветовые схемы друг в друга

class color_conv : public scope {
public:
  // преобразование из схемы hsb в схему rgb
  static rgbcolor hsb2rgb( const hsbcolor& );

  // преобразование из схемы rgb в схему hsb
  static hsbcolor rgb2hsb( const rgbcolor& );

};

}; // namespace lwml

#endif // _RGB_
