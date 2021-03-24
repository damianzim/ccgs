#include "cards.hpp"

#include <algorithm>
#include <utility>

#include "random.hpp"

Card::Traits Card::Traits::BuyTraits(int power_level) {
  Traits traits{};
  if (power_level >= 12) {
    power_level -= 12;
    traits.swift = true;
  }
  if (power_level >= 8) {
    power_level -= 8;
    traits.symbiotic = true;
  }
  if (power_level >= 4) {
    power_level -= 4;
    traits.poisonous = true;
  }
  if (power_level >= 3) {
    power_level -= 3;
    traits.empowering = true;
  }
  if (power_level >= 2) {
    power_level -= 2;
    traits.sabotaging = true;
  }
  if (power_level >= 1) {
    traits.supporting = true;
  }
  return traits;
}

void CardsPool::InitPool(const size_t pool_size, CardGenerator& gen) {
  pool_.clear();
  pool_.reserve(pool_size);
  for (size_t i = 0; i < pool_.capacity(); ++i) pool_.push_back(gen.GetCard());
}

std::optional<Deck> CardsPool::GetDeck(const size_t deck_size) {
  if (pool_.size() <= deck_size) return std::nullopt;
  DeviceGenerator dgen;
  std::shuffle(pool_.begin(), pool_.end(), dgen.Engine());

  std::vector<Card> cards;
  cards.reserve(deck_size);
  for (size_t i = 0; i < cards.capacity(); ++i) cards.push_back(*pool_[i]);
  return Deck(std::move(cards));
}
