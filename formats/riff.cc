#include "riff.h"
#include "stdmem.h"
#include "mdefs.h"
#include "basex.h"
#include "filename.h"

/*#lake:stop*/

namespace lwml {

// definitions

const char RIFFCHUNK_ID[] = "RIFF";  // ????????????? ????? RIFF
const char FMTCHUNK_ID[]  = "fmt ";  // ????????????? ????? fmt
const char DATACHUNK_ID[] = "data";  // ????????????? ????? data

const int CHUNKID_SIZE  = 4;  // ?????? ?????????????? ?????
const int CHUNKHDR_SIZE = 8;  // ?????? ????????? ?????

const char WAVEFORM_ID[]  = "WAVE"; // ????????????? WAVE-?????
const int WAVEFORMID_SIZE = 4;      // ?????? ?????????????? WAVE-?????

const int PCMFORMAT_ID      = 1;   // ????????????? PCM-???????
const int FMTCHUNK_DATASIZE = 16;  // ?????? ?????? ????? fmt
const int FMTCHUNK_SIZE = CHUNKHDR_SIZE + FMTCHUNK_DATASIZE; // (????????? + ??????)

// common

inline int ALIGN2EVEN( int size )
{
  return (size & 0x01) ? size + 1 : size;
}

inline bool ISODD( int size )
{
  return size & 0x01;
}

// ????????? ???????

const int PCM8_MIN = 0;
const int PCM8_MAX = 255;
const int PCM8_MID = 128;

const int PCM16_MIN = -32768;
const int PCM16_MAX = 32767;
const int PCM16_MID = 0;

// chunk header

class chunkhdr : public value {
public:
  chunkhdr() {}
  chunkhdr( referer<stream> file ) { read(file); }

  void read( referer<stream> );

  void skipdata( referer<stream> );

  bool is_fmt() const  { return isid(FMTCHUNK_ID); }
  bool is_data() const { return isid(DATACHUNK_ID); }
  bool is_riff() const { return isid(RIFFCHUNK_ID); }
  int  size() const { return _sz; }

private:
  char   _id[CHUNKID_SIZE];  // ????????????? ?????
  uint32 _sz;                // ?????? ?????? ? ??????

  bool isid( const char* id ) const;
};

void chunkhdr::read( referer<stream> file )
{
  file->read(_id, sizeof(_id));
  file->read(&_sz, sizeof(_sz));
}

void chunkhdr::skipdata( referer<stream> file )
{
  file->skip(_sz);
}

bool chunkhdr::isid( const char *id ) const
{
  return strncmp(_id, id, CHUNKID_SIZE) == 0;
}

// WAVE-form hdr

class waveformhdr : public value {
public:
  waveformhdr( referer<stream> );

  bool is_wave() const;

private:
  char _id[WAVEFORMID_SIZE];
};

waveformhdr::waveformhdr( referer<stream> file )
{
  file->read(_id, WAVEFORMID_SIZE);
}

bool waveformhdr::is_wave() const
{
  return strncmp(_id, WAVEFORM_ID, WAVEFORMID_SIZE) == 0;
}

// fmt-chunk

class fmtchunk : public value {
public:
  fmtchunk( referer<stream> file, int size );

  int channels() const { return _channels; }
  int samplespersec() const { return _samplespersec; }
  int bitspersample() const { return _bitspersample; }
  int alignment() const { return _alignment; }

private:
  uint16 _formattag;         // ????????? ???????
  uint16 _channels;          // ????? ???????
  uint16  _bitspersample;    // ??????????? ?????? (? ????? ??????)
  uint32 _samplespersec;     // ??????? ????????????? (??????? ? ??????? ? ????? ??????)
  uint32 _bytespersec;       // ????? ???? ? ??????? ??? ???? ???????
  uint16 _alignment;         // ???????????? ??????
  // _bytespersec = (_channels * _samplespersec * _bitspersample)/8
  // _alignment = (nChannels * nBitsPerSample)/8
};

fmtchunk::fmtchunk( referer<stream> file, int size )
{
  if( size < FMTCHUNK_DATASIZE )
    throw ex_riff("incorrect data size in fmt-chunk");

  file->read(&_formattag, sizeof(_formattag));
  file->read(&_channels, sizeof(_channels));
  file->read(&_samplespersec, sizeof(_samplespersec));
  file->read(&_bytespersec, sizeof(_bytespersec));
  file->read(&_alignment, sizeof(_alignment));
  file->read(&_bitspersample, sizeof(_bitspersample));

  if( _formattag != PCMFORMAT_ID )
    throw ex_riff("file format seems not to be a PCM");

  if( size > FMTCHUNK_DATASIZE )
    file->skip(size-FMTCHUNK_DATASIZE);
}

// riff wave reader

int riffwave_reader::wait4data()
{
  chunkhdr chh;
  while( 1 ){
    chh.read(_file);
    if( chh.is_data() )
      break;
    else
      chh.skipdata(_file);
  }
  return chh.size();
}

int riffwave_reader::wait4fmt()
{
  chunkhdr chh;
  while( 1 ){
    chh.read(_file);
    if( chh.is_fmt() )
      break;
    else
      chh.skipdata(_file);
  }
  return chh.size();
}

void riffwave_reader::seek( int pos )
{
  test_index(pos, data_size());
  _bufpos = pos;
  _bufsize = t_min(data_size() - pos, _data.len() / _alignment);
  _file->seek(_datapos + pos * _alignment);
  _file->read(_data.access_raw_data(), _bufsize * _alignment);
}

riffwave_reader::riffwave_reader( const char *nm, int bufsize )
{
  // ??????? ????
  _file = stream::create(nm, fmREAD, fmBINARY);

  // ????????? ???? RIFF
  chunkhdr chh(_file);
  if( !chh.is_riff() )
    throw ex_riff("bad riff-chunk, <%s>", nm);

  // ????????? ???? WAVE
  waveformhdr wfh(_file);
  if( !wfh.is_wave() )
    throw ex_riff("bad wave-chunk, <%s>", nm);

  // ????????? ????? fmt ? ?????????
  int fmtsize = wait4fmt();
  fmtchunk fmt(_file, fmtsize);
  _channels = fmt.channels();
  _samplespersec = fmt.samplespersec();
  _bitspersample = fmt.bitspersample();
  if( _bitspersample != 8 && _bitspersample != 16 )
    throw ex_riff("can't read RIFF WAVE file with given bitspersample (%d)", _bitspersample);
  _alignment = fmt.alignment();
  if( _alignment * 8 != _channels * _bitspersample )
    throw ex_riff("incorrect value of alignment field in RIFF fmt chunk");

  // ????????? ?????? ? ?????????
  _datasize = wait4data();
  _datapos = _file->tell();
  if( _datasize % _alignment != 0 )
    throw ex_riff("incorrect data size in RIFF data chunk");

  if( bufsize > 0 ) // ????? ?????? ??????
    _data.resize(bufsize * _alignment);
  else // ????? ??????? ??? ?????? ?? ?????
    _data.resize(_datasize);

  seek(0);
}

void riffwave_reader::printinfo( referer<stream> file ) const
{
  file->printf("channels=%d\n", _channels);
  file->printf("samplespersec=%d\n", _samplespersec);
  file->printf("bitspersample=%d\n", _bitspersample);
  file->printf("datasize=%d\n", _datasize);
}

int riffwave_reader::operator()( int j, channel ch ) const
{
  test_index(j, buf_size());
  if( ch == RIGHT && _channels != 2 )
    fail_assert("can't read right channel for mono RIFF WAVE");

  int sample = j * _alignment;
  if( ch == RIGHT ) sample += (_bitspersample == 8) ? 1 : 2;
  int16 res = (_bitspersample == 8) ? _data[sample] : (_data[sample+1] << 8) | _data[sample];
  return res;
}

// out parameters

namespace {
  const int OUT_BYTESPERSAMPLE = 2;  // ???? ?? ?????? ??? ??????
  const int OUT_BITSPERSAMPLE  =  OUT_BYTESPERSAMPLE * 8;

  inline int OUT_ALIGNMENT( int channels ) // ????? ?????? ?? ?????? (?? ???? ???????)
  {
    return channels * OUT_BYTESPERSAMPLE;
  }

  inline int OUT_BYTESPERSEC( int sps, int channels )
  {
    return channels * sps * OUT_BYTESPERSAMPLE;
  }

  inline int DATA_SIZE( int size, int channels )
  {
    return channels * size * OUT_BYTESPERSAMPLE;
  }

  inline int DATACHUNK_SIZE( int size, int channels )
  {
    return CHUNKHDR_SIZE + DATA_SIZE(size, channels);
  }

  void calc_min_max( const vector& v, real* min, real* max )
  {
    *min = v.min();
    *max = v.max();
    if( *max == *min )
      *max = *min + 1.0;
    if( *min == *min + 1.0 || *max == *max + 1.0 )
      throw ex_sing();
  }
};

// riff wave saver

void riffwave_saver::write_chunkhdr( referer<stream> file, const char *id, uint32 sz )
{
  file->write(id, CHUNKID_SIZE);
  file->write(&sz, sizeof(uint32));
}

void riffwave_saver::write_fmt( referer<stream> file, int sps, int channels )
{
  uint16 formattag = PCMFORMAT_ID;                      // ???????? ???????
  uint16 channels_buf = channels;                       // ????? ???????
  uint32 samplespersec = sps;                           // ??????? ?????????????
  uint16 bitspersample = OUT_BITSPERSAMPLE;             // ?????????? ?????????????
  uint32 bytespersec = OUT_BYTESPERSEC(sps, channels);  // ????? ???? ? ???????
  uint16 alignment = OUT_ALIGNMENT(channels);           // ???????????? ??????

  file->write(&formattag, sizeof(formattag));
  file->write(&channels_buf, sizeof(channels_buf));
  file->write(&samplespersec, sizeof(samplespersec));
  file->write(&bytespersec, sizeof(bytespersec));
  file->write(&alignment, sizeof(alignment));
  file->write(&bitspersample, sizeof(bitspersample));
}

void riffwave_saver::write_data( referer<stream> file, const vector& v )
{
  real min, max;
  calc_min_max(v, &min, &max);
  for( int j = 0; j < v.len(); j++ ){
    uint16 buf = scale::interval(v[j], min, max, PCM16_MIN, PCM16_MAX);
    file->write(&buf, sizeof(buf));
  }
}

void riffwave_saver::write_data( referer<stream> file, const int_vector& v )
{
  for( int j = 0; j < v.len(); j++ ){
    uint16 buf = v[j];
    file->write(&buf, sizeof(buf));
  }
}

void riffwave_saver::write_data( referer<stream> file, const int_vector& vl, const int_vector& vr )
{
  for( int j = 0; j < vl.len(); j++ ){
    uint16 lbuf = vl[j];
    uint16 rbuf = vr[j];
    file->write(&lbuf, sizeof(lbuf));
    file->write(&rbuf, sizeof(rbuf));
  }
}

void riffwave_saver::write_align( referer<stream> file )
{
  uchar buf = 0;
  file->write(&buf, sizeof(uchar));
}

void riffwave_saver::write_pref( referer<stream> file, int len, int channels, int sps )
{
  int riff_size = WAVEFORMID_SIZE + FMTCHUNK_SIZE + DATACHUNK_SIZE(len, channels);
  write_chunkhdr(file, RIFFCHUNK_ID, ALIGN2EVEN(riff_size));              // RIFF-chunk
  file->write(WAVEFORM_ID, WAVEFORMID_SIZE);                              // WAVE-form
  write_chunkhdr(file, FMTCHUNK_ID, FMTCHUNK_DATASIZE);                   // fmt_chunk
  write_fmt(file, sps, channels);
  write_chunkhdr(file, DATACHUNK_ID, DATA_SIZE(len, channels));           // data_chunk hdr
}

void riffwave_saver::write_suff( referer<stream> file, int len, int channels )
{
  int riff_size = WAVEFORMID_SIZE + FMTCHUNK_SIZE + DATACHUNK_SIZE(len, channels);
  if( ISODD(riff_size) ) // alignment
    write_align(file);
}

void riffwave_saver::write( const char* name, const vector& x, int sps )
{
  referer<stream> file = stream::create(name, fmWRITE, fmBINARY);

  write_pref(file, x.len(), 1/*channels*/, sps);
  write_data(file, x);
  write_suff(file, x.len(), 1/*channels*/);
}

void riffwave_saver::write( const char* name, const int_vector& x, int sps )
{
  referer<stream> file = stream::create(name, fmWRITE, fmBINARY);

  write_pref(file, x.len(), 1/*channels*/, sps);
  write_data(file, x);
  write_suff(file, x.len(), 1/*channels*/);
}

void riffwave_saver::write( const char* name, const int_vector& xl, const int_vector& xr, int sps )
{
  test_size(xl.len(), xr.len());
  referer<stream> file = stream::create(name, fmWRITE, fmBINARY);

  write_pref(file, xl.len(), 2/*channels*/, sps);
  write_data(file, xl, xr);
  write_suff(file, xl.len(), 2/*channels*/);
}

// ?????? ??????? ? ??????? bitspersample ?????

}; // namespace lwml
