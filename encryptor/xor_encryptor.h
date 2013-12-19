#ifndef XOR_ENCRYPTOR_H
#define XOR_ENCRYPTOR_H

#include <string>

// Класс, реализующий XOR-алгоритм
class xor_encryptor
{
public:
  // Функция зашифровки данных
  std::string encrypt(const std::string& input_str, const std::string& key)
  {
    // Пробегаемся по каждому из символов входной строки
    // и применяем к ним XOR с текущим символом ключа
    std::string res;
    std::string::size_type cur_key_index = 0;
    for (char cur_input_char : input_str)
    {
      res += cur_input_char ^ key[cur_key_index];

      if (++cur_key_index == key.size())
      {
        cur_key_index = 0;
      }
    }

    return res;
  }
};

#endif // !XOR_ENCRYPTOR_H