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
