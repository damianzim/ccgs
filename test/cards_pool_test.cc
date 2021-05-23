#include "cards_pool.hpp"

#include <gtest/gtest.h>

#include <optional>

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
