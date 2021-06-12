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
