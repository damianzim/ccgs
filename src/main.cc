#include <iostream>

#include "args.hpp"

int main(int argc, char* argv[]) {
  Args args;
  if (!args.ResolveArgs(argv)) {
    std::cerr << "Could not resolve arguments." << std::endl;
    return -1;
  }

  return 0;
}
