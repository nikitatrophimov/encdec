#include "map_encryptor.h"
#include "reorder_encryptor.h"
#include "xor_encryptor.h"

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
    std::cout << "Usage: encryptor input_file output_file algorithm key \n";
    return command_line_arguments;
  }

  if (argc < 4)
  {
    std::cout << "Usage: encryptor input_file output_file algorithm key \n";
    return command_line_arguments;
  }

  command_line_arguments["input-file"] = argv[1];
  command_line_arguments["output-file"] = argv[2];
  command_line_arguments["algorithm"] = argv[3];
  if (argc == 5)
  {
    command_line_arguments["key"] = argv[4];
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

int main(int argc, char* argv[])
{
  // Парсим аргументы командной строки
  auto command_line_arguments = parse_command_line_arguments(argc, argv);

  // Получаем содержимое входного файла, который необходимо зашифровать
  const std::string& input_file_content = get_file_content(
    command_line_arguments["input-file"]
  );
  if (input_file_content.empty())
  {
    std::cerr << "Empty / invalid file \n";
    return EXIT_FAILURE;
  }

  // Получаем метод шифрования и ключ из аргументов командной строки
  const std::string& encyption_method = command_line_arguments["algorithm"];

  // Создаём выходной файл для шифрованных данных
  std::ofstream output_file(command_line_arguments["output-file"]);
  if (!output_file)
  {
    std::cerr << "Unable to create output file \n";
    return EXIT_FAILURE;
  }

  // В зависимости от того или иного метода шифрования, который выбрал пользователь,
  // создаём объект соответствующего класса и зашифровываем данные
  // В начало выходного файла добавляем букву, обозначающую то, какой именно
  // алгоритм шифрования использовался для зашифровки данного текста, чтобы дешифровщик
  // мог автоматически определить алгоритм
  if (encyption_method == XOR_ENCYPTION_METHOD_NAME)
  {
    const std::string& key = command_line_arguments["key"];
    if (key.empty())
    {
      std::cerr << "You need to enter key for this method \n";
      return EXIT_FAILURE;
    }

    xor_encryptor enc;
    const std::string& encrypted_str = enc.encrypt(
      input_file_content
      , key
    );

    output_file << 'x' << encrypted_str;
  }
  else if (encyption_method == MAP_ENCYPTION_METHOD_NAME)
  {
    map_encryptor enc;
    const std::string& encrypted_str = enc.encrypt(
      input_file_content
    );

    output_file << 'm' << encrypted_str;
  }
  else if (encyption_method == REORDER_ENCYPTION_METHOD_NAME)
  {
    reorder_encryptor enc;

    // В случае перестановки мы генерируем ключ самостоятельно
    int key_size = enc.calculate_key_size(input_file_content);
    // Если размер ключа слишком маленький, то завершаем приложение с кодом ошибки,
    // иначе дешифровать текст будет не так уж и сложно
    if (key_size < 3)
    {
      std::cerr << "Unable to encrypt this text. Please use another method \n";
      return EXIT_FAILURE;
    }
    const std::string& key = enc.generate_key(key_size);
    std::cout << "Key: " << key << '\n';

    const std::string& encrypted_str = enc.encrypt(
      input_file_content
      , key
    );

    output_file << 'r' << encrypted_str;
  }
}
