#include "map_decryptor.h"
#include "reorder_decryptor.h"
#include "xor_decryptor.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/program_options.hpp>

#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>

// Названия методов шифрования
const std::string XOR_ENCYPTION_METHOD_NAME = "xor";
const std::string MAP_ENCYPTION_METHOD_NAME = "map";
const std::string REORDER_ENCYPTION_METHOD_NAME = "reorder";

// Функция парсинга аргументов командной строки
boost::program_options::variables_map parse_command_line_arguments(int argc, char* argv[])
{
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
  (
    "help,h"
    , "produce help message"
  )
  (
    "input-file,i"
    , boost::program_options::value<std::string>()
    , "[REQUIRED] set input file"
  )
  (
    "output-file,o"
    , boost::program_options::value<std::string>()
    , "[REQUIRED] set output file"
  )
  (
    "key,k"
    , boost::program_options::value<std::string>()
    , ("set key (for " + XOR_ENCYPTION_METHOD_NAME
        + " method only"
      ).c_str()
  );

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);

  if (vm.count("help")
    || !vm.count("input-file")
    || !vm.count("output-file")
  )
  {
    std::cout << desc << '\n';
    std::exit(EXIT_FAILURE);
  }

  return vm;
}

// Функция получения содержимого файла
std::string get_file_content(const boost::filesystem::path& file_path)
{
  std::string file_content;

  boost::filesystem::ifstream f(file_path);
  if (f)
  {
    file_content.append(
      (std::istreambuf_iterator<char>(f))
      , std::istreambuf_iterator<char>()
    );
  }

  return file_content;
}

// Функция определения метода шифрования по первому символу входной строки
std::string get_encryption_method(const std::string& input_file_content)
{
  if (input_file_content.empty())
  {
    return std::string();
  }

  if (input_file_content[0] == 'x')
  {
    return XOR_ENCYPTION_METHOD_NAME;
  }
  else if (input_file_content[0] == 'm')
  {
    return MAP_ENCYPTION_METHOD_NAME;
  }
  else if (input_file_content[0] == 'r')
  {
    return REORDER_ENCYPTION_METHOD_NAME;
  }

  return std::string();
}

int main(int argc, char* argv[])
{
  // Парсим аргументы командной строки
  const boost::program_options::variables_map& command_line_arguments = parse_command_line_arguments(argc, argv);

  // Получаем содержимое входного файла, который необходимо расшифровать
  const std::string& input_file_content = get_file_content(
    command_line_arguments["input-file"].as<std::string>()
  );
  if (input_file_content.empty())
  {
    std::cerr << "Empty / invalid file \n";
    return EXIT_FAILURE;
  }

  // Получаем метод шифрования, который использовался для зишфровки входного файла
  const std::string& encyption_method = get_encryption_method(input_file_content);
  if (encyption_method.empty())
  {
    std::cerr << "Unknown encryption method \n";
    return EXIT_FAILURE;
  }

  // Получаем строку, представляющую собой содержмое входного файла без первого символа
  const std::string& part_of_file_content_to_encrypt = input_file_content.substr(1);
  std::cout << part_of_file_content_to_encrypt << '\n';

  // Получаем ключ из аргументов командной строки
  std::string key;
  if (command_line_arguments.count("key"))
  {
    key = command_line_arguments["key"].as<std::string>();
  }

  // Создаём выходной файл для расшифрованных данных
  boost::filesystem::ofstream output_file(command_line_arguments["output-file"].as<std::string>());
  if (!output_file)
  {
    std::cerr << "Unable to create output file \n";
    return EXIT_FAILURE;
  }

  // В зависимости от того или иного метода шифрования, которым был зашифрован входной файл,
  // создаём объект соответствующего класса и расшифровываем данные
  if (encyption_method == XOR_ENCYPTION_METHOD_NAME)
  {
    xor_decryptor dec;
    const std::string& decrypted_str = dec.decrypt(
      part_of_file_content_to_encrypt
      , key
    );

    output_file << decrypted_str;
  }
  else if (encyption_method == MAP_ENCYPTION_METHOD_NAME)
  {
    map_decryptor dec;
    const std::string& decrypted_str = dec.decrypt(
      part_of_file_content_to_encrypt
    );

    output_file << decrypted_str;
  }
  else if (encyption_method == REORDER_ENCYPTION_METHOD_NAME)
  {
    reorder_decryptor dec;
    const std::string& decrypted_str = dec.decrypt(
      part_of_file_content_to_encrypt
      , key
    );

    output_file << decrypted_str;
  }
}
