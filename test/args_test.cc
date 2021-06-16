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

#include "args.hpp"

#include <string>

#include "gtest/gtest.h"

TEST(ArgumentParserResolveArgsTest, InvalidArgumentArray) {
  Args args;
  ASSERT_FALSE(args.ResolveArgs(nullptr));

  char* null_ptr = nullptr;
  ASSERT_FALSE(args.ResolveArgs(&null_ptr));

  static const char* const zero_char_arr[] = {""};
  ASSERT_FALSE(args.ResolveArgs(zero_char_arr));
}

TEST(ArgumentParserResolveArgsTest, NoValidFlagsAndOptions) {
  Args args;
  static const char* arr[] = {" ", nullptr};
  ASSERT_TRUE(args.ResolveArgs(arr));
  arr[0] = "--";
  ASSERT_TRUE(args.ResolveArgs(arr));
}

TEST(ArgumentParserResolveArgsTest, MissingArgument) {
  Args args;
  ASSERT_EQ(args.GetValue("arg"), nullptr);
  static const char* const arr[] = {" ", nullptr};
  EXPECT_TRUE(args.ResolveArgs(arr));
  ASSERT_EQ(args.GetValue("arg"), nullptr);
}

namespace {

class ArgumentParserTest : public ::testing::Test {
 protected:
  static constexpr const char* const args_arr_[] = {"./args",
                                                    "--flag",
                                                    "--invalid-value",
                                                    "-",
                                                    "--string-value",
                                                    "It's value",
                                                    "--string-value",
                                                    "Overwritten value",
                                                    "--flag",
                                                    "--another-flag",
                                                    nullptr};

  void SetUp() override { ASSERT_TRUE(args_.ResolveArgs(args_arr_)); }

  Args args_;
};

}  // namespace

TEST_F(ArgumentParserTest, FlagsAbsence) {
  ASSERT_FALSE(args_.IsFlag(""));
  ASSERT_FALSE(args_.IsFlag("./args"));
  ASSERT_FALSE(args_.IsFlag("--invalid-value"));
  ASSERT_FALSE(args_.IsFlag("invalid-value"));
  ASSERT_FALSE(args_.IsFlag("-"));
  ASSERT_FALSE(args_.IsFlag("string-value"));
}

TEST_F(ArgumentParserTest, FlagsPresence) {
  ASSERT_TRUE(args_.IsFlag("flag"));
  ASSERT_TRUE(args_.IsFlag("another-flag"));
}

TEST_F(ArgumentParserTest, OptionsAbsence) {
  ASSERT_FALSE(args_.IsOption(""));
  ASSERT_FALSE(args_.IsOption("./args"));
  ASSERT_FALSE(args_.IsOption("--flag"));
  ASSERT_FALSE(args_.IsOption("flag"));
  ASSERT_FALSE(args_.IsOption("invalid-value"));
  ASSERT_FALSE(args_.IsOption("-"));
  ASSERT_FALSE(args_.IsOption("--string-value"));
  ASSERT_FALSE(args_.IsOption("-0x80000000"));
  ASSERT_FALSE(args_.IsOption("--another-flag"));
}

TEST_F(ArgumentParserTest, OptionsPresence) {
  ASSERT_TRUE(args_.IsOption("string-value"));
}

TEST_F(ArgumentParserTest, OptionsCorrectness) {
  ASSERT_STREQ(args_.GetValue("string-value"), "Overwritten value");
  ASSERT_STREQ(args_.GetValue("flag"), nullptr);
}
