#include "map_decryptor.h"
#include "reorder_decryptor.h"
#include "xor_decryptor.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>

// Названия методов шифрования
const std::string XOR_ENCYPTION_METHOD_NAME = "xor";
const std::string MAP_ENCYPTION_METHOD_NAME = "map";
const std::string REORDER_ENCYPTION_METHOD_NAME = "reorder";

// Функция парсинга аргументов командной строки
std::map<std::string, std::string> parse_command_line_arguments(int argc, char* argv[])
{
  std::map<std::string, std::string> command_line_arguments;

  if (argc == 1
    || (argc == 2 && argv[1] == "--help")
  )
  {
    std::cout << "Usage: decryptor input_file output_file key \n";
    return command_line_arguments;
  }

  if (argc < 3)
  {
    std::cout << "Usage: decryptor input_file output_file key \n";
    return command_line_arguments;
  }

  command_line_arguments["input-file"] = argv[1];
  command_line_arguments["output-file"] = argv[2];
  if (argc == 4)
  {
    command_line_arguments["key"] = argv[3];
  }

  return command_line_arguments;
}

// Функция получения содержимого файла
std::string get_file_content(const std::string& file_path)
{
  std::string file_content;

  std::ifstream f(file_path);
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
  auto command_line_arguments = parse_command_line_arguments(argc, argv);
  if (command_line_arguments.empty())
  {
    return EXIT_SUCCESS;
  }

  // Получаем содержимое входного файла, который необходимо расшифровать
  const std::string& input_file_content = get_file_content(
    command_line_arguments["input-file"]
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
  if (argc == 4)
  {
    key = command_line_arguments["key"];
  }

  // Создаём выходной файл для расшифрованных данных
  std::ofstream output_file(command_line_arguments["output-file"]);
  if (!output_file)
  {
    std::cerr << "Unable to create output file \n";
    return EXIT_FAILURE;
  }

  // В зависимости от того или иного метода шифрования, которым был зашифрован входной файл,
  // создаём объект соответствующего класса и расшифровываем данные
  if (encyption_method == XOR_ENCYPTION_METHOD_NAME)
  {
    if (key.empty())
    {
      std::cerr << "You need to enter key for this method \n";
      return EXIT_FAILURE;
    }

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
    if (key.empty())
    {
      std::cerr << "You need to enter key for this method \n";
      return EXIT_FAILURE;
    }

    reorder_decryptor dec;
    const std::string& decrypted_str = dec.decrypt(
      part_of_file_content_to_encrypt
      , key
    );

    output_file << decrypted_str;
  }
}
