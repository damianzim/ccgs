#ifndef CCGS_ARGS_HPP_
#define CCGS_ARGS_HPP_

#include <map>
#include <set>
#include <string>

class Args {
 public:
  Args(){};

  const char* GetValue(const std::string& name) const;
  bool IsFlag(const std::string& name) const;
  bool IsOption(const std::string& name) const;

  bool ResolveArgs(const char* const* argv);

 private:
  std::set<std::string> flags_;
  std::map<std::string, const char*> options_;
};

#endif  // CCGS_ARGS_HPP_
