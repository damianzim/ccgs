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

#include "game_result.hpp"

GameResult::FinalResultType const& GameResult::FinalResult() const {
  return final_;
}

bool GameResult::Ok() const { return status_ == kDone || status_ == kDraw; }

GameResult::StatusType GameResult::Status() const { return status_; }

std::string_view GameResult::StatusStringify() const {
  switch (status_) {
    case kUninitialised:
      return "uninitialised";
    case kInitialised:
      return "initialised";
    case kInProgress:
      return "in progress";
    case kFail:
      return "fail";
    case kDraw:
      return "draw";
    case kDone:
      return "done";
    default:
      return "unknown";
  }
}
