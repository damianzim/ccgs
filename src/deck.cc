#include "deck.hpp"

#include <algorithm>
#include <iterator>
#include <numeric>

#include "common.hpp"
#include "random.hpp"

Deck::Deck(std::vector<Card*> cards) : cards_(std::move(cards)){};

Deck::~Deck() {
  LOGT("Destructing container with size {} ", Size());
  for (auto card : cards_) delete card;
}

Card::Strength Deck::GetStrength() const {
  return std::accumulate(cards_.begin(), cards_.end(), 0.F,
                         [](Card::Strength sum, const Card* card) {
                           return sum + card->GetStrength();
                         });
}

size_t Deck::Size() const { return cards_.size(); }

Card* Deck::PullRandom() {
  if (!Size()) return nullptr;
  size_t i = DeviceGenerator().Random() % Size();
  auto card = cards_[i];
  cards_.erase(cards_.begin() + i);
  return card;
}

Card* Deck::Pull() {
  if (!Size()) return nullptr;
  auto card = cards_.back();
  cards_.pop_back();
  return card;
}

std::vector<Card*> MutableDeck::Filter(Func<bool(Card*)> condition) const {
  ScopeTrace scope{"Filter"};
  std::vector<Card*> satysfying;
  std::copy_if(cards_.begin(), cards_.end(), std::back_inserter(satysfying),
               condition);
  return satysfying;
}

void MutableDeck::Push(Card* card) { cards_.push_back(card); }
