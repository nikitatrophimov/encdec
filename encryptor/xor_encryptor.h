#ifndef XOR_ENCRYPTOR_H
#define XOR_ENCRYPTOR_H

#include <string>

class xor_encryptor
{
public:
  std::string encrypt(const std::string& input_str, const std::string& key)
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

#endif // !XOR_ENCRYPTOR_H