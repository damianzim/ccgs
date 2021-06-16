//
// This file is part of the CCGS (https://github.com/damianzim/ccgs)
// distribution.
// Copyright (c) 2021 Piotr Jasik & Damian Zimon.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

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
