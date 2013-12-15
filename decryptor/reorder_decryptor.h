#ifndef REORDER_DECRYPTOR_H
#define REORDER_DECRYPTOR_H

#include <boost/lexical_cast.hpp>

#include <ctime>
#include <set>
#include <string>

class reorder_decryptor
{
public:
  std::string decrypt(const std::string& input_str, const std::string& key)
  {
    std::string res;

    for (
      std::string::size_type i = 0, input_str_size = input_str.size();
      i < input_str_size;
      i += key.size()
    )
    {
      const std::string& cur_substr = input_str.substr(i, key.size());
      for (int i = 0; i < key.size(); ++i)
      {
        const auto& char_index = key.find(boost::lexical_cast<char>(i));
        res += cur_substr[char_index];
      }
    }

    return res;
  }
};

#endif // !REORDER_DECRYPTOR_H