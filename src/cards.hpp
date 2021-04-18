#ifndef CCGS_CARDS_HPP_
#define CCGS_CARDS_HPP_

#include <memory>
#include <queue>
#include <vector>

#include "card.hpp"
#include "common.hpp"

class CardsQueue {
 public:
  CardsQueue() = default;
  ~CardsQueue();

  size_t Size() const { return queue_.size(); }

  Card* Pull();
  void Push(Card* card);

 private:
  std::queue<Card*> queue_;
};

class Deck {
 public:
  Deck(std::vector<Card*> cards) : cards_(std::move(cards)){};
  Deck(const Deck&) = delete;
  Deck& operator=(const Deck&) = delete;
  ~Deck();

  Card* NextCard() const;
  size_t Size() const { return cards_.size(); }

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

class CardGenerator;
class GameParams;

class CardsPool {
 public:
  CardsPool(){};

  size_t Size() const { return pool_.size(); }

  std::unique_ptr<Deck> GetDeck(const GameParams& params);
  void InitPool(const GameParams& params, CardGenerator& gen);

 private:
  std::vector<std::unique_ptr<Card>> pool_;
};

#endif  // CCGS_CARDS_HPP_
