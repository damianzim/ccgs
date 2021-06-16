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

#ifndef CCGS_CARDS_POOL_HPP_
#define CCGS_CARDS_POOL_HPP_

#include <memory>
#include <vector>

#include "deck.hpp"

class CardGenerator;
class GameParams;

class CardsPool {
 public:
  // Create empty cards pool. To create random decks, must be initialised with
  // the `InitPool()` method.
  CardsPool(){};

  size_t Size() const;

  // Get deck counting number of cards implicit in game params. If the pool size
  // is smaller or equal the expected deck size return nullptr. Otherwise return
  // the deck containing already generated (in `InitPool()` method) cards, but
  // with random order.
  std::unique_ptr<Deck> GetDeck(const GameParams& params);

  // Initialise pool using `gen` generator with the number of cards taken from
  // game params.
  void InitPool(const GameParams& params, CardGenerator& gen);

 private:
  std::vector<std::unique_ptr<Card>> pool_;
};

#endif  // CCGS_CARDS_POOL_HPP_
