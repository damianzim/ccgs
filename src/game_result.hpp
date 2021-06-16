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

#ifndef CCGS_GAME_RESULT_HPP_
#define CCGS_GAME_RESULT_HPP_

#include <string_view>

class GameResult {
 public:
  using StatusType = int;

  struct FinalPlayerResult {
    std::string_view name;
    float strength;
  };

  using FinalResultType = FinalPlayerResult[2];

  enum : StatusType {
    kUninitialised,
    kInitialised,
    kInProgress,
    kFail,  //?
    kDraw,
    kDone,
  };

  // Get final result object.
  FinalResultType const& FinalResult() const;

  // Check whenever the game ended up with a correct result.
  bool Ok() const;

  // Return current status value.
  StatusType Status() const;

  // Get string corresponding to the `status_` value.
  std::string_view StatusStringify() const;

 protected:
  FinalPlayerResult final_[2]{};
  int status_{kUninitialised};
};

#endif  // CCGS_GAME_RESULT_HPP_
