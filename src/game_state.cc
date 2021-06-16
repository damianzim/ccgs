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

#include "game_state.hpp"

bool GameState::CanRun() const {
  return status_ == kInProgress || status_ == kInitialised;
}

void GameState::Set(StatusType state) { status_ = state; }
void GameState::Set(const std::string_view& p1_name, const float p1_strength,
                    const std::string_view& p2_name, const float p2_strength) {
  final_[0].name = p1_name;
  final_[0].strength = p1_strength;
  final_[1].name = p2_name;
  final_[1].strength = p2_strength;
}
