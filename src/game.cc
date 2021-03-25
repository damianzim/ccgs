#include "game.hpp"

#include <math.h>

std::optional<GameParams> ParseGameParams(const Args& args) {
  GameParams params{kDefaultBalance,   kDefaultDeckSize,
                    kDefaultPoolSize,  kDefaultStartingCards,
                    kDefaultTurnLimit, kDefaultPointsLeading};

  try {
    auto [balance, result] = args.ParseOption<int>("balance", Args::CaptureInt);
    if (!result || balance < 0 || balance > 5) {
      std::cerr << "Invalid value for --balance, see usage." << std::endl;
      return std::nullopt;
    }
    params.balance = balance;
  } catch (MissingArgumentException&) {
  }

  try {
    auto [deck_size, result] =
        args.ParseOption<int>("deck-size", Args::CaptureInt);
    if (!result || deck_size < 15 || deck_size > 50) {
      std::cerr << "Invalid value for --deck-size, see usage." << std::endl;
      return std::nullopt;
    }
    params.deck_size = deck_size;
  } catch (MissingArgumentException&) {
  }

  try {
    auto [pool_size, result] =
        args.ParseOption<unsigned long>("pool-size", Args::CaptureUnsignedLong);
    auto min_pool_size = params.deck_size + 1;
    auto max_pool_size = params.deck_size * params.deck_size;
    if (!result || pool_size < min_pool_size || pool_size > max_pool_size) {
      std::cerr << "Invalid value for --pool-size, for deck size = "
                << params.deck_size << " the value must be in the interval ["
                << min_pool_size << ';' << max_pool_size << "], see usage."
                << std::endl;
      return std::nullopt;
    }
    params.pool_size = pool_size;
  } catch (MissingArgumentException&) {
  }

  try {
    auto [cards, result] = args.ParseOption<int>("cards", Args::CaptureInt);
    if (!result || cards < 3 || cards > 10) {
      std::cerr << "Invalid value for --cards, see usage." << std::endl;
      return std::nullopt;
    }
    params.starting_cards = cards;
  } catch (MissingArgumentException&) {
  }

  try {
    auto [turn_limit, result] =
        args.ParseOption<int>("turn-limit", Args::CaptureInt);
    if (!result || turn_limit < 3 || turn_limit > 20) {
      std::cerr << "Invalid value for --turn-limit, see usage." << std::endl;
      return std::nullopt;
    }
    params.turn_limit = turn_limit;
  } catch (MissingArgumentException&) {
  }

  try {
    auto [points_leading, result] =
        args.ParseOption<int>("leading", Args::CaptureInt);
    if (!result || points_leading < 20 || points_leading > 100) {
      std::cerr << "Invalid value for --leading, see usage." << std::endl;
      return std::nullopt;
    }
    params.points_leading = points_leading;
  } catch (MissingArgumentException&) {
  }

  return params;
}
