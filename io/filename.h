// ������ � ������� ������
// lwml, (c) ltwood

#ifndef _FILENAME_
#define _FILENAME_

#include "defs.h"
#include "cstrng.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

// ���������� ��� ������� ������� � ��������������� �����

DEF_EX_TYPE_MSG(ex_base, ex_nosuchfile, "no such file");

// ����� ������������� ������ ����� �����
// ������ ��� ����� ����������� �� ����, ��� � ����������
// ������ ����� ������� ����� �������� � ���� ������� ������ strng
// ��� � ������������ ����������� �� �����
// ��� ������� ����� ��������� ����� ����� ����� � ������ ��
// ���������� ����� ������������
// ����������� ���������� ����� ����� �� ��������� ����� ������������
// ��� ������� ����� ���� D:NAME.EXT ������ ��������� ������ D:NAME
// ������� set_xxx() ��������� �������� ��������� ��������� ����� ������� �����
// ��������������� ������� � ������� xxx() ���������� ��������� ����� �����
// ������� pne() ���������� ������ (����������) ��� �����
// ������� ne() ���������� ������������� (��� ����) ��� �����
//
// ����� �������� ����� ����������� ������� ��� ���������� ����� ������������ ��������
// ������� chext() �������� ���������� �����
// ������� chrelname() �������� ������������� ��� ����� (��� ����� ����)
// ������� index() ���������� ��� �����, �������� � ����� ����������� �����
// �������� ������ (�������� width ��������� ������������ ������������� �������)
// ������� suffix() ���������� ������� index(), �� ��������� ��������� �������,
// ������� ��� �� ����� �������
//
// � ��������� ������ ����� �������� �������, ���������� ���������������
// � �������� ��������.
// ��� ������� ����� �������� ��� � ����������������,
// ��� � � ������������������ ������� ��������.
// � ��� ����� ������� � ����, ��� �� ����� ����� ����� ���� ����������
// ����������� ��������� ���� (����� �����), ������� ������������
// � �� ���������������� ��� �������������� �������� ����������.
// ����������� ������� is_exists() ���������, ���������� �� ���� � �������� ������.
// ����� is_exists() ���������, ���������� �� ���� � ������, ���������� � �������.
// ����������� ������� remove() ������� ���� � �������� ������.
// ����� remove() ������� ���� � ������, ���������� � �������.
// ����������� ������� rename() ��������������� ���� � ������, ��������
// � ������ ���������, ������� ��� ��� �� �������� �� ������ ���������.
// ����� rename() ��������������� ���� � ������, ���������� � �������,
// ������� ��� ��� �� ��������.
// ����������� ������� mod_time() ���������� ����� �����������
// ��� ����� � �������� ������.
// ����� mod_time() ���������� ����� ����������� ��� ����� � ������,
// ���������� � �������.
// ����������� ������� norm_path() ���������� ���������� ����.
// ����� ���������� ���� ������ ������������ �� ����, � ��� �����
// � ���� ����������� �����������.
// �� ��������� ������������ ������ ���� ('/'), �� ���� �� ������ ���������
// (use_backslash) �������� �������� true, �� ������������ �������� ���� ('\').
// �� ��������� �������� ������� ��������� ��������-���������.
// ��� ������������������� ������ ����� �������� false � ��������� case_sens
// � ����� ��������� ������������ ����� �������������� � ������ �������.
// ������� current_path() ���������� ����������������� ���� � ������� ����������.

class filename : public value {
public:
  filename( const char* fnm );

  void set_path( const char* path ) { _path = path; }
  void set_name( const char* name ) { _name = name; }
  void set_extn( const char* extn ) { _extn = extn; }

  strng path() const { return _path; }
  strng name() const { return _name; }
  strng extn() const { return _extn; }

  strng pn() const;   // path+name
  strng pne() const;  // path+name+extn
  strng ne() const;   // name+extn

  static strng chext( const char*, const char* );
  static strng chrelname( const char* nm, const char* nnm );
  static strng index( const char* nm, int idx, int width = 2 );
  static strng suffix( const char* nm, const char* suff );

  // ������, ���������� � �������� ��������.

  static bool is_exists( const char* fname );
  static bool is_dir( const char *fname );
  static void remove( const char* fname );
  static void rename( const char* fname, const char* new_fname );
  static time_t mod_time( const char* fname );

  bool is_exists();
  bool is_dir();
  void remove();
  void rename( const char* new_fname );
  time_t mod_time();

  enum delimiter { POSIX_DELIM, WIN_DELIM };
  enum case_sensitivity { CASE_SENS, CASE_INSENS };

  static strng norm_path(
    const char* path,
    delimiter delim = POSIX_DELIM,
    case_sensitivity cs = CASE_SENS
  );

  static strng current_path();

private:
  strng _path, _name, _extn;

  void norm_path_x( char* );      // ������������ ���� (������ '\\' �� '/')
  void parse( const char* );    // ������ ������� �����

  static strng denorm( const char* );
};

}; // namespace lwml

#endif // _FILENAME_
