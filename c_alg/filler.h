// Simple recursive filler
// lwml, (c) ltwood

#ifndef _FILLER_
#define _FILLER_

#include "defs.h"
#include "mdefs.h"

#include "t_list.h"
#include "igeom.h"

/*#build_stop*/

namespace lwml {

// ??????? ??????????? ???????? ?????????? ??????? ???????? ?? ?????? ?????.
// ???????? ?????? mark ??? ???????? ?????????? ?????????,
// ?????????? ??????? ? ???????????? x, y
// ? ?????????? ????????? ?????????? ????????, ??????? ?? ??
// ????????, ??? ? ??????? ? ???????????? x, y.
// bim -- ??????? ?????? ??????????????? ???????????.

class filler : public scope {
public:
  template<typename T, typename elT>
  static void mark_4n( T& bim, int x, int y, elT mark );
  
  template<typename T, typename elT>
  static void mark_8n( T& bim, int x, int y, elT mark );
};

// implementation

template<typename T, typename elT>
void filler::mark_4n( T& bim, int x, int y, elT mark ){
  t_list<int_point> stack(t_list<int_point>::MANUAL_PACK);

  int lx = bim.col();
  int ly = bim.str();
  elT what = bim(y, x);
  stack.put(int_point(x, y));
  while( !stack.is_empty() ){
    int_point p = stack.get();
    bim(p.y(), p.x()) = mark;
    if( p.x() > 0 && bim(p.y(), p.x()-1) == what )
      stack.put(int_point(p.x()-1, p.y()));
    if( p.x() < lx-1 && bim(p.y(), p.x()+1) == what )
      stack.put(int_point(p.x()+1, p.y()));
    if( p.y() > 0 && bim(p.y()-1, p.x()) == what )
      stack.put(int_point(p.x(), p.y()-1));
    if( p.y() < ly-1 && bim(p.y()+1, p.x()) == what )
      stack.put(int_point(p.x(), p.y()+1));
  }
}

template<typename T, typename elT>
void filler::mark_8n( T& bim, int x, int y, elT mark ){
  t_list<int_point> stack(t_list<int_point>::MANUAL_PACK);

  int lx = bim.col();
  int ly = bim.str();
  elT what = bim(y, x);
  stack.put(int_point(x, y));
  while( !stack.is_empty() ){
    int_point p = stack.get();
    bim(p.y(), p.x()) = mark;
    if( p.x() > 0 && bim(p.y(), p.x()-1) == what )
      stack.put(int_point(p.x()-1, p.y()));
    if( p.x() < lx-1 && bim(p.y(), p.x()+1) == what )
      stack.put(int_point(p.x()+1, p.y()));
    if( p.y() > 0 && bim(p.y()-1, p.x()) == what )
      stack.put(int_point(p.x(), p.y()-1));
    if( p.y() < ly-1 && bim(p.y()+1, p.x()) == what )
      stack.put(int_point(p.x(), p.y()+1));
      
    if( p.x() > 0 && p.y() > 0 && bim(p.y()-1, p.x()-1) == what )
      stack.put(int_point(p.x() - 1, p.y() - 1));
    if( p.x() < lx-1 && p.y() > 0 && bim(p.y()-1, p.x()+1) == what )
      stack.put(int_point(p.x() + 1, p.y() - 1));
    if( p.x() > 0 && p.y() < ly-1 && bim(p.y()+1, p.x()-1) == what )
      stack.put(int_point(p.x() - 1, p.y() + 1));
    if( p.x() < lx-1 && p.y() < ly-1 && bim(p.y()+1, p.x()+1) == what )
      stack.put(int_point(p.x() + 1, p.y() + 1));    
  }
}

}; // namespace lwml

#endif // _FILLER_
