#ifndef CCGS_GAME_HPP_
#define CCGS_GAME_HPP_

#include <memory>

#include "export.hpp"
#include "game_state.hpp"
#include "table.hpp"

class GameParams;

class Game {
 public:
  Game(const GameParams& params);

  bool InitGame(const char* output_dir);
  GameResult Run();

 private:
  const GameParams& params_;

  void SetState(GameResult::StatusType state);

  std::unique_ptr<Export> export_;
  std::unique_ptr<Table> table_;
  GameState state_;
};

#endif  // CCGS_GAME_HPP_
