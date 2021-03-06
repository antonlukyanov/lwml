// ??????? ??????? ?????????.
// lwml, (c) ltwood

#ifndef _BITSET_
#define _BITSET_

#include "defs.h"
#include "t_membuf.h"
#include "stream.h"

/*#lake:stop*/

namespace lwml {

// ????????? ??????????? ??? ?????? ?????, ??????????? ? ???????????????? ?????.
// ????????? ????????? ?????? ? ??????? ?????? ? ???? ?????? ??????.
// ????????? ????????? ????? ????????????? ??????????? ????????? ? ?????????.
// ??????? ????? ???????? ??????????????????? ?? ????????????:
// ????? ??????????? ??????????????? ? ???????, ? ???? ? ???
// ???????????????, ??????? ? ????????.
// ????? ?????? ?????? ?????? ??????????????? ????? ????????? ?????????.
// ??? ??????? ? ???????? ??? ??????????? ????? ?????? ?????? ???????
// ???????????? ??????? char_num().

class bitset : public value {
public:
  // ??????? ??????? ????????? ???????? size ???, ????????? ?????? ??????? ? v.
  bitset( int size, bool v = false );

  // ????????????? ???????? ????????? ?? ?????? ??????.
  void char_read( const char* buf );

  // ?????????????? ???????? ????????? ? ?????? ??????.
  void char_write( char* buf ) const;

  // ?????????? ????? ?????? ? ?????????????? ????????? ?????? ??????.
  int char_num() const { return _data.len(); }

  // ???????? ????? ????? ? ?????????.
  int len() const { return _size; }

  // ???????? ??? ???? ????????? ? ????.
  void set_zero();

  // ?????????? ??? ???? ????????? ? ???????.
  void set_one();

  // ?????????? ??? ? ??????? bit.
  void set( int bit );

  // ???????? ??? ? ??????? bit.
  void clear( int bit );

  // ????????????? ??? ? ??????? bit.
  void flip( int bit );

  // ???????? ???????? ???? ? ??????? bit.
  bool test( int bit ) const;

  // ???????? ???????????? ???????? ???? ?? ????????? 1.
  int get_max();

  // ???????? ??????????? ???????? ???? ?? ????????? 1.
  int get_min();

  // ????????? ????????? ?? ???????????.
  // ?????????? true ???? ????????? ???????????? ? ?????????? ls.
  bool is_meet( const bitset& ls ) const;

  // ?????????? ??????????? ????????.
  // ????????? ?????????-???????? ? ???????? ????????? ?.?.
  // ????????????? ? ????????? ??? ????, ????????????? ? ????????? ls.
  // ?????????? ?????? ?? ?????????? ???????? ?????????.
  bitset& join( const bitset& ls );

  // ?????????? ??????????? ????????.
  // ?????????? ??????? ????????? ? ??????????-?????????? ?.?.
  // ????????? ?????????????? ?????? ????, ????????????? ? ????????? ls.
  // ?????????? ?????? ?? ?????????? ???????? ?????????.
  bitset& meet( const bitset& ls );

  // ?????????? ?????????????? ???????? ????????.
  // ? ????????? ?????? ??? ???????? ???? ????????,
  // ?? ???????? ???????????? ? ??? ????????? (????????? XOR).
  // ?????????? ?????? ?? ?????????? ???????? ?????????.
  bitset& symm_dif( const bitset& ls );

  // ?????????? true, ???? ??? ???? ????????? ? ????????? false.
  bool is_empty() const;

  void print() {
    for( int i = 0; i < _size; i++ ){
      printf("%d", test(i) ? 1 : 0);
    }
    printf("\n");
  }

private:
  int _size;
  t_membuf<uchar> _data;
};

}; // namespace lwml

#endif // _BITSET_
