#include "map_encryptor.h"
#include "reorder_encryptor.h"
#include "xor_encryptor.h"

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
    "algorithm,a"
    , boost::program_options::value<std::string>()
    , ("[REQUIRED] set encryption method (allowed values - "
        + XOR_ENCYPTION_METHOD_NAME + ", "
        + MAP_ENCYPTION_METHOD_NAME + ", "
        + REORDER_ENCYPTION_METHOD_NAME
      ).c_str()
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
    || !vm.count("algorithm")
    || (vm["algorithm"].as<std::string>() == XOR_ENCYPTION_METHOD_NAME && !vm.count("key"))
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

int main(int argc, char* argv[])
{
  // Парсим аргументы командной строки
  const boost::program_options::variables_map& command_line_arguments = parse_command_line_arguments(argc, argv);

  // Получаем содержимое входного файла, который необходимо зашифровать
  const std::string& input_file_content = get_file_content(
    command_line_arguments["input-file"].as<std::string>()
  );
  if (input_file_content.empty())
  {
    std::cerr << "Empty / invalid file \n";
    return EXIT_FAILURE;
  }

  // Получаем метод шифрования и ключ из аргументов командной строки
  const std::string& encyption_method = command_line_arguments["algorithm"].as<std::string>();

  // Создаём выходной файл для шифрованных данных
  boost::filesystem::ofstream output_file(command_line_arguments["output-file"].as<std::string>());
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
    const std::string& key = command_line_arguments["key"].as<std::string>();

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
