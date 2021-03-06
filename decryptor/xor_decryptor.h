﻿#ifndef XOR_DECRYPTOR_H
#define XOR_DECRYPTOR_H

#include <string>

// Класс, реализующий XOR-алгоритм
class xor_decryptor
{
public:
  // Функция расшифровки данных
  std::string decrypt(const std::string& input_str, const std::string& key)
  {
    // Пробегаемся по каждому из символов входной строки
    // и применяем к ним XOR с текущим символом ключа
    std::string res;
    std::string::size_type cur_key_index = 0;
    bool need_to_convert_next_char = true;
    for (char cur_input_char : input_str)
    {
      if (cur_input_char == ':')
      {
        need_to_convert_next_char = false;
        continue;
      }

      if (!need_to_convert_next_char)
      {
        res += cur_input_char;
        need_to_convert_next_char = true;
      }
      else
      {
        res += cur_input_char ^ key[cur_key_index];
      }

      if (++cur_key_index == key.size())
      {
        cur_key_index = 0;
      }
    }

    return res;
  }
};

#endif // !XOR_DECRYPTOR_H