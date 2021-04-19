#include "cards.hpp"

#include <gtest/gtest.h>

#include <optional>

#include "args.hpp"
#include "game.hpp"
#include "random.hpp"

inline bool operator==(const Traits::TraitsTable& lhs,
                       const Traits::TraitsTable& rhs) {
  return lhs.swift == rhs.swift && lhs.symbiotic == rhs.symbiotic &&
         lhs.poisonous == rhs.poisonous && lhs.empowering == rhs.empowering &&
         lhs.sabotaging == rhs.sabotaging && lhs.supporting == rhs.supporting;
}

TEST(TraitsTest, CorrectPurchases) {
  Traits::TraitsTable ttable;

  ttable = {1, 1, 1, 1, 1, 1};
  ASSERT_EQ(Traits(30).GetTraits(), ttable);

  ttable = {0, 0, 0, 0, 0, 0};
  ASSERT_EQ(Traits(0).GetTraits(), ttable);

  ttable = {0, 0, 0, 0, 0, 1};
  ASSERT_EQ(Traits(1).GetTraits(), ttable);

  ttable = {0, 0, 0, 0, 1, 0};
  ASSERT_EQ(Traits(2).GetTraits(), ttable);
}

TEST(CardsPoolTest, PoolInitialisation) {
  CardsPool pool;
  auto params = GameParams::Parse(Args{});
  CardGenerator gen{*params};
  pool.InitPool(*params, gen);

  auto deck = pool.GetDeck(*params);
  ASSERT_TRUE(deck);
  ASSERT_EQ(deck->Size(), params->DeckSize());
}
