#ifndef CCGS_COMMON_HPP_
#define CCGS_COMMON_HPP_

#include <assert.h>
#include <stddef.h>

#define ARR_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

static constexpr auto kDefaultBalance = 0;
static constexpr auto kDefaultDeckSize = 30;
static constexpr auto kDefaultPoolSize = 100;
static constexpr auto kDefaultStartingCards = 5;
static constexpr auto kDefaultTurnLimit = 10;
static constexpr auto kDefaultPointsLeading = 50;

struct GameParams {
  size_t balance;
  size_t deck_size;
  size_t pool_size;
  size_t starting_cards;
  size_t turn_limit;
  size_t points_leading;
};

#endif  // CCGS_COMMON_HPP_
