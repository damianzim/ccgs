#include <iostream>

#include "args.hpp"

static const char* prog;
[[noreturn]] void usage(int exit_status = EXIT_FAILURE);

int main(int argc, char* argv[]) {
  prog = argv[0];

  Args args;
  if (!args.ResolveArgs(argv)) {
    std::cerr << "Could not resolve arguments." << std::endl;
    return EXIT_FAILURE;
  }

  if (args.IsFlag("help")) usage();

  int balance = 0;
  if (args.IsOption("balance")) {
    balance = -1;
    try {
      balance = args.ParseOption<int>("balance", Args::CaptureInt);
    } catch (const std::invalid_argument& e) {
    } catch (const std::out_of_range& e) {
    }

    if (balance < 0 || balance > 5) {
      std::cerr << "Invalid value for --balance, see usage." << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

void usage(int exit_status) {
  std::cerr << prog << " [--help] [--balance BALANCE]\n" << std::endl;
  std::cerr
      << "optional arguments:\n"
      << "  --help            this help message\n"
      << "  --balance BALANCE integral number in the interval [0;5], default: 0"
      << std::endl;
  exit(exit_status);
}
