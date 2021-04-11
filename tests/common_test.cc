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

TEST(ParserTest, ParseULongInvalidInput) {
  ASSERT_FALSE(Parser::ParseULong("string").has_value());
  ASSERT_FALSE(Parser::ParseULong("0x10000000000000000").has_value());
}

TEST(ParserTest, ParseULongValidInput) {
  auto hex = Parser::ParseULong("0x80000000");
  ASSERT_TRUE(hex.has_value());
  ASSERT_EQ(*hex, 0x80000000);

  auto oct = Parser::ParseULong("0644");
  ASSERT_TRUE(oct.has_value());
  ASSERT_EQ(*oct, 0644);
}
