#ifndef MAP_ENCRYPTOR_H
#define MAP_ENCRYPTOR_H

#include <map>
#include <string>

// Класс, реализующий алгоритм "маппинга"
class map_encryptor
{
public:
  map_encryptor()
  {
    // Создаём словарь
    _mapping.insert(std::make_pair('z', 'a'));
    _mapping.insert(std::make_pair('y', 'b'));
    _mapping.insert(std::make_pair('x', 'c'));
    _mapping.insert(std::make_pair('w', 'd'));
    _mapping.insert(std::make_pair('v', 'e'));
    _mapping.insert(std::make_pair('u', 'f'));
    _mapping.insert(std::make_pair('t', 'g'));
    _mapping.insert(std::make_pair('s', 'h'));
    _mapping.insert(std::make_pair('r', 'i'));
    _mapping.insert(std::make_pair('q', 'j'));
    _mapping.insert(std::make_pair('p', 'k'));
    _mapping.insert(std::make_pair('o', 'l'));
    _mapping.insert(std::make_pair('n', 'm'));
    _mapping.insert(std::make_pair('m', 'n'));
    _mapping.insert(std::make_pair('l', 'o'));
    _mapping.insert(std::make_pair('k', 'p'));
    _mapping.insert(std::make_pair('j', 'q'));
    _mapping.insert(std::make_pair('i', 'r'));
    _mapping.insert(std::make_pair('h', 's'));
    _mapping.insert(std::make_pair('g', 't'));
    _mapping.insert(std::make_pair('f', 'u'));
    _mapping.insert(std::make_pair('e', 'v'));
    _mapping.insert(std::make_pair('d', 'w'));
    _mapping.insert(std::make_pair('c', 'x'));
    _mapping.insert(std::make_pair('b', 'y'));
    _mapping.insert(std::make_pair('a', 'z'));
    _mapping.insert(std::make_pair(' ', '!'));
    _mapping.insert(std::make_pair('`', '@'));
    _mapping.insert(std::make_pair('~', '#'));
    _mapping.insert(std::make_pair('?', '$'));
    _mapping.insert(std::make_pair('\'', '%'));
    _mapping.insert(std::make_pair('\\', '^'));
    _mapping.insert(std::make_pair('=', '&'));
    _mapping.insert(std::make_pair('-', '*'));
    _mapping.insert(std::make_pair('/', '('));
    _mapping.insert(std::make_pair(',', ')'));
    _mapping.insert(std::make_pair('.', '_'));
    _mapping.insert(std::make_pair('+', '+'));
    _mapping.insert(std::make_pair('_', '.'));
    _mapping.insert(std::make_pair(')', ','));
    _mapping.insert(std::make_pair('(', '/'));
    _mapping.insert(std::make_pair('*', '-'));
    _mapping.insert(std::make_pair('&', '='));
    _mapping.insert(std::make_pair('^', '\\'));
    _mapping.insert(std::make_pair('%', '\''));
    _mapping.insert(std::make_pair('$', '?'));
    _mapping.insert(std::make_pair('#', '~'));
    _mapping.insert(std::make_pair('@', '`'));
    _mapping.insert(std::make_pair('!', ' '));
  }

  // Функция зашифровки данных
  std::string encrypt(const std::string& input_str)
  {
    // Пробегаемся по каждому из символов входной строки
    // и ищем соответствие им в заранее созданном словаре
    // Если нашли, то заменяем его. Если нет, то оставляем его, как есть
    std::string res;
    for (char cur_input_char : input_str)
    {
      const auto& itr = _mapping.find(cur_input_char);
      if (itr == _mapping.end())
      {
        res += cur_input_char;
      }
      else
      {
        res += itr->second;
      }
    }

    return res;
  }

private:
  std::map<char, char> _mapping; // Словарь
};

#endif // !MAP_ENCRYPTOR_H