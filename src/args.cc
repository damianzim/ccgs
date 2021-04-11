#include "args.hpp"

#include <string.h>

const char* Args::GetValue(const std::string& name) const {
  auto search = options_.find(name);
  return search != options_.end() ? search->second : nullptr;
}

bool Args::IsFlag(const std::string& name) const { return flags_.count(name); }

bool Args::IsOption(const std::string& name) const {
  return options_.count(name);
}

bool Args::ResolveArgs(const char* const* argv) {
  flags_.clear();
  options_.clear();
  if (argv == nullptr || *argv == nullptr || **argv == '\0') return false;

  const char* next;
  for (const char* arg = *argv; arg != nullptr && *arg != '\0';
       arg = *(++argv)) {
    if (strncmp(arg, "--", 2) != 0) continue;
    arg += 2;
    if (*arg == '\0') continue;
    std::string name = arg;
    next = *(argv + 1);
    if (next == nullptr || *next == '\0') {
      flags_.insert(name);
      break;
    } else if (*next == '-' && *(next + 1) == '\0') {
      // Do not allow single '-' to be an argument value.
      continue;
    } else if (strncmp(next, "--", 2) == 0) {
      flags_.insert(name);
      continue;
    }
    // This assignment already handle the case, when we provide in the command
    // line multiple the same options (with different values) - in such case
    // always the last one provided will be considered.
    options_[name] = next;
    arg = *(++argv);
  }
  return true;
}
