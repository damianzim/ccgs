#ifndef CCGS_DECK_HPP_
#define CCGS_DECK_HPP_

#include <utility>
#include <vector>

#include "card.hpp"

class Deck {
 public:
  Deck(std::vector<Card*> cards);
  Deck(const Deck&) = delete;
  Deck& operator=(const Deck&) = delete;
  ~Deck();

  Card::Strength GetStrength() const;
  size_t Size() const;

  Card* PullRandom();
  Card* Pull();

 protected:
  Deck() = default;

  std::vector<Card*> cards_;
};

class MutableDeck : public Deck {
 public:
  MutableDeck() = default;

  std::vector<Card*> Filter(Func<bool(Card*)> condition) const;

  void Push(Card* card);
};

#endif  // CCGS_DECK_HPP_
