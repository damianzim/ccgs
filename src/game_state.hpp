#ifndef CCGS_GAME_STATE_HPP_
#define CCGS_GAME_STATE_HPP_

#include <string_view>

#include "game_result.hpp"

class GameState : public GameResult {
 public:
  // Check whenever the game's current state allows for the further playing.
  bool CanRun() const;

  // Set the current game state.
  void Set(StatusType state);

  // Set the final game result. `p1` is the winner or in case of a draw, the
  // order does not matter.
  void Set(const std::string_view& p1_name, float p1_strength,
           const std::string_view& p2_name, float p2_strength);
};

#endif  // CCGS_GAME_STATE_HPP_
