// lwml, (c) ltwood

#ifndef _RIFF_
#define _RIFF_

// PCM WAVE RIFF
// Pulse Code Modulation
// Resource Interchange File Format

#include "defs.h"
#include "vector.h"
#include "stream.h"
#include "t_membuf.h"

/*#lake:stop*/

namespace lwml {

// Исключение при ошибке интерпретации содержимого RIFF-Файла

DEF_EX_TYPE_MSG(ex_integrity, ex_riff, "incorrect riff");

class riffwave_reader : public value {
public:
  riffwave_reader( const char* );

  void printinfo( referer<stream> ) const;

  int channels() const { return _channels; }
  int samplespersec() const { return _samplespersec; }
  int bitspersample() const { return _bitspersample; }

  int size() const { return _datasize / _alignment; }

  enum channel { LEFT, RIGHT };

  int operator()( int j, channel ch = LEFT ) const;

private:
  int _channels;
  int _samplespersec;
  int _bitspersample;
  int _alignment;
  int _datasize;
  t_membuf<char> _data;

  int wait4data( referer<stream> file );
  int wait4fmt( referer<stream> file );
  void read_data( referer<stream> file, int size );
};

class riffwave_saver : public scope {
public:
  static void put( const char*, const vector&, int sps );

private:
  static void write_chunkhdr( referer<stream> file, const char *id, uint32 sz );
  static void write_fmt( referer<stream> file, int sps );
  static void write_data( referer<stream> file, const vector& );
  static void write_align( referer<stream> file );
};

}; // namespace lwml

#endif // _RIFF_
