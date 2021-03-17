#ifndef CCGS_ARGS_HPP_
#define CCGS_ARGS_HPP_

#include <string.h>

#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include "common.hpp"

class MissingArgumentException : public std::runtime_error {
  std::string msg_{"Missing argument"};

 public:
  explicit MissingArgumentException(const std::string& arg_name = "")
      : std::runtime_error("") {
    if (arg_name.size()) msg_ += " --" + arg_name;
  }

  const char* what() const noexcept override { return msg_.c_str(); }
};

class Args {
 public:
  Args(){};

  static inline int CaptureInt(const char* arg) { return std::stoi(arg, 0, 0); }
  static inline std::string CaptureString(const char* arg) {
    return std::string(arg);
  }
  static inline unsigned long CaptureUnsignedLong(const char* arg) {
    return std::stoul(arg, 0, 0);
  }

  bool IsOption(const std::string& name) const { return options_.count(name); }
  bool IsFlag(const std::string& name) const { return flags_.count(name); }

  template <typename T>
  T ParseOption(const std::string& name,
                std::function<T(const char*)> capture) const {
    auto search = options_.find(name);
    if (search == options_.end()) throw MissingArgumentException(name);
    return capture(search->second);
  }

  bool ResolveArgs(char** argv) {
    flags_.clear();
    options_.clear();
    if (argv == nullptr || *argv == nullptr || **argv == '\0') return false;

    char* next;
    for (char* arg = *argv; arg != nullptr && *arg != '\0'; arg = *(++argv)) {
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

 private:
  std::set<std::string> flags_;
  std::map<std::string, char*> options_;
};

#endif  // CCGS_ARGS_HPP_
