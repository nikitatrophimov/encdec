#ifndef MAP_DECRYPTOR_H
#define MAP_DECRYPTOR_H

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <string>

// Структура, содержащая информацию по поводу "маппинга" символов
// согласно некоторому заранее заданному словарю
struct mapping
{
  mapping(
    char original
    , char mapped
  )
  : original(original)
  , mapped(mapped) {}

  char original;
  char mapped;
};

// Вспомогательные структуры для использования объектов структуры mapping в boost::multi_index_container
struct original {};
struct mapped {};

typedef boost::multi_index_container<
    mapping,
    boost::multi_index::indexed_by<
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<original>, BOOST_MULTI_INDEX_MEMBER(mapping, char, original)
      >,
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<mapped>, BOOST_MULTI_INDEX_MEMBER(mapping, char, mapped)
      >
  >
> mapping_set_t;

typedef boost::multi_index::index<mapping_set_t, original>::type mapping_set_ordered_by_original_index_t;
typedef boost::multi_index::index<mapping_set_t, mapped>::type mapping_set_ordered_by_mapped_index_t;

// Класс, реализующий алгоритм "маппинга"
class map_decryptor
{
public:
  map_decryptor()
  {
    // Создаём словарь
    _mapping.insert(mapping('a', 'z'));
    _mapping.insert(mapping('b', 'y'));
    _mapping.insert(mapping('c', 'x'));
    _mapping.insert(mapping('d', 'w'));
    _mapping.insert(mapping('e', 'v'));
    _mapping.insert(mapping('f', 'u'));
    _mapping.insert(mapping('g', 't'));
    _mapping.insert(mapping('h', 's'));
    _mapping.insert(mapping('i', 'r'));
    _mapping.insert(mapping('j', 'q'));
    _mapping.insert(mapping('k', 'p'));
    _mapping.insert(mapping('l', 'o'));
    _mapping.insert(mapping('m', 'n'));
    _mapping.insert(mapping('n', 'm'));
    _mapping.insert(mapping('o', 'l'));
    _mapping.insert(mapping('p', 'k'));
    _mapping.insert(mapping('q', 'j'));
    _mapping.insert(mapping('r', 'i'));
    _mapping.insert(mapping('s', 'h'));
    _mapping.insert(mapping('t', 'g'));
    _mapping.insert(mapping('u', 'f'));
    _mapping.insert(mapping('v', 'e'));
    _mapping.insert(mapping('w', 'd'));
    _mapping.insert(mapping('x', 'c'));
    _mapping.insert(mapping('y', 'b'));
    _mapping.insert(mapping('z', 'a'));
    _mapping.insert(mapping('!', ' '));
    _mapping.insert(mapping('@', '`'));
    _mapping.insert(mapping('#', '~'));
    _mapping.insert(mapping('$', '?'));
    _mapping.insert(mapping('%', '\''));
    _mapping.insert(mapping('^', '\\'));
    _mapping.insert(mapping('&', '='));
    _mapping.insert(mapping('*', '-'));
    _mapping.insert(mapping('(', '/'));
    _mapping.insert(mapping(')', ','));
    _mapping.insert(mapping('_', '.'));
    _mapping.insert(mapping('+', '+'));
    _mapping.insert(mapping('.', '_'));
    _mapping.insert(mapping(',', ')'));
    _mapping.insert(mapping('/', '('));
    _mapping.insert(mapping('-', '*'));
    _mapping.insert(mapping('=', '&'));
    _mapping.insert(mapping('\\', '^'));
    _mapping.insert(mapping('\'', '%'));
    _mapping.insert(mapping('?', '$'));
    _mapping.insert(mapping('~', '#'));
    _mapping.insert(mapping('`', '@'));
    _mapping.insert(mapping(' ', '!'));
  }

  // Функция расшифровки данных
  std::string decrypt(const std::string& input_str)
  {
    // Пробегаемся по каждому из символов входной строки
    // и ищем соответствие им в заранее созданном словаре
    // Если нашли, то заменяем его. Если нет, то оставляем его, как есть
    std::string res;
    for (char cur_input_char : input_str)
    {
      const mapping_set_ordered_by_mapped_index_t& index_mapped = boost::multi_index::get<mapped>(_mapping);
      const auto& itr = index_mapped.find(cur_input_char);
      if (itr == index_mapped.end())
      {
        res += cur_input_char;
      }
      else
      {
        res += itr->original;
      }
    }

    return res;
  }

private:
  mapping_set_t _mapping; // Словарь
};

#endif // !MAP_DECRYPTOR_H