#ifndef CCGS_EXPORT_HPP_
#define CCGS_EXPORT_HPP_

#include <filesystem>

#include "common.hpp"

class Export {
 public:
  Export(const std::filesystem::path& dir);

 private:
  std::filesystem::path dir_;
};

#endif  // CCGS_EXPORT_HPP_
