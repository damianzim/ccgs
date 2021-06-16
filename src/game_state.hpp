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

#ifndef CCGS_GAME_STATE_HPP_
#define CCGS_GAME_STATE_HPP_

#include <string_view>

#include "game_result.hpp"

class GameState : public GameResult {
 public:
  // Check whenever the game's current state allows for the further playing.
  bool CanRun() const;

  // Set the current game state.
  void Set(StatusType state);

  // Set the final game result. `p1` is the winner or in case of a draw, the
  // order does not matter.
  void Set(const std::string_view& p1_name, float p1_strength,
           const std::string_view& p2_name, float p2_strength);
};

#endif  // CCGS_GAME_STATE_HPP_
