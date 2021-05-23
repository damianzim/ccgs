#ifndef CCGS_CARDS_POOL_HPP_
#define CCGS_CARDS_POOL_HPP_

#include <memory>
#include <vector>

#include "deck.hpp"

class CardGenerator;
class GameParams;

class CardsPool {
 public:
  CardsPool(){};

  size_t Size() const;

  std::unique_ptr<Deck> GetDeck(const GameParams& params);
  void InitPool(const GameParams& params, CardGenerator& gen);

 private:
  std::vector<std::unique_ptr<Card>> pool_;
};

#endif  // CCGS_CARDS_POOL_HPP_
