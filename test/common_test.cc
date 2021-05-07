#include "common.hpp"

#include <gtest/gtest.h>

TEST(ParserTest, ParseIntInvalidInput) {
  ASSERT_FALSE(Parser::ParseInt("0x10000000000000000").has_value());
  ASSERT_FALSE(Parser::ParseInt("string").has_value());
}

TEST(ParserTest, ParseIntValidInput) {
  auto negative_hex = Parser::ParseInt("-0x80000000");
  ASSERT_TRUE(negative_hex.has_value());
  ASSERT_EQ(*negative_hex, 0x80000000);
}
