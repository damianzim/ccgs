#ifndef CCGS_CARDS_HPP_
#define CCGS_CARDS_HPP_

#include <memory>
#include <optional>
#include <vector>

#include "common.hpp"

class Card {
 public:
  using AttrValue = uint8_t;
  using Strength = float;

  struct Attributes {
    AttrValue water;
    AttrValue fire;
    AttrValue nature;
  };

  struct Traits {
    bool swift;
    bool symbiotic;
    bool poisonous;
    bool empowering;
    bool sabotaging;
    bool supporting;

    static Traits BuyTraits(int power_level);
  };

  Card(Strength strength, Attributes attrs, Traits traits)
      : attrs_(attrs), strength_(strength), traits_(traits){};

  Card(const Card& other) = default;
  Card(Card&& other) = default;
  Card& operator=(const Card&) = delete;
  Card& operator=(Card&&) = delete;

 protected:
  const Attributes attrs_;
  const Strength strength_;
  const Traits traits_;
};

class Deck {
 public:
  Deck(std::vector<Card>&& cards) : cards_(cards){};

  size_t Size() const { return cards_.size(); }

 private:
  std::vector<Card> cards_;
};

class CardGenerator;

class CardsPool {
 public:
  CardsPool(){};

  size_t Size() const { return pool_.size(); }

  std::optional<Deck> GetDeck(size_t deck_size);
  void InitPool(size_t pool_size, CardGenerator& gen);

 private:
  std::vector<std::unique_ptr<Card>> pool_;
};

#endif  // CCGS_CARDS_HPP_
