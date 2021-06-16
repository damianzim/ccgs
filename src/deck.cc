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

#include "deck.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>

#include "common.hpp"
#include "random.hpp"

Deck::Deck(std::vector<Card*> cards) : cards_(std::move(cards)){};

Deck::~Deck() {
  LOGT("Destructing container with size {} ", Size());
  for (auto card : cards_) delete card;
}

Card::Strength Deck::GetStrength() const {
  return std::accumulate(cards_.begin(), cards_.end(), 0.F,
                         [](Card::Strength sum, const Card* card) {
                           return sum + card->GetStrength();
                         });
}

size_t Deck::Size() const { return cards_.size(); }

Card* Deck::PullRandom() {
  if (!Size()) return nullptr;
  size_t i = DeviceGenerator().Random() % Size();
  auto card = cards_[i];
  cards_.erase(cards_.begin() + i);
  return card;
}

Card* Deck::Pull() {
  if (!Size()) return nullptr;
  auto card = cards_.back();
  cards_.pop_back();
  return card;
}

std::vector<Card*> MutableDeck::Filter(Func<bool(Card*)> condition) const {
  ScopeTrace scope{"Filter"};
  std::vector<Card*> satysfying;
  std::copy_if(cards_.begin(), cards_.end(), std::back_inserter(satysfying),
               condition);
  return satysfying;
}

void MutableDeck::Push(Card* card) { cards_.push_back(card); }
