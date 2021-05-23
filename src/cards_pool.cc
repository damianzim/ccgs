#include "cards_pool.hpp"

#include <algorithm>

#include "game_params.hpp"
#include "random.hpp"

size_t CardsPool::Size() const { return pool_.size(); }

std::unique_ptr<Deck> CardsPool::GetDeck(const GameParams& params) {
  size_t deck_size = params.DeckSize();
  if (pool_.size() <= deck_size) return nullptr;
  DeviceGenerator dgen;
  std::shuffle(pool_.begin(), pool_.end(), dgen.Engine());

  std::vector<Card*> cards;
  cards.reserve(deck_size);
  for (size_t i = 0; i < deck_size; ++i) cards.push_back(new Card(*pool_[i]));
  return std::make_unique<Deck>(cards);
}

void CardsPool::InitPool(const GameParams& params, CardGenerator& gen) {
  pool_.clear();
  pool_.reserve(params.PoolSize());
  for (size_t i = 0; i < pool_.capacity(); ++i) pool_.push_back(gen.GetCard());
}
