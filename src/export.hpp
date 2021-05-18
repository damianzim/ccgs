#ifndef CCGS_EXPORT_HPP_
#define CCGS_EXPORT_HPP_

#include <filesystem>
#include <fstream>

#include "common.hpp"
#include "rapidjson/document.h"

class GameParams;
class GameResult;

class Export {
 public:
  Export(std::filesystem::path dir);
  ~Export();

  void DumpFinalResult(const GameResult& result);
  void DumpGameParams(const GameParams& params);

 private:
  inline rapidjson::Document::AllocatorType& Alloc() {
    return json_.GetAllocator();
  }

  rapidjson::Document json_;
  std::ofstream json_out_;
};

#endif  // CCGS_EXPORT_HPP_
