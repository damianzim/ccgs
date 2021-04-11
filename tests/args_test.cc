#include "args.hpp"

#include <gtest/gtest.h>

#include <string>

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
