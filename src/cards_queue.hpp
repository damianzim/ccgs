#ifndef CCGS_CARDS_QUEUE_HPP_
#define CCGS_CARDS_QUEUE_HPP_

#include <queue>

#include "card.hpp"

class CardsQueue {
 public:
  CardsQueue() = default;
  ~CardsQueue();

  size_t Size() const;

  Card* Pull();
  void Push(Card* card);

 private:
  std::queue<Card*> queue_;
};

#endif  // CCGS_CARDS_QUEUE_HPP_
