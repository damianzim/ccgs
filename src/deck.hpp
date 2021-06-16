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

#ifndef CCGS_DECK_HPP_
#define CCGS_DECK_HPP_

#include <utility>
#include <vector>

#include "card.hpp"

class Deck {
 public:
  // Deck cards container is characterised by the fact that it can only be
  // filled while constructing the object.
  Deck(std::vector<Card*> cards);
  Deck(const Deck&) = delete;
  Deck& operator=(const Deck&) = delete;

  // Destruct the object and the cards if it contains them.
  ~Deck();

  // Get total strength of the cards present in the deck.
  Card::Strength GetStrength() const;
  size_t Size() const;

  // Pull random card from the deck.
  Card* PullRandom();

  // Pull a card from the end of the deck.
  Card* Pull();

 protected:
  // Construct empty deck (only for inheritance!).
  Deck() = default;

  std::vector<Card*> cards_;
};

class MutableDeck : public Deck {
 public:
  // Construct an empty MutableDeck.
  MutableDeck() = default;

  // Run the `condition` function for every card in the deck. Return a vector of
  // all cards satisfying the condtion, in other words, for cards for which the
  // function returned true.
  std::vector<Card*> Filter(Func<bool(Card*)> condition) const;


  // Push the `card` at the end of the deck.
  void Push(Card* card);
};

#endif  // CCGS_DECK_HPP_
