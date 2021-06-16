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

#include "cards_pool.hpp"

#include <optional>

#include "gtest/gtest.h"
#include "args.hpp"
#include "game_params.hpp"
#include "random.hpp"

TEST(CardsPoolTest, PoolInitialisation) {
  CardsPool pool;
  auto params = GameParams::Parse(Args{});
  CardGenerator gen{*params};
  pool.InitPool(*params, gen);

  auto deck = pool.GetDeck(*params);
  ASSERT_TRUE(deck);
  ASSERT_EQ(deck->Size(), params->DeckSize());
}
