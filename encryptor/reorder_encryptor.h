#ifndef REORDER_ENCRYPTOR_H
#define REORDER_ENCRYPTOR_H

#include <ctime>
#include <set>
#include <string>

// Класс, реализующий алгоритм перестановки
class reorder_encryptor
{
public:
  // Функция зашифровки данных
  std::string encrypt(const std::string& input_str, const std::string& key)
  {
    // - Пробегаемся по каждому из "блоков" входной строки (длина блока равна длине ключа)
    // - На каждой итерации пробегаемся по всем символам ключа и расставляем
    // символы в соответствии с цифрами, указанными в ключе
    std::string res;

    for (
      std::string::size_type i = 0, input_str_size = input_str.size();
      i < input_str_size;
      i += key.size()
    )
    {
      const std::string& cur_substr = input_str.substr(i, key.size());
      for (const char c : key)
      {
        res += c - '0';
      }
    }

    return res;
  }

  // Функция генерации случайного ключа
  std::string generate_key(int key_size)
  {
    std::srand(std::time(0));
    std::string key;
    std::set<int> helper;
    while (true)
    {
      int value = std::rand() % key_size;
      const auto& res = helper.insert(value);
      if (res.second)
      {
        key += (char)(((int)'0') + value);
      }
      if (helper.size() == key_size)
      {
        break;
      }
    }

    return key;
  }
  
  // Функция определения длины оптимального ключа для данного алгоритма
  int calculate_key_size(const std::string& input_str)
  {
    for (int i = 10; i != 0; --i)
    {
      if (!(input_str.size() % i))
      {
        return i;
      }
    }
  }
};

#endif // !REORDER_ENCRYPTOR_H