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
  // Construct an object of the export. Create directory `dir` if it does not
  // exist. Create `dir`/simulation.json and `dir`/simulation.csv files for
  // appropriate outputs if they do not exsit, otherwise wipe them. Open the
  // streams for the output files. The constructor may throw execeptions that
  // comes from the filesytem functions.
  Export(std::filesystem::path dir);

  // Dump the simulation.csv and simulation.json files and close the streams.
  ~Export();

  // Dump the json containing the final game result to the memory.
  void DumpFinalResult(const GameResult& result);

  // Dump the json containing game params and their values to the memory.
  void DumpGameParams(const GameParams& params);

  // Dump csv row to the output file.
  template <typename... Args>
  void PushRow(Args&&... args) {
    csv_.BuildRow(std::forward<Args>(args)...);
  }

 private:
  // Get a reference to the allocator of the json file currently beign built.
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
