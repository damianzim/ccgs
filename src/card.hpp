#ifndef CCGS_CARD_HPP_
#define CCGS_CARD_HPP_

#include <vector>

#include "common.hpp"
#include "traits.hpp"

class Card : public Traits {
 public:
  using AttrValue = uint8_t;
  using Strength = float;

  struct Attributes {
    AttrValue water;
    AttrValue fire;
    AttrValue nature;
  };

  // Construct Card object with given attributes, strength and traits.
  Card(Attributes attrs, Strength strength, Traits&& traits);
  Card(const Card&) = default;
  Card(Card&&) = default;
  Card& operator=(const Card&) = delete;
  Card& operator=(Card&&) = delete;

  Strength GetStrength() const;
  Attributes GetAttrs() const;

  // Apply the formula to reduce strength of this card depending on `previous`
  // card (which should be the opponent's card last played card).
  void ApplyAttrs(const Card& previous);

  void SetAttrs(const Attributes& attrs);
  void SetStrength(const Strength& strength);

 private:
  Attributes attrs_;
  Strength strength_;
};

#endif  // CCGS_CARD_HPP_
