// lwml, (c) ltwood

#ifndef _RIFF_
#define _RIFF_

// PCM WAVE RIFF
// Pulse Code Modulation
// Resource Interchange File Format

#include "defs.h"
#include "vector.h"
#include "ivector.h"
#include "stream.h"
#include "t_membuf.h"

/*#lake:stop*/

namespace lwml {

// ?????????? ??? ?????? ????????????? ??????????? RIFF-?????

DEF_EX_TYPE_MSG(ex_integrity, ex_riff, "incorrect riff");

class riffwave_reader : public value {
public:
  // ???????? bufsize ?????? ?????? ?????? ??? ???????? ?????????????? ?????? ??????.
  // ???? ???????? ????? ????????? ????? 0 (?? ?????????), ?? ??? ?????? ???????? ?????.
  riffwave_reader( const char*, int bufsize = 0 );

  void printinfo( referer<stream> ) const;

  int channels() const { return _channels; }
  int samplespersec() const { return _samplespersec; }
  int bitspersample() const { return _bitspersample; }

  int data_size() const { return _datasize / _alignment; }

  enum channel { LEFT, RIGHT };

  int operator()( int j, channel ch = LEFT ) const;

  void seek( int pos );
  int tell() const { return _bufpos; }
  int buf_size() const { return _bufsize; }

private:
  referer<stream> _file;

  int _channels;
  int _samplespersec; // ? ????? ??????
  int _bitspersample; // ? ????? ??????
  int _alignment;     // ?????? ?? ???? ?????? ????? ???????
  int _datasize;      // ????? ?????? ? ??????
  int _datapos;       // ??????? ?????? ?????? ? ????? ? ??????

  int _bufpos;           // ??????? ??????? ?????? ? ????????
  int _bufsize;          // ?????? ??????????? ????? ??????
  t_membuf<uchar> _data;

  int wait4data();
  int wait4fmt();
};

class riffwave_saver : public scope {
public:
  static void write( const char*, const vector&, int sps );
    // ?????? ?????? ?????? ? ????????????????
  static void write( const char* name, const int_vector& x, int sps );
    // ?????? ?????? ?????? ??? ???????????????
  static void write( const char* name, const int_vector& lx, const int_vector& rx, int sps );
    // ?????? ???? ??????? ??? ???????????????

private:
  static void write_chunkhdr( referer<stream> file, const char *id, uint32 sz );
  static void write_fmt( referer<stream> file, int sps, int channels );
  static void write_data( referer<stream> file, const vector& );
  static void write_data( referer<stream> file, const int_vector& v );
  static void write_data( referer<stream> file, const int_vector& vl, const int_vector& vr );
  static void write_align( referer<stream> file );

  static void write_pref( referer<stream> file, int len, int channels, int sps );
  static void write_suff( referer<stream> file, int len, int channels );
};

}; // namespace lwml

#endif // _RIFF_
