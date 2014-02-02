// Реализация абстрактного графического устройства
// в виде транзитного графического устройства
// lwml, (c) ltwood

#ifndef _VIEWPORT_
#define _VIEWPORT_

#include "defs.h"
#include "mdefs.h"
#include "canvas.h"
#include "stdmem.h"
#include "refcount.h"

/*#lake:stop*/

namespace lwml {

// класс viewport - т.н. транзитное графическое устройство
// Транзитное устройство служит для преобразования координат
// Оно позволяет клиенту рисовать в своей координатной системе,
// а само синхронно рисует в прямоугольнике, лежащем на
// другом наследнике класса i_canvas со своей системой координат
// Класс viewport реализует интерфейс класса i_canvas
// В файле canvas.h можно найти описание этого интерфейса
// Здесь описаны только особенности класса viewport
// При конструировании можно указать только канву назначения,
// в которой будет происходить рисование и границы сцены
// самого объекта viewport
// В этом случае рисование идет по всей плоскости канвы назначения
// Если при конструировании указывается параметр dstrect,
// то этот параметр задает прямоугольник из канвы назначения,
// в котором будет производиться рисование
// Функции xunit(), yunit() дают размеры RLU в координатах viewport
// Функция отрисовывает границы объекта viewport на канве назначения (рамку)
// Функция set_clip() устанавливает режим отсечения линий по границам viewport
// Отсечение работает только для функций line() и point()

class viewport : public i_canvas { // public refcount
protected:
  // конструктор для рисования на всей сцене канвы назначения
  viewport( referer<i_canvas>, frect srcrect );

  // конструктор для рисования в прямоугольнике dstrect,
  // лежащем на сцене канвы назначения
  viewport( referer<i_canvas>, frect srcrect, frect dstrect );

public:
  // конструктор для рисования на всей сцене канвы назначения
  static referer<viewport> create( referer<i_canvas> pcv, frect srcrect = frect(0, 1, 0, 1) ){
    return referer<viewport>(new(lwml_alloc) viewport(pcv, srcrect));
  }

  // конструктор для рисования в прямоугольнике dstrect,
  // лежащем на сцене канвы назначения
  static referer<viewport> create( referer<i_canvas> pcv, frect srcrect, frect dstrect ){
    return referer<viewport>(new(lwml_alloc) viewport(pcv, srcrect, dstrect));
  }

  virtual frect boundrect() const { return _src; }
  virtual real xunit() const;
  virtual real yunit() const;

  virtual void pixel( real x, real y );
  virtual void marker( real x, real y );

  virtual void line( real x1, real y1, real x2, real y2 );
  virtual void box( real x1, real y1, real x2, real y2 );
  virtual void rectangle( real x1, real y1, real x2, real y2 );

  virtual void triangle( real x1, real y1, real x2, real y2, real x3, real y3 );
  virtual void facet( real x1, real y1, real x2, real y2, real x3, real y3 );

  virtual void set_gray( real graycol );
  virtual void set_rgb( rgbcolor rgbcol );
  virtual void set_hsb( hsbcolor hsbcol );

  virtual void puts(
    real x, real y, const char* str, const align& algn, real alpha = 0.0
  );

  virtual void special( real x, real y, int id, const char* cmd );

  // отрисовывает границы объекта viewport на канве назначения
  void show_viewbox();

  // установить режим отсечения линий по границам viewport
  void set_clip( bool state = true ) { _clip_state = state; }

private:
  referer<i_canvas> _cv;  // канва для рисования
  frect _src;             // входной прямоугольник
  frect _dst;             // прямоугольник назначения

  color_scheme _colortype;        // текущая схема выбора цвета
  real _gray;                     // цвет для схемы GRAY
  rgbcolor _rgb;                  // цвет для схемы RGB
  hsbcolor _hsb;                  // цвет для схемы HSV

  bool _clip_state;

  void setup();

protected:
  void setcol();

  real X( real x );
  real Y( real y );

  referer<i_canvas> basecv() const { return _cv; }
  frect baserect() const { return _dst; }
};

// стандартные значения отступов для диаграммы (см. класс diagram)
// все значения даны в долях RLU

const real std_diagram_top_shift = 0.25;
const real std_diagram_bot_shift = 0.15;
const real std_diagram_xgridsize = 0.35;
const real std_diagram_ygridsize = 0.35;

// класс diagram_param - вспомогательный
// Он просто хранит локальные параметры диаграммы
// Конструктор устанавливает умалчиваемые значения
// Функции установки параметров возвращают ссылку на класс,
// что позволяет записывать такую цепочку действий:
// diagram_param().set_xshift(0.16).set_ygridsize(0.38);
// Функции получения параметров позволяют классу diagram
// получить установленные параметры

class diagram_param : public value {
public:
  diagram_param(){
    _top_shift = std_diagram_top_shift;
    _bot_shift = std_diagram_bot_shift;
    _xgridsize = std_diagram_xgridsize;
    _ygridsize = std_diagram_ygridsize;
  }

  // функции установки параметров
  // можно установить: отступы, размеры засечки
  // все размеры измеряются в долях RLU
  diagram_param& set_top_shift( real top_shift ){
    _top_shift = top_shift; return *this;
  }
  diagram_param& set_bot_shift( real bot_shift ){
    _bot_shift = bot_shift; return *this;
  }
  diagram_param& set_xgridsize( real xgridsize ){
    _xgridsize = xgridsize; return *this;
  }
  diagram_param& set_ygridsize( real ygridsize ){
    _ygridsize = ygridsize; return *this;
  }

  // функции получения параметров
  real top_shift() const { return _top_shift; }
  real bot_shift() const { return _bot_shift; }
  real xgridsize() const { return _xgridsize; }
  real ygridsize() const { return _ygridsize; }

private:
  real _top_shift, _bot_shift;
  real _xgridsize, _ygridsize;
};

// diagram - специфическое транзитное графическое устройство
// отличается от класса viewport тем, что само выбирает место
// для рисования в канве назначения, делая отступ для подписей и засечек

class diagram : public viewport { // public refcount
private:
  diagram( referer<i_canvas>, frect srcrect, const diagram_param& par );

public:
  static referer<diagram> create( 
    referer<i_canvas> pcv, frect srcrect = frect(0, 1, 0, 1),
    const diagram_param& par = diagram_param()
  ){
    return referer<diagram>(new(lwml_alloc) diagram(pcv, srcrect, par));
  }

  // поставить засечку в заданной позиции на оси X или Y
  // с необязательной подписью
  void xgrid( real, const char *subs = 0 );
  void ygrid( real, const char *subs = 0 );

  // подписать диаграмму (справа вверху)
  void title( const char* );

  // подписать ось X или Y с краев
  // нулевой указатель приводит к отсутствию подписи
  void xsubscript( const char* bl = 0, const char* br = 0 );
  void ysubscript( const char* lb = 0, const char* lt = 0 );

private:
  real _tdx, _tdy;   // отступ над надписью в координатах канвы назначения
  real _bdx, _bdy;   // отступ под надписью в координатах канвы назначения
  real _xgsz, _ygsz; // размер рисок в координатах канвы назначения

  frect blow( referer<i_canvas> cv, const diagram_param& );
};

}; // namespace lwml

#endif // _VIEWPORT_
