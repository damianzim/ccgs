#ifndef CCGS_EXPORT_HPP_
#define CCGS_EXPORT_HPP_

#include <filesystem>
#include <fstream>

#include "libcsv/builder.h"
#include "libcsv/type.h"
#include "rapidjson/document.h"
#include "common.hpp"

class GameParams;
class GameResult;

enum class ExportRowLabel {
  kIn,
  kCard,
  kOut,
};

class Export {
 public:
  Export(std::filesystem::path dir);
  ~Export();

  void DumpFinalResult(const GameResult& result);
  void DumpGameParams(const GameParams& params);

  template <typename... Args>
  void PushRow(Args&&... args) {
    csv_.BuildRow(std::forward<Args>(args)...);
  }

 private:
  inline rapidjson::Document::AllocatorType& Alloc() {
    return json_.GetAllocator();
  }

  csv::CsvBuilder<
      int, int, const char*, const char*, csv::type::Opt<int>,
      csv::type::Opt<int>, csv::type::Opt<int>,
      csv::type::Opt<csv::type::Float<2>>, csv::type::Opt<int>,
      csv::type::Opt<int>, csv::type::Opt<int>, csv::type::Opt<csv::type::Bool>,
      csv::type::Opt<csv::type::Bool>, csv::type::Opt<csv::type::Bool>,
      csv::type::Opt<csv::type::Bool>, csv::type::Opt<csv::type::Bool>,
      csv::type::Opt<csv::type::Bool>>
      csv_{{"turn", "subturn", "player_name", "stage", "controlled",
            "discarded", "in_hand", "strength", "attr.water", "attr.fire",
            "attr.nature", "trait.swift", "trait.symbiotic", "trait.poisonous",
            "trait.empowering", "trait.sabotaging", "trait.supporting"}};
  std::ofstream csv_out_;

  rapidjson::Document json_;
  std::ofstream json_out_;
};

#endif  // CCGS_EXPORT_HPP_
