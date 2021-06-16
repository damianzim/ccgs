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

#ifndef CCGS_CARDS_QUEUE_HPP_
#define CCGS_CARDS_QUEUE_HPP_

#include <queue>

#include "card.hpp"

class CardsQueue {
 public:
  // Construct an empty FIFO container for cards.
  CardsQueue() = default;

  // Destruct the object and the cards if it contains some.
  ~CardsQueue();

  size_t Size() const;

  // Pull a card from the beginning of the queue.
  Card* Pull();

  // Push card at the end of the queue.
  void Push(Card* card);

 private:
  std::queue<Card*> queue_;
};

#endif  // CCGS_CARDS_QUEUE_HPP_
