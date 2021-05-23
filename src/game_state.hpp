#ifndef CCGS_GAME_STATE_HPP_
#define CCGS_GAME_STATE_HPP_

#include <string_view>

#include "game_result.hpp"

class GameState : public GameResult {
 public:
  bool CanRun() const;

  void Set(StatusType state);
  void Set(const std::string_view& p1_name, float p1_strength,
           const std::string_view& p2_name, float p2_strength);
};

#endif  // CCGS_GAME_STATE_HPP_
