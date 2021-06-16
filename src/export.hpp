//
// This file is part of the CCGS (https://github.com/damianzim/ccgs)
// distribution.
// Copyright (c) 2021 Piotr Jasik & Damian Zimon.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

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
