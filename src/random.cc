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

#include "random.hpp"

#include "card.hpp"
#include "game_params.hpp"

unsigned long DeviceGenerator::Random() { return gen_(); }

long long DeviceGenerator::RandomBetween(unsigned long lr, unsigned long rr) {
  if (lr > rr)
    return -1;
  else if (lr == rr)
    return lr;
  rr -= lr;
  return (Random() % ++rr) + lr;
}

unsigned long DeviceGenerator::RandomMax(unsigned long max) {
  if (!max) return 0;
  return Random() % (max + 1);
}

CardGenerator::CardGenerator(const GameParams& params)
    : balance_(params.Balance() * 10),
      max_(kPowerLevel + balance_),
      min_(kPowerLevel - balance_){};

std::unique_ptr<Card> CardGenerator::GetCard() {
  int power_level = !balance_ ? kPowerLevel : gen_.RandomBetween(min_, max_);

  int strength = gen_.RandomBetween(10, 100);
  power_level -= strength;

  Card::Attributes attrs{};
  for (auto attr : {&attrs.water, &attrs.fire, &attrs.nature}) {
    int attr_val = gen_.RandomMax(std::min<int>(5, power_level / 10));
    power_level -= attr_val * 10;
    *attr = attr_val;
  }

  power_level /= 10;  // Return to the natural scale of Power Level. Forget the
                      // fractional part because it will not be needed anymore.
  return std::make_unique<Card>(attrs, strength / 10.F, Traits(power_level));
}
