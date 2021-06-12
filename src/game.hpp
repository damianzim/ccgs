#ifndef CCGS_GAME_HPP_
#define CCGS_GAME_HPP_

#include <memory>

#include "export.hpp"
#include "game_state.hpp"
#include "table.hpp"

class GameParams;

class Game {
 public:
  // Construct game object with the `params` parameters. For playing must be
  // initialised with `InitGame()` method.
  Game(const GameParams& params);

  // Initalise game which output will be saved in the `output_dir` (must contain
  // valid string or will fail) directory. Create table and assigned to it
  // players with decks.
  bool InitGame(const char* output_dir);

  // Run the simulation until the end.
  GameResult Run();

 private:
  const GameParams& params_;

  std::unique_ptr<Export> export_;
  std::unique_ptr<Table> table_;
  GameState state_;
};

#endif  // CCGS_GAME_HPP_
