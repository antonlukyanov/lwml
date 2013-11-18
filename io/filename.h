// Работа с именами файлов
// lwml, (c) ltwood

#ifndef _FILENAME_
#define _FILENAME_

#include "defs.h"
#include "cstrng.h"
#include "basex.h"

/*#lake:stop*/

namespace lwml {

// Исключение при попытке доступа к несуществующему файлу

DEF_EX_TYPE_MSG(ex_base, ex_nosuchfile, "no such file");

// Класс инкапсулирует разбор имени файла
// Полное имя файла разбивается на путь, имя и расширение
// Каждая часть полного имени хранится в виде объекта класса strng
// чем и определяются ограничения на длину
// При разборе путем считается часть имени файла с начала до
// последнего слэша включительно
// Расширением становится часть имени от последней точки включительно
// При разборе имени вида D:NAME.EXT именем становитс строка D:NAME
// Функции set_xxx() позволяют заменить отдельные составные части полного имени
// Соответствующие функции с именами xxx() возвращают составние части имени
// Функция pne() возвращает полное (абсолютное) имя файла
// Функция ne() возвращает относительное (без пути) имя файла
//
// Класс содержит набор статических функций для выполнения часто используемых действий
// Функция chext() заменяет расширение файла
// Функция chrelname() заменяет относительное имя файла (все кроме пути)
// Функция index() генерирует имя файла, добавляя в конец переданного имени
// числовой индекс (параметр width управляет разрядностью представления индекса)
// Функция suffix() аналогична функции index(), но добавляет строковый суффикс,
// отделяя его от имени дефисом
//
// В отдельную группу можно выделить функции, работающие непосредственно
// с файловой системой.
// Эти функции могут работать как с нормализованными,
// так и с ненормализованными именами объектов.
// В том числе имеется в виду, что на конце имени может быть расположен
// разделитель элементов пути (косая черта), который игнорируется
// и не интерпретируется как исключительный аттрибут директорий.
// Статическая функция is_exists() проверяет, существует ли файл с заданным именем.
// Метод is_exists() проверяет, существует ли файл с именем, хранящимся в объекте.
// Статическая функция remove() удаляет файл с заданным именем.
// Метод remove() удаляет файл с именем, хранящимся в объекте.
// Статическая функция rename() переименовывает файл с именем, заданным
// в первом аргументе, изменяя его имя на заданное во втором аргументе.
// Метод rename() переименовывает файл с именем, хранящимся в объекте,
// изменяя его имя на заданное.
// Статическая функция mod_time() возвращает время модификации
// для файла с заданным именем.
// Метод mod_time() возвращает время модификации для файла с именем,
// хранящимся в объекте.
// Статическая функция norm_path() производит нормировку пути.
// После нормировки путь всегда оканчивается на слэш, а все слэши
// в пути оказываются одинаковыми.
// По умолчанию используется прямой слэш ('/'), но если во втором аргументе
// (use_backslash) передано значение true, то используется обратный слэш ('\').
// По умолчанию файловая система считается регистро-зависимой.
// Для регистронезависимых систем можно передать false в параметре case_sens
// и тогда результат нормализации будет сконвертирован в нижний регистр.
// Функция current_path() возвращает ненормализованный путь к текущей директории.

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

  // Методы, работающие с файловой системой.

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

  void norm_path_x( char* );      // нормализация пути (замена '\\' на '/')
  void parse( const char* );    // разбор полного имени

  static strng denorm( const char* );
};

}; // namespace lwml

#endif // _FILENAME_
