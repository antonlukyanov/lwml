#ifndef _MSG_RECIEVER_
#define _MSG_RECIEVER_

#include "defs.h"
#include "mdefs.h"

#include "t_array.h"
#include "matrix.h"
#include "imatrix.h"
#include "igeom.h"
#include "bmp.h"

namespace lwml {

//
// ��������� ��������� ���������.
//

// ��������� ��������� ��������� ���������.
class i_msg_receiver : public interface {
public:
  virtual void put_msg( const char* msg, int mode = 0 ) const = 0;
};

// ������ �������� ��������� ���������.
class empty_msg_receiver : public i_msg_receiver {
public:
  virtual void put_msg( const char* msg, int mode ) const {}
};

// �������� ��������� ��������� � ������� �� �����.
class screen_msg_receiver : public i_msg_receiver {
public:
  virtual void put_msg( const char* msg, int mode = 0 ) const {
    printf("%s", msg);
  }
};

// �������� ��������� ��������� � ������� �� ����� � � ����.
class file_msg_receiver : public i_msg_receiver {
public:
  enum msg_mode { TO_ALL = 0, TO_FILE = 1, TO_SCREEN = 2 };

  file_msg_receiver( const char* fn, bool do_scr_out = true )
    : _do_scr_out(do_scr_out)
  {
    _file = stream::create(fn, fmWRITE, fmTEXT);
  }

  virtual void put_msg( const char* msg, int mode = TO_ALL ) const {
    if( _do_scr_out && (mode == TO_ALL || mode == TO_SCREEN) ){
      printf("%s", msg);
      fflush(stdout);
    }
    if( mode == TO_ALL || mode == TO_FILE )
      _file->printf("%s", msg);
  }

private:
  bool _do_scr_out;
  referer<stream> _file;
};

}; // namespase endo

#endif // _MSG_RECIEVER_
