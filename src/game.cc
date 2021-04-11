#include "game.hpp"

namespace {

static constexpr auto kDefaultBalance = 0;
static constexpr auto kDefaultDeckSize = 30;
static constexpr auto kDefaultPoolSize = 100;
static constexpr auto kDefaultStartingCards = 5;
static constexpr auto kDefaultTurnLimit = 10;
static constexpr auto kDefaultPointsLeading = 50;

}  // namespace

std::optional<GameParams> GameParams::Parse(const Args& args) {
  GameParams p;
  try {
    p.balance_ = ParseParam(args, "balance", 0, 5, kDefaultBalance);
    p.deck_size_ = ParseParam(args, "deck-size", 15, 50, kDefaultDeckSize);
    p.pool_size_ = ParseParam(args, "pool-size", p.deck_size_ + 1,
                              p.deck_size_ * p.deck_size_, kDefaultDeckSize);
    p.starting_cards_ = ParseParam(args, "cards", 3, 10, kDefaultStartingCards);
    p.turn_limit_ = ParseParam(args, "turn-limit", 3, 20, kDefaultTurnLimit);
    p.points_leading_ =
        ParseParam(args, "leading", 20, 100, kDefaultPointsLeading);
  } catch (const InvalidParamValue& e) {
    LOGE(e.what());
    return std::nullopt;
  }
  return p;
}
