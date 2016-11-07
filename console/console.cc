#include "lwml/console/console.h"

#include "lwml/io/filename.h"
#include "lwml/base/refowner.h"
#include "lwml/utils/debug.h"
#include "lwml/system/syssrv.h"
#include "lwml/memory/stdmem.h"
#include "lwml/lwmlconf.h"

/*#lake:stop*/

namespace lwml {

// Консольная реализация рендерера прогресс-индикатора

const int CONSOLE_PROGRESS_LEN = 50;

class console_progress_renderer : public i_progress_renderer { // public refcount
private:
  console_progress_renderer();

public:
  static referer<console_progress_renderer> create() {
    return referer<console_progress_renderer>(new(lwml_alloc) console_progress_renderer());
  }

  virtual void start( const char* nm, int sz );
  virtual void finish();
  virtual bool up( int st );

private:
  bool _is_active;      // флаг активности прогресс-индикатора
  int _size;            // логический размер прогресс-индикатора
  int _vlaststate;      // состояние при последнем вызове

  void pch( int ch, int num ) const;
};

void console_progress_renderer::pch( int ch, int num ) const
{
  for( int j = 0; j < num; j++ ){
    fprintf(stderr, "%c", ch);
    fflush(stderr);
  }
}

console_progress_renderer::console_progress_renderer()
{
  _is_active = false;
}

void console_progress_renderer::start( const char* nm, int sz )
{
  if( _is_active )
    fail_assert("can't create recursive progress indicator");
  _is_active = true;

  _size = sz;
  _vlaststate = 0;

  fprintf(stderr, "%s: ", nm);
  fflush(stderr);

  if( _size >= 0 ){
    pch('.', CONSOLE_PROGRESS_LEN);       // выводим точечки
    pch(0x08, CONSOLE_PROGRESS_LEN);      // возвращаемся назад
  }
}

void console_progress_renderer::finish()
{
  if( !_is_active )
    fail_assert("progress indicator is not active, can't close it");
  _is_active = false;

  if( _size >= 0 ){
    if( _vlaststate < CONSOLE_PROGRESS_LEN )      // выводим недостающие отметки
      pch('o', CONSOLE_PROGRESS_LEN - _vlaststate);
    pch(0x08, CONSOLE_PROGRESS_LEN);              // возвращаемся назад
    pch(' ', CONSOLE_PROGRESS_LEN);               // стираем отметки
    pch(0x08, CONSOLE_PROGRESS_LEN);              // снова назад
    fprintf(stderr, "Ok\n");                      // выводим 'Ok'
    fflush(stderr);
  }
  else {
    fprintf(stderr, "done\n");
    fflush(stderr);
  }
}

bool console_progress_renderer::up( int st )
{
  if( !_is_active )
    fail_assert("progress indicator is not active, can't update it");

  if( _size >= 0 ){
    if( st < 0 ) st = 0;
    if( st > _size-1 ) st = _size-1;
    int vst = (_size > 1) ? (st * CONSOLE_PROGRESS_LEN) / (_size-1) : CONSOLE_PROGRESS_LEN;
    if( vst > _vlaststate ){
      int delta = vst - _vlaststate;
      pch('o', delta);
      _vlaststate = vst;
    }
  }
  else {
    fprintf(stderr, "%d:", st);
    fflush(stderr);
  }

  return false;
}

// console

void console::abort( const char *msg )
{
  zzz_ex("lwml:console", "abort: %s", msg);
  print_msg_error("abort", msg);
  ::exit(1);
}

void console::usage( const char *name, const char *usage )
{
  zzz_ex("lwml:console", "app: %s", name);
  zzz_ex("lwml:console", "usage: %s", usage);
  print_msg_usage(name, cstrng::form("Usage: %s", usage).ascstr());
  ::exit(1);
}

void console::handlex( const ex_base& ex )
{
  zzz_ex("lwml:console", "exception: %s", ex.msg());
  print_msg_error("exception", ex.msg());
  ::exit(1);
}

namespace {
  bool _isopt( const char* s )
  {
    return (strlen(s) >= 2 && s[0] == '-' && isalpha(s[1])) ||
           (strlen(s) >= 3 && s[0] == '-' && s[1] == '-' && isalpha(s[2]));
  }

  // Запись этих переменных делает только init(), которая должна вызываться
  // один раз. Немного опасно, но проблемы возникнут только при совершенно
  // нелепой оплошности.
  bool     _is_active = false;
  int      _sys_argc;
  char**   _sys_argv;
  int      _sys_argnum;

  referer<i_progress_renderer> _progress; //!! TODO: многопоточность?

  int count_arg()
  {
    int res = 0;
    for( int j = 1; j < _sys_argc; j++ ){
      if( !_isopt(_sys_argv[j]) )
        res++;
    }
    return res;
  }

  void test_active()
  {
    if( !_is_active )
      fail_assert("console is not initialized");
  }

  bool _use_sysmsg = false;
};

void console::print_msg_error( const char* title, const char* msg )
{
#if _USE_WIN32API_
  if( _use_sysmsg )
    syssrv::message(title, msg);
  else {
    fflush(stdout);
    fflush(stderr);
    fprintf(stderr, "\n");
    fprintf(stderr, "%s: %s\n", title, msg);
    fflush(stderr);
  }
#else
  fflush(stdout);
  fflush(stderr);
  fprintf(stderr, "\n");
  fprintf(stderr, "%s: %s\n", title, msg);
  fflush(stderr);
#endif
}

void console::print_msg_usage( const char* title, const char* msg )
{
#if _USE_WIN32API_
  if( _use_sysmsg )
    syssrv::message(title, msg);
  else {
    fflush(stdout);
    fflush(stderr);
    fprintf(stderr, "\n");
    fprintf(stderr, "%s\n", title);
    fprintf(stderr, "%s\n", msg);
    fflush(stderr);
  }
#else
  fflush(stdout);
  fflush(stderr);
  fprintf(stderr, "\n");
  fprintf(stderr, "%s\n", title);
  fprintf(stderr, "%s\n", msg);
  fflush(stderr);
#endif
}

void console::init( int argc, char *argv[], errmsg_mode errmsg )
{
  _sys_argc = argc;
  _sys_argv = argv;
  _sys_argnum = count_arg();

  // создаем рендерер прогресс-индикатора
  _progress = console_progress_renderer::create();
  progress::set_renderer(_progress);

  _is_active = true;

  _use_sysmsg = (errmsg == USE_SYSMSG);
}

strng console::app_name()
{
  test_active();
  filename nm(_sys_argv[0]);
  return nm.ne();
}

strng console::app_path()
{
  test_active();
  filename nm(_sys_argv[0]);
  return nm.path();
}

int console::argc()
{
  test_active();
  return _sys_argnum;
};

strng console::argv( int k )
{
  test_active();
  if( k < 0 || k > _sys_argnum - 1 )
    throw ex_console("can't find cmdl parameter #%d", k+1);
  int j, k1 = 0;
  for( j = 1; j < _sys_argc; j++ ){
    if( !_isopt(_sys_argv[j]) ){
      if( k1 == k )
        break;
      ++k1;
    }
  }
  return strng(_sys_argv[j]);
}

namespace {
  // Возвращает -1, если опция не найдена.
  int get_opt_pos( char ch )
  {
    for( int j = 1; j < _sys_argc; j++ ){
      const char* arg = _sys_argv[j];
      if( _isopt(arg) && arg[1] == ch )
        return j;
    }
    return -1;
  }

  int get_opt_pos( const char* opt_name )
  {
    for( int j = 1; j < _sys_argc; j++ ){
      const char* arg = _sys_argv[j];
      if( _isopt(arg) && (strstr(arg + 1, opt_name) || strstr(arg + 2, opt_name)) )
        return j;
    }
    return -1;
  }
};

bool console::is_opt( char ch )
{
  test_active();
  return get_opt_pos(ch) != -1;
}

strng console::get_opt( char ch )
{
  test_active();
  int k = get_opt_pos(ch);
  if( k == -1 )
    throw ex_console("can't find cmdl option <%c>", ch);
  char *res = _sys_argv[k] + 2;  // skip '-<ch>'
  if( *res == ':' || *res == '=' ) res++;
  return strng(res);
}

bool console::is_opt( const char* opt_name )
{
  test_active();
  return get_opt_pos(opt_name) != -1;
}

strng console::get_opt( const char* opt_name )
{
  test_active();
  int k = get_opt_pos(opt_name);
  if( k == -1 )
    throw ex_console("can't find cmdl option <%s>", opt_name);

  // Skip one or two symbols '-', name of option and symbols '=' or ':'.
  char *res = _sys_argv[k] + 1;

  if( *res == '-' )
    res++;
  res += strlen(opt_name);

  if( *res == ':' || *res == '=' )
    res++;

  return strng(res);
}

}; // namespace lwml
