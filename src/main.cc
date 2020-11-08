#include "help.h"
#include "test_shell.h"
#include <string>

int main(int argc, char *argv[]) {
  bool input_file = 0, output_file = 0;
  std::string input_file_path, output_file_path;
  for (int i = 1; i < argc; ++i) {
    std::string tmp = argv[i]; // Convert args to std::string
    if (tmp == "-h" || tmp == "--help")
      PrintHelp();
    else if (tmp == "-i" || tmp == "--input") {
      if (input_file)
        PrintHelp(-3); // Duplicate argument
      input_file = 1, ++i;
      if (i >= argc)
        PrintHelp(-2); // No <file> for -i
      input_file_path = argv[i];
    } else if (tmp == "-o" || tmp == "--output") {
      if (output_file)
        PrintHelp(-3); // Duplicate argument
      if (i >= argc)
        PrintHelp(-2); // No <file> for -o
      output_file = 1, ++i;
      output_file_path = argv[i];
    } else
      PrintHelp(-1); // Wrong argument format
  }
  return RunShell(input_file, input_file_path, output_file, output_file_path);
}
