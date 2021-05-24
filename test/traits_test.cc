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
