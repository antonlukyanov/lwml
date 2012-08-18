// Обобщенный динамический массив
// lwml, (c) ltwood

#ifndef _T_LIST_
#define _T_LIST_

#include "defs.h"
#include "stdmem.h"
#include "basex.h"
#include "lwmlconf.h"

/*#lake:stop*/

namespace lwml {

// Класс реализует простой обобщенный список, реализованный
// посредством массива с переменной длиной.
template<typename T>
class t_list : public value {
public:
  enum pack_mode { AUTO_PACK, MANUAL_PACK };

  // Создание пустого списка.
  // Аргумент определяет политику распределения памяти.
  // При autopack = false занимаемая память будет только
  // расти и упаковку списка необходимо производить вручную (см. pack()).
  t_list( pack_mode pm = AUTO_PACK )
    : _size(0), _len(0),
      _autopack(pm == AUTO_PACK),
      _data(0)
  {}

  // Копирование списка.
  t_list( const t_list<T>& src ){
    _data = raw_alloc<T>(_size = src._size);
    _len = src._len;
    _autopack = src._autopack;
    for( int j = 0; j < _len; j++ )
      new(_data + j) T(src[j]);
  }

  // Разрушение списка.
  ~t_list(){
    destroy(_data, _len);
    raw_free<T>(_data);
  }

  // Копирование списков.
  t_list<T>& operator=( const t_list<T>& src ){
    if( &src == this )
      return *this;

    destroy(_data, _len);
    raw_free<T>(_data);
    _data = raw_alloc<T>(_size = src._size);
    _len = src._len;
    _autopack = src._autopack;
    for( int j = 0; j < _len; j++ )
      new(_data + j) T(src[j]);
    return *this;
  }

  // Получение размера зарезервированного списком массива.
  int  size() const     { return _size; }

  // Получение текущего размера списка.
  int  len() const      { return _len; }

  // Проверка списка на пустоту.
  bool is_empty() const { return _len == 0; }

  // Операторы индексирования.
  T& operator[]( int j ) {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[j];
  }
  const T& operator[]( int j ) const {
    #if _CHECK_IDX_
      test_index(j, _len);
    #endif
    return _data[j];
  }

  // Добавить el в конец списка.
  // Возвращает позицию вставленного элемента.
  int put( const T& el ) {
    int pos = inclen();
    new(_data + pos) T(el);
    return pos;
  }

  // Получить копию последнего элемента списка и удалить его
  // (удаляемый элемент возвращается по значению).
  T get() {
    if( _len == 0 )
      fail_assert("empty t_list");
    int pos = _len - 1;
    T tmp = _data[pos];
    (_data + pos)->~T();
    --_len;
    if( _autopack )
      pack();
    return tmp;
  }

  // удалить элемент в позиции j.
  void del( int pos ){
    test_index(pos, _len);
    for( int j = pos+1; j < _len; j++ )
      _data[j-1] = _data[j];
    (_data + _len - 1)->~T();
    --_len;
    if( _autopack )
      pack();
  }

  // Вставить el в позицию j.
  // Позиция вставки может принимать значение, равное длине списка.
  // В этом случае производится вставка элемента в конец списка т.е. фактически операция put().
  void ins( int pos, const T& el ){
    test_index(pos, _len+1);
    int new_pos = inclen();
    new(_data + new_pos) T();
    for( int j = _len-1; j > pos; j-- )
      _data[j] = _data[j-1];
    _data[pos] = el;
  }

  // Очистка списка.
  void clear(){
    destroy(_data, _len);
    raw_free<T>(_data);
    _size = _len = 0;
    _data = 0;
  }

  // Низкоуровневый доступ к "сырым" данным.
  const T* access_raw_data() const { return _data; }

  // Попытка уменьшить распределенную память
  void pack(){
    if( _size >= 64 && _size - _len > _len )
      resize(_len + _len/2);
  }

  int comp( int i, int j ) const {
    return (_data[i] < _data[j]) ? -1 : (_data[i] > _data[j]) ? 1 : 0;
  }

private:
  int   _size;
  int   _len;
  bool  _autopack;
  T    *_data;

  void destroy( T* x, int n ){
    for( int j = 0; j < n; j++ )
      (x + j)->~T();
  }

  void resize( int new_size ){
    T* old_data = _data;
    int old_size = _size;

    _data = raw_alloc<T>(_size = new_size);
    for( int j = 0; j < _len; j++ )
      new(_data + j) T(old_data[j]);

    destroy(old_data, old_size);
    raw_free<T>(old_data);
  }

  int inclen()
  {
    if( _len == _size ){
      int delta = (_size >= 64) ? _size / 2 : 16;
      resize(_size + delta);
    }
    return _len++;
  }
};

}; // namespace lwml

#endif // _T_LIST_
