#include "game.hpp"

#include <gtest/gtest.h>

#include <optional>

#include "args.hpp"
#include "common.hpp"

inline bool operator==(const GameParams& lhs, const GameParams& rhs) {
  return lhs.balance == rhs.balance && lhs.deck_size == rhs.deck_size &&
         lhs.points_leading == rhs.points_leading &&
         lhs.pool_size == rhs.pool_size &&
         lhs.starting_cards == rhs.starting_cards &&
         lhs.turn_limit == rhs.turn_limit;
}

namespace {

class GameParamsParserTest : public ::testing::Test {
 protected:
  Args args_;
  GameParams params_;

  std::optional<GameParams> ParseParams(const char** argv) {
    SetArgs(argv);
    return ParseGameParams(args_);
  }

  void SetArgs(const char** argv) {
    ASSERT_TRUE(args_.ResolveArgs((char**)argv));
  }

  void SetUp() override {
    params_ = {kDefaultBalance,       kDefaultDeckSize,  kDefaultPoolSize,
               kDefaultStartingCards, kDefaultTurnLimit, kDefaultPointsLeading};
  }
};

}  // namespace

TEST_F(GameParamsParserTest, DefaultParams) {
  auto params = ParseGameParams(args_);
  ASSERT_TRUE(params);
  ASSERT_EQ(params_, params);
}

TEST_F(GameParamsParserTest, TestBalance) {
  const char* argv[] = {"--balance", "-1", nullptr};
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "6";
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "1";
  params_.balance = 1;
  ASSERT_EQ(ParseParams(argv).value(), params_);

  argv[1] = "5";
  params_.balance = 5;
  ASSERT_EQ(ParseParams(argv).value(), params_);
}

TEST_F(GameParamsParserTest, TestDeckSize) {
  const char* argv[] = {"--deck-size", "14", nullptr};
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "51";
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "15";
  params_.deck_size = 15;
  ASSERT_EQ(ParseParams(argv).value(), params_);

  argv[1] = "50";
  params_.deck_size = 50;
  ASSERT_EQ(ParseParams(argv).value(), params_);
}

TEST_F(GameParamsParserTest, TestPoolSize) {
  const char* argv[] = {"--pool-size", "35", "--deck-size", "35", nullptr};
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "1226";
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "36";
  params_.deck_size = 35;
  params_.pool_size = 36;
  ASSERT_EQ(ParseParams(argv).value(), params_);

  argv[1] = "1225";
  params_.deck_size = 35;
  params_.pool_size = 1225;
  ASSERT_EQ(ParseParams(argv).value(), params_);
}

TEST_F(GameParamsParserTest, TestTurnLimit) {
  const char* argv[] = {"--turn-limit", "2", nullptr};
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "21";
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "3";
  params_.turn_limit = 3;
  ASSERT_EQ(ParseParams(argv).value(), params_);

  argv[1] = "20";
  params_.turn_limit = 20;
  ASSERT_EQ(ParseParams(argv).value(), params_);
}

TEST_F(GameParamsParserTest, TestPointsLeading) {
  const char* argv[] = {"--leading", "19", nullptr};
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "101";
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "20";
  params_.points_leading = 20;
  ASSERT_EQ(ParseParams(argv).value(), params_);

  argv[1] = "100";
  params_.points_leading = 100;
  ASSERT_EQ(ParseParams(argv).value(), params_);
}
