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
