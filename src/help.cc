#include "help.h"
#include <iostream>

const std::string kHelpUsage =
    "A Simple process and resource management emulator for study.\n\n"
    "Usage:\n\n"
    "  prme  [-i <file>] [-o <file>] [-h]\n\n"
    "Options:\n\n"
    "  -i, --input <file>\n"
    "    Read commands form <file>.\n\n"
    "  -o, --output <file>\n"
    "    Write outputs to <file>.\n\n"
    "  -h, --help\n"
    "    Display this usage information and exits.\n";

const std::string kHelpCommands =
    "Supported Commands:\n\n"
    "  init\n"
    "    init.\n\n"
    "  cr <name> <priority>(= 1 or 2)\n"
    "    create process.\n\n"
    "  de <name>\n"
    "    delete process.\n\n"
    "  req <resource name> <#of units>\n"
    "    request resource.\n\n"
    "  rel <resource name> <#of units>\n"
    "    release resource.\n\n"
    "  to\n"
    "    time out.\n\n"
    "  list ready\n"
    "    list all processes in the ready queue.\n\n"
    "  list block\n"
    "    list all processes in the block queue.\n\n"
    "  list res\n"
    "    list all available resources.\n\n"
    "  pr <name>\n"
    "    print pcb information about a given process.\n\n"
    "  ?, h\n"
    "    show this help.\n\n"
    "  <EOF>, <SIGINT> (Ctrl + D, Ctrl + C in an ANSI terminal)\n"
    "    exit.\n\n";

void PrintHelp(int error) {
  auto &out = error ? std::cerr : std::cout;
  out << kHelpUsage << std::endl << kHelpCommands << std::endl;
  std::exit(error);
}

void PrintHelpCommands() {
  std::cout << kHelpCommands << std::endl;
}
