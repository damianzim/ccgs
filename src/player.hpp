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

#ifndef CCGS_PLAYER_HPP_
#define CCGS_PLAYER_HPP_

#include <memory>
#include <string>

#include "deck.hpp"
class Player {
 public:
  // Create player with deck and name.
  Player(std::unique_ptr<Deck> deck, std::string name = "UNNAMED")
      : name_(name), deck_(std::move(deck)) {
    LOGD("Creating player {} with deck size {}", name, deck_->Size());
  }

  // Check if the player has cards.
  bool HasCards() const { return deck_->Size(); }

  // Get the player's name.
  const char* Name() const { return name_.c_str(); }

  // Get a card from the player's deck.
  Card* PullCard() const { return deck_->Pull(); }

 private:
  const std::string name_;

  std::unique_ptr<Deck> deck_;
};

#endif  // CCGS_PLAYER_HPP_
