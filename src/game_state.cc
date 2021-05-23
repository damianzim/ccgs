#include "game_state.hpp"

bool GameState::CanRun() const {
  return status_ == kInProgress || status_ == kInitialised;
}

void GameState::Set(StatusType state) { status_ = state; }
void GameState::Set(const std::string_view& p1_name, const float p1_strength,
                    const std::string_view& p2_name, const float p2_strength) {
  final_[0].name = p1_name;
  final_[0].strength = p1_strength;
  final_[1].name = p2_name;
  final_[1].strength = p2_strength;
}
