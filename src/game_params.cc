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

#include "game_params.hpp"

#include <math.h>

#include "common.hpp"

namespace {

constexpr auto kDefaultBalance = 0;
constexpr auto kDefaultDeckSize = 30;
constexpr auto kDefaultPoolSize = 100;
constexpr auto kDefaultStartingCards = 5;
constexpr auto kDefaultTurnLimit = 10;
constexpr auto kDefaultPointsLeading = 50;

}  // namespace

class GameParams::InvalidParamValue : public std::runtime_error {
  std::string msg_;

 public:
  explicit InvalidParamValue(const std::string& param_name)
      : std::runtime_error("") {
    msg_ = "Invalid value for --" + param_name + ", see usage.";
  }
  const char* what() const noexcept override { return msg_.c_str(); }
};

std::optional<GameParams> GameParams::Parse(const Args& args) {
  GameParams p;
  try {
    p.balance_ = ParseParam(args, "balance", 0, 5, kDefaultBalance);
    p.deck_size_ = ParseParam(args, "deck-size", 15, 50, kDefaultDeckSize);
    p.pool_size_ = ParseParam(args, "pool-size", p.deck_size_ + 1,
                              p.deck_size_ * p.deck_size_, kDefaultPoolSize);
    p.starting_cards_ = ParseParam(args, "cards", 3, 10, kDefaultStartingCards);
    p.turn_limit_ = ParseParam(args, "turn-limit", 3, 20, kDefaultTurnLimit);
    p.points_leading_ =
        ParseParam(args, "leading", 0, 100, kDefaultPointsLeading);
    if (p.points_leading_ > 0 && p.points_leading_ < 20)
      throw InvalidParamValue{"leading"};
  } catch (const InvalidParamValue& e) {
    LOGE(e.what());
    return std::nullopt;
  }
  return p;
}

int GameParams::ParseParam(const Args& args, const std::string& name,
                           const int min, const int max,
                           const int& default_value) {
  auto arg_value = args.GetValue(name);
  if (arg_value == nullptr) return default_value;
  auto value = Parser::ParseInt(arg_value);
  if (!value || *value < min || *value > max) throw InvalidParamValue(name);
  return *value;
}
