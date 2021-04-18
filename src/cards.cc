#include "cards.hpp"

#include <algorithm>
#include <utility>

#include "game.hpp"
#include "random.hpp"

CardsQueue::~CardsQueue() {
  LOGT("Destructing queue with size {} ", Size());
  while (queue_.size()) {
    delete queue_.front();
    queue_.pop();
  }
}

Card* CardsQueue::Pull() {
  if (!queue_.size()) return nullptr;
  auto card = queue_.front();
  queue_.pop();
  return card;
}

void CardsQueue::Push(Card* card) {
  if (card != nullptr) queue_.push(card);
}

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
