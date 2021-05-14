#include "export.hpp"

#include "common.hpp"

namespace fs = std::filesystem;

Export::Export(const fs::path& dir) {
  fs::create_directories(dir);
  dir_ = fs::canonical(dir);
  LOGI("Simulation output dir {}", dir_.c_str());
};
