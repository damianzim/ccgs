#include "cards.hpp"

#include <gtest/gtest.h>

#include <optional>

#include "random.hpp"

inline bool operator==(const Card::Traits& lhs, const Card::Traits& rhs) {
  return lhs.swift == rhs.swift && lhs.symbiotic == rhs.symbiotic &&
         lhs.poisonous == rhs.poisonous && lhs.empowering == rhs.empowering &&
         lhs.sabotaging == rhs.sabotaging && lhs.supporting == rhs.supporting;
}

TEST(TraitsTest, CorrectPurchases) {
  using Traits = Card::Traits;
  Traits traits;

  traits = {1, 1, 1, 1, 1, 1};
  ASSERT_EQ(Traits::BuyTraits(30), traits);

  traits = {0, 0, 0, 0, 0, 0};
  ASSERT_EQ(Traits::BuyTraits(0), traits);

  traits.supporting = 1;
  ASSERT_EQ(Traits::BuyTraits(1), traits);

  traits.supporting = 0;
  traits.sabotaging = 1;
  ASSERT_EQ(Traits::BuyTraits(2), traits);
}

TEST(CardsPoolTest, PoolInitialisation) {
  CardsPool pool;
  ASSERT_EQ(pool.GetDeck(0), std::nullopt);
  ASSERT_EQ(pool.GetDeck(1), std::nullopt);

  CardGenerator gen{2};
  pool.InitPool(100, gen);

  ASSERT_EQ(pool.GetDeck(100), std::nullopt);
  ASSERT_EQ(pool.GetDeck(101), std::nullopt);

  auto deck = pool.GetDeck(99);
  ASSERT_TRUE(deck);
  ASSERT_EQ(deck->Size(), 99);
}
