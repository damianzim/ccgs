#include "args.hpp"

#include <gtest/gtest.h>

#include <string>

TEST(ArgumentParserResolveArgsTest, InvalidArgumentArray) {
  Args args;
  ASSERT_FALSE(args.ResolveArgs(nullptr));

  char* null_ptr = nullptr;
  ASSERT_FALSE(args.ResolveArgs(&null_ptr));

  std::string zero_char = "";
  char* zero_char_arr[] = {zero_char.data()};
  ASSERT_FALSE(args.ResolveArgs(zero_char_arr));
}

TEST(ArgumentParserResolveArgsTest, NoValidFlagsAndOptions) {
  Args args;
  std::string arg = " ";
  char* arr[] = {arg.data(), nullptr};
  ASSERT_TRUE(args.ResolveArgs(arr));
  arg = "--";
  ASSERT_TRUE(args.ResolveArgs(arr));
}

TEST(ArgumentParserResolveArgsTest, MissingArgumentException) {
  std::string arg = " ";
  static char* arr[2] = {arg.data()};
  Args args;
  ASSERT_THROW(args.ParseOption<int>("test", Args::CaptureInt),
               MissingArgumentException);
  EXPECT_TRUE(args.ResolveArgs(arr));
  ASSERT_THROW(args.ParseOption<int>("test", Args::CaptureInt),
               MissingArgumentException);
}

namespace {

class ArgumentParserTest : public ::testing::Test {
 protected:
  static constexpr const char* args_arr_[] = {"./args",
                                              "--flag",
                                              "--invalid-value",
                                              "-",
                                              "--string-value",
                                              "It's value",
                                              "--unsigned-oct-value",
                                              "0644",
                                              "--integer-negative-hex-value",
                                              "-0x80000000",
                                              "--string-value",
                                              "Overwritten value",
                                              "--flag",
                                              "--another-flag",
                                              "--integer-overflow-value",
                                              "0x10000000000000000",
                                              "--integer-invalid-value",
                                              "string"};

  Args args_;

  void SetUp() override { ASSERT_TRUE(args_.ResolveArgs((char**)args_arr_)); }
};

}  // namespace

TEST_F(ArgumentParserTest, FlagsAbsence) {
  ASSERT_FALSE(args_.IsFlag(""));
  ASSERT_FALSE(args_.IsFlag("./args"));
  ASSERT_FALSE(args_.IsFlag("--invalid-value"));
  ASSERT_FALSE(args_.IsFlag("invalid-value"));
  ASSERT_FALSE(args_.IsFlag("-"));
  ASSERT_FALSE(args_.IsFlag("string-value"));
  ASSERT_FALSE(args_.IsFlag("integer-overflow-value"));
  ASSERT_FALSE(args_.IsFlag("integer-invalid-value"));
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
  ASSERT_TRUE(args_.IsOption("unsigned-oct-value"));
  ASSERT_TRUE(args_.IsOption("integer-negative-hex-value"));
  ASSERT_TRUE(args_.IsOption("integer-overflow-value"));
  ASSERT_TRUE(args_.IsOption("integer-invalid-value"));
}

TEST_F(ArgumentParserTest, OptionsParserExceptions) {
  auto overflow =
      args_.ParseOption<int>("integer-overflow-value", Args::CaptureInt);
  ASSERT_FALSE(overflow.second);
  auto invalid =
      args_.ParseOption<int>("integer-invalid-value", Args::CaptureInt);
  ASSERT_FALSE(invalid.second);
}

TEST_F(ArgumentParserTest, ParsedOptionsCorrectness) {
  auto string =
      args_.ParseOption<std::string>("string-value", Args::CaptureString);
  ASSERT_TRUE(string.second);
  ASSERT_EQ(std::string{"Overwritten value"}, string.first);

  auto oct = args_.ParseOption<unsigned long>("unsigned-oct-value",
                                              Args::CaptureUnsignedLong);
  ASSERT_TRUE(oct.second);
  ASSERT_EQ(0644U, oct.first);

  auto hex =
      args_.ParseOption<int>("integer-negative-hex-value", Args::CaptureInt);
  ASSERT_TRUE(hex.second);
  ASSERT_EQ(-0x80000000, hex.first);
}
