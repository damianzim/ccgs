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

#include "cards_queue.hpp"

#include "common.hpp"

CardsQueue::~CardsQueue() {
  LOGT("Destructing queue with size {} ", Size());
  while (queue_.size()) {
    delete queue_.front();
    queue_.pop();
  }
}

size_t CardsQueue::Size() const { return queue_.size(); }

Card* CardsQueue::Pull() {
  if (!queue_.size()) return nullptr;
  auto card = queue_.front();
  queue_.pop();
  return card;
}

void CardsQueue::Push(Card* card) {
  if (card != nullptr) queue_.push(card);
}
