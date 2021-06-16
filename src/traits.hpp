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

#ifndef CCGS_TRAITS_HPP_
#define CCGS_TRAITS_HPP_

#include <type_traits>
#include <vector>

#include "trait.hpp"

class Table;

class Traits {
 public:
  struct TraitsTable {
    bool swift;
    bool symbiotic;
    bool poisonous;
    bool empowering;
    bool sabotaging;
    bool supporting;
  };

  // Construct Traits object with the traits that succeed to purchase.
  Traits(int power_level);

  // Move the traits of the `other` object.
  Traits(const Traits& other);

  // Move the traits of the `other` object.
  Traits(Traits&& other);
  Traits& operator=(const Traits&) = delete;
  Traits& operator=(Traits&&) = delete;

  // Desturct the object and its owned traits.
  ~Traits();

  // Return the table which represents traits acquired during the object
  // constructing.
  // Note: The table may indicated that there was no poisonous trait while it
  // was. This is because this value may be changed to false in the meantime.
  TraitsTable GetTraits() const;

  // Move all traits from this object to the table.
  void ApplyTraits(Table& table);

  // This(!) poisonous no longer impact the game. (It can be used only once to
  // destruct opponent's card.)
  void MarkPoisonousUneffective();

 private:
  TraitsTable table_{};
  std::vector<Trait*> traits_;

  // Purchase a trait (pushing it to object's container) if the power
  // level >= cost, and change `trait_in_table` value to true.
  template <class T>
  void BuyTrait(int& power_level, int cost, bool& trait_in_table) {
    static_assert(std::is_base_of<Trait, T>::value);
    if (power_level < cost) return;
    power_level -= cost;
    traits_.emplace_back(new T);
    trait_in_table = true;
  }
};

#endif  // CCGS_TRAITS_HPP_
