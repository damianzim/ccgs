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

  Card(Attributes attrs, Strength strength, Traits&& traits);
  Card(const Card&) = default;
  Card(Card&&) = default;
  Card& operator=(const Card&) = delete;
  Card& operator=(Card&&) = delete;

  Strength GetStrength() const;
  Attributes GetAttrs() const;

  void ApplyAttrs(const Card& previous);
  void SetAttrs(const Attributes& attrs);
  void SetStrength(const Strength& strength);

 private:
  Attributes attrs_;
  Strength strength_;
};

#endif  // CCGS_CARD_HPP_
