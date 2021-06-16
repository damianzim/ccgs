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

#include "common.hpp"

#include "gtest/gtest.h"

TEST(ParserTest, ParseIntInvalidInput) {
  ASSERT_FALSE(Parser::ParseInt("0x10000000000000000").has_value());
  ASSERT_FALSE(Parser::ParseInt("string").has_value());
}

TEST(ParserTest, ParseIntValidInput) {
  auto negative_hex = Parser::ParseInt("-0x80000000");
  ASSERT_TRUE(negative_hex.has_value());
  ASSERT_EQ(*negative_hex, 0x80000000);
}
