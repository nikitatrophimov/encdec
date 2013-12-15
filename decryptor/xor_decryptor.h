#ifndef XOR_DECRYPTOR_H
#define XOR_DECRYPTOR_H

#include <string>

class xor_decryptor
{
public:
  std::string decrypt(const std::string& input_str, const std::string& key)
  {
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

#endif // !XOR_DECRYPTOR_H