#include "game.hpp"

#include <gtest/gtest.h>

#include <optional>

#include "args.hpp"
#include "common.hpp"

struct GameParamsMembers {
  int balance;
  int deck_size;
  int pool_size;
  int starting_cards;
  int turn_limit;
  int points_leading;
};

inline bool operator==(const GameParams& lhs, const GameParamsMembers& rhs) {
  return lhs.Balance() == rhs.balance && lhs.DeckSize() == rhs.deck_size &&
         lhs.PoolSize() == rhs.pool_size &&
         lhs.PointsLeading() == rhs.points_leading &&
         lhs.StartingCards() == rhs.starting_cards &&
         lhs.TurnLimit() == rhs.turn_limit;
}

namespace {

class GameParamsParserTest : public ::testing::Test {
 protected:
  Args args_{};

  // Set params to default values thanks to the empty args.
  GameParamsParserTest() : params_(*GameParams::Parse(args_)){};

  GameParamsMembers DefaultParams() const {
    return {params_.Balance(),   params_.DeckSize(),
            params_.PoolSize(),  params_.StartingCards(),
            params_.TurnLimit(), params_.PointsLeading()};
  }

  std::optional<GameParams> ParseParams(const char* const* argv) {
    SetArgs(argv);
    return GameParams::Parse(args_);
  }

  void SetArgs(const char* const* argv) {
    ASSERT_TRUE(args_.ResolveArgs(argv));
  }

 private:
  const GameParams params_;
};

}  // namespace

TEST_F(GameParamsParserTest, TestBalance) {
  GameParamsMembers params = DefaultParams();

  const char* argv[] = {"--balance", "-1", nullptr};
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "6";
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "1";
  params.balance = 1;
  ASSERT_EQ(ParseParams(argv).value(), params);

  argv[1] = "5";
  params.balance = 5;
  ASSERT_EQ(ParseParams(argv).value(), params);
}

TEST_F(GameParamsParserTest, TestDeckSize) {
  GameParamsMembers params = DefaultParams();

  const char* argv[] = {"--deck-size", "14", nullptr};
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "51";
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "15";
  params.deck_size = 15;
  ASSERT_EQ(ParseParams(argv).value(), params);

  argv[1] = "50";
  params.deck_size = 50;
  ASSERT_EQ(ParseParams(argv).value(), params);
}

TEST_F(GameParamsParserTest, TestPoolSize) {
  GameParamsMembers params = DefaultParams();

  const char* argv[] = {"--pool-size", "35", "--deck-size", "35", nullptr};
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "1226";
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "36";
  params.deck_size = 35;
  params.pool_size = 36;
  ASSERT_EQ(ParseParams(argv).value(), params);

  argv[1] = "1225";
  params.deck_size = 35;
  params.pool_size = 1225;
  ASSERT_EQ(ParseParams(argv).value(), params);
}

TEST_F(GameParamsParserTest, TestTurnLimit) {
  GameParamsMembers params = DefaultParams();

  const char* argv[] = {"--turn-limit", "2", nullptr};
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "21";
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "3";
  params.turn_limit = 3;
  ASSERT_EQ(ParseParams(argv).value(), params);

  argv[1] = "20";
  params.turn_limit = 20;
  ASSERT_EQ(ParseParams(argv).value(), params);
}

TEST_F(GameParamsParserTest, TestPointsLeading) {
  GameParamsMembers params = DefaultParams();

  const char* argv[] = {"--leading", "19", nullptr};
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "101";
  ASSERT_EQ(ParseParams(argv), std::nullopt);

  argv[1] = "20";
  params.points_leading = 20;
  ASSERT_EQ(ParseParams(argv).value(), params);

  argv[1] = "100";
  params.points_leading = 100;
  ASSERT_EQ(ParseParams(argv).value(), params);
}
