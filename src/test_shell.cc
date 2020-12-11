#include "test_shell.h"
#include "help.h"
#include "scheduler.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <string>

int RunCommand(const std::string &command);

std::unique_ptr<Scheduler> scheduler;

int RunShell(const int input_file, const std::string &input_file_path,
             const int output_file, const std::string &output_file_path) {
  std::ifstream input_stream;
  std::ofstream output_stream;
  std::streambuf *cinbuf = std::cin.rdbuf(),
                 *coutbuf = std::cout.rdbuf(); // Backup original rdbuf
  if (input_file) {                            // Redirect cin
    input_stream.open(input_file_path);
    std::cin.rdbuf(input_stream.rdbuf());
  }
  if (output_file) { // Redirect cout
    output_stream.open(output_file_path);
    std::cout.rdbuf(output_stream.rdbuf());
  }
  std::ofstream stdoutput;
  stdoutput.basic_ios<char>::rdbuf(coutbuf); // Always output prompt to console
  scheduler = std::make_unique<Scheduler>(
      2, std::vector<std::pair<std::string, int>>{
             {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4}});
  std::string command;
  while (1) {
    if (!input_file)
      stdoutput << "shell> " << std::flush;
    if (!getline(std::cin, command))
      return 0;
    auto tmp = RunCommand(command);
    if (!output_file)
      std::cout << "\x1B[31m"; // Output error in red
    switch (tmp) {
    case 0:
      break;
    case -1:
      std::cout << "Command not found. Use ? for help." << std::endl;
      break;
    case -2:
      std::cout << "Wrong number of arguments. Use ? for help." << std::endl;
      break;
    case -3:
      std::cout << "Wrong argument(s). Use ? for help." << std::endl;
    }
    if (!output_file)
      std::cout << "\x1B[0m"; // Reset color
  }
}
int RunCommand(const std::string &command) {
  const static std::regex WSRegex("\\s+");    // Split by whitespace characters
  const static std::regex NUMRegex("[1-9]+"); // Split by whitespace characters
  std::vector<std::string> argv(
      std::sregex_token_iterator(command.cbegin(), command.cend(), WSRegex, -1),
      {});
  int argc = argv.size();
  if (!argc)
    return 0; // Just skip blank lines
  if (argv[0] == "init") {
    if (argc != 1)
      return -2; // Wrong number of arguments
    scheduler->init();
  } else if (argv[0] == "cr") {
    if (argc != 3)
      return -2;
    if (!std::regex_match(argv[2].begin(), argv[2].end(), NUMRegex))
      return -3; // Wrong argument format
    int priority = stoi(argv[2]);
    if (priority > scheduler->max_priority || priority <= 0)
      return -3;
    scheduler->cr(argv[1], priority);
  } else if (argv[0] == "de") {
    if (argc != 2)
      return -2;
    scheduler->de(argv[1]);
  } else if (argv[0] == "req") {
    if (argc != 2 && argc != 3)
      return -2;
    if (argc == 3 &&
        !std::regex_match(argv[2].begin(), argv[2].end(), NUMRegex))
      return -3;
    scheduler->req(argv[1], argc == 3?stoi(argv[3]): 1);
  } else if (argv[0] == "rel") {
    if (argc != 2 && argc != 3)
      return -2;
    if (argc == 3 &&
        !std::regex_match(argv[2].begin(), argv[2].end(), NUMRegex))
      return -3;
    scheduler->rel(argv[1], argc == 3?stoi(argv[3]): 1);
  } else if (argv[0] == "to") {
    if (argc != 1)
      return -2;
    scheduler->to();
  } else if (argv[0] == "list") {
    if (argc != 2)
      return -2;
    if (argv[1] == "ready")
      scheduler->list_ready();
    else if (argv[1] == "block")
      scheduler->list_block();
    else if (argv[1] == "res")
      scheduler->list_res();
    else
      return -3;
  } else if (argv[0] == "pr") {
    if (argc != 0)
      return -2;
    ; // Run "pr" here
  } else if (argv[0] == "?" || argv[0] == "h")
    PrintHelpCommands();
  else
    return -1;
  return 0;
}
