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

#ifndef CCGS_GAME_HPP_
#define CCGS_GAME_HPP_

#include <memory>

#include "export.hpp"
#include "game_state.hpp"
#include "table.hpp"

class GameParams;

class Game {
 public:
  // Construct game object with the `params` parameters. For playing must be
  // initialised with `InitGame()` method.
  Game(const GameParams& params);

  // Initalise game which output will be saved in the `output_dir` (must contain
  // valid string or will fail) directory. Create table and assigned to it
  // players with decks.
  bool InitGame(const char* output_dir);

  // Run the simulation until the end.
  GameResult Run();

 private:
  const GameParams& params_;

  std::unique_ptr<Export> export_;
  std::unique_ptr<Table> table_;
  GameState state_;
};

#endif  // CCGS_GAME_HPP_
