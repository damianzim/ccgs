#include "game.hpp"

#include <string>

#include "cards_pool.hpp"
#include "common.hpp"
#include "player.hpp"
#include "random.hpp"

Game::Game(const GameParams& params) : params_(params){};

bool Game::InitGame(const char* output_dir) {
  if (state_.Status() != GameState::kUninitialised) {
    LOGE("Cannot initialise the game again");
    return false;
  }

  try {
    if (output_dir == nullptr) return false;
    export_ = std::make_unique<Export>(output_dir);
  } catch (std::filesystem::filesystem_error& e) {
    LOGC(e.what());
    return false;
  }
  export_->DumpGameParams(params_);

  CardGenerator gen{params_};
  CardsPool pool{};
  pool.InitPool(params_, gen);

  std::unique_ptr<Player> players[2];

  for (int i = 0; i < 2; ++i) {
    auto deck = pool.GetDeck(params_);
    if (!deck) return false;

    players[i] =
        std::make_unique<Player>(std::move(deck), "P" + std::to_string(i + 1));
  }

  table_ = std::make_unique<Table>(params_, *export_, std::move(players[0]),
                                   std::move(players[1]));

  state_.Set(GameState::kInitialised);
  LOGI("Game has been initialised");
  return true;
}

GameResult Game::Run() {
  if (state_.Status() != GameState::kInitialised) {
    LOGE("Cannot play not initialised game");
    return state_;
  }

  while (state_.CanRun()) state_.Set(table_->PlayTurn());

  if (state_.Ok()) table_->GetFinalResult(state_);
  export_->DumpFinalResult(state_);

  return state_;
}
