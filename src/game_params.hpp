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

#ifndef CCGS_GAME_PARAMS_HPP_
#define CCGS_GAME_PARAMS_HPP_

#include <optional>
#include <string>

#include "args.hpp"

class GameParams {
 public:
  // Return GameParams object. Look for valid game parameters in the `args`
  //  object and try to parse them. If some parameter is present but contains
  // invalid or illicit value at once return nullopt. By default the parameters
  // adopt default values.
  static std::optional<GameParams> Parse(const Args& args);

  int Balance() const { return balance_; }
  int DeckSize() const { return deck_size_; }
  int PoolSize() const { return pool_size_; }
  int StartingCards() const { return starting_cards_; }
  int TurnLimit() const { return turn_limit_; }
  int PointsLeading() const { return points_leading_; }

 private:
  class InvalidParamValue;

  // Construct GameParams object with uninitialised parameter values.
  GameParams(){};

  // Look for and parse game parameter `name` in `args`. Parsed parameter's
  // value should be in [min, max], otherwise throw
  // GameParams::InvalidParamValue exception. If the parameter is not present
  // return `default_value`.
  static int ParseParam(const Args& args, const std::string& name,
                        const int min, const int max, const int& default_value);

  int balance_;
  int deck_size_;
  int pool_size_;
  int starting_cards_;
  int turn_limit_;
  int points_leading_;
};

#endif  // CCGS_GAME_PARAMS_HPP_
