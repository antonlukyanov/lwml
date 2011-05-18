#include "dirstr.h"

#include <dirent.h>

/*#lake:stop*/

namespace lwml {

dirstr::dirstr( const char* dn )
{
  _dir = opendir(dn);
  if( _dir == 0 ){
    if( errno == ENOENT )
      throw ex_dirstr("%s", dn);
    else
      fail_syscall("libc::opendir()");
  }
  read();
}

dirstr::~dirstr()
{
  if( closedir(static_cast<DIR*>(_dir)) != 0 )
    fail_syscall("libc::closedir()");
}

void dirstr::rewind()
{
  rewinddir(static_cast<DIR*>(_dir));
}

bool dirstr::read()
{
  _dirent = readdir(static_cast<DIR*>(_dir));
  return _dirent != 0;
}

strng dirstr::get() const
{
  return strng(static_cast<struct dirent*>(_dirent)->d_name);
}

bool dirstr::is_end() const
{
  return _dirent == 0;
}

}; // namespace lwml
