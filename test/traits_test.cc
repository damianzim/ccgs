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

#include "traits.hpp"

#include "gtest/gtest.h"

inline bool operator==(const Traits::TraitsTable& lhs,
                       const Traits::TraitsTable& rhs) {
  return lhs.swift == rhs.swift && lhs.symbiotic == rhs.symbiotic &&
         lhs.poisonous == rhs.poisonous && lhs.empowering == rhs.empowering &&
         lhs.sabotaging == rhs.sabotaging && lhs.supporting == rhs.supporting;
}

TEST(TraitsTest, CorrectPurchases) {
  Traits::TraitsTable ttable;

  ttable = {1, 1, 1, 1, 1, 1};
  ASSERT_EQ(Traits(42).GetTraits(), ttable);

  ttable = {0, 0, 0, 0, 0, 0};
  ASSERT_EQ(Traits(0).GetTraits(), ttable);

  ttable = {0, 0, 0, 0, 0, 1};
  ASSERT_EQ(Traits(2).GetTraits(), ttable);

  ttable = {0, 0, 0, 0, 1, 0};
  ASSERT_EQ(Traits(3).GetTraits(), ttable);
}
