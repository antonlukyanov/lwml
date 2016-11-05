#ifndef _MSG_RECIEVER_
#define _MSG_RECIEVER_

#include "defs.h"
#include "refcount.h"
#include "stream.h"
#include "cstrng.h"

namespace lwml {

//
// Приемники строковых сообщений.
//

// Интерфейс приемника строковых сообщений.
class i_msg_receiver : public interface {
public:
  virtual void put_msg( const char* msg, int mode = 0 ) const = 0;
  virtual void put_msg( const strng& msg, int mode = 0 ) const = 0;
};

// Пустой приемник текстовых сообщений.
class empty_msg_receiver : public i_msg_receiver {
public:
  virtual void put_msg( const char* msg, int mode ) const {}
  virtual void put_msg( const strng& msg, int mode ) const {}
};

// Приемник текстовых сообщений с выводом на экран.
class screen_msg_receiver : public i_msg_receiver {
public:
  virtual void put_msg( const char* msg, int mode = 0 ) const {
    printf("%s", msg);
  }
  virtual void put_msg( const strng& msg, int mode = 0 ) const {
    put_msg(msg.ascstr());
  }
};

// Приемник текстовых сообщений с выводом на экран и в файл.
class file_msg_receiver : public i_msg_receiver {
public:
  enum msg_mode { TO_ALL = 0, TO_FILE = 1, TO_SCREEN = 2 };

  file_msg_receiver( const char* fn, bool do_scr_out = true ){
    init(fn, do_scr_out);
  }

  file_msg_receiver( const strng& fn, bool do_scr_out = true ){
    init(fn.ascstr(), do_scr_out);
  }

  virtual void put_msg( const char* msg, int mode = TO_ALL ) const {
    if( _do_scr_out && (mode == TO_ALL || mode == TO_SCREEN) ){
      printf("%s", msg);
      fflush(stdout);
    }
    if( mode == TO_ALL || mode == TO_FILE )
      _file->printf("%s", msg);
  }

  void put_msg( const strng& msg, int mode = TO_ALL ) const {
    put_msg(msg.ascstr(), mode);
  }

private:
  void init( const char* fn, bool do_scr_out ){
    _do_scr_out = do_scr_out;
    _file = stream::create(fn, fmWRITE, fmTEXT);
  }

  bool _do_scr_out;
  referer<stream> _file;
};

}; // namespase endo

#endif // _MSG_RECIEVER_
