#ifndef CCGS_GAME_HPP_
#define CCGS_GAME_HPP_

#include <list>
#include <memory>
#include <optional>
#include <string>

#include "args.hpp"
#include "cards.hpp"
#include "common.hpp"
#include "player.hpp"
#include "random.hpp"

class GameParams {
 public:
  static std::optional<GameParams> Parse(const Args& args);

  int Balance() const { return balance_; }
  int DeckSize() const { return deck_size_; }
  int PoolSize() const { return pool_size_; }
  int StartingCards() const { return starting_cards_; }
  int TurnLimit() const { return turn_limit_; }
  int PointsLeading() const { return points_leading_; }

 private:
  class InvalidParamValue;

  GameParams(){};

  static int ParseParam(const Args& args, const std::string& name,
                        const int min, const int max, const int& default_value);

  int balance_;
  int deck_size_;
  int pool_size_;
  int starting_cards_;
  int turn_limit_;
  int points_leading_;
};

class GameResult {
 public:
  using StateType = int;
  enum : StateType {
    kUninitialised,
    kInitialised,  //?
    kInProgress,
    kFail,
    kDraw,
    kDone,
  };

  inline bool Ok() const { return status_ == kDone || status_ == kDraw; }

  virtual void Set(StateType) {}

 protected:
  int status_{kUninitialised};
};

class GameState : public GameResult {
 public:
  inline bool CanRun() const {
    return status_ == kInitialised || status_ == kInProgress;
  }

  bool Status() const { return status_; }

  void Set(StateType state) override { status_ = state; }
};

enum class TaskOwner {
  kCurrentPlayer,
  kOpponent,
};

enum class TaskExecTime {
  kNow,
  kLater,
};

struct TaskCtx {
  // Card conatiners
  MutableDeck& controlled;
  CardsQueue& discarded;
  CardsQueue& hand;

  // This card
  Card* card;
  Card::Attributes& attrs;
  Card::Strength& strength;

  CardsQueue& played_queue;
  Func<void(Trait*)> task_queue_push;
};

class Table {
  struct PlayerCtx;
  friend void Traits::ApplyTraits(Table&);

 public:
  Table(const GameParams& params, std::unique_ptr<Player> p1,
        std::unique_ptr<Player> p2);
  ~Table();

  bool PlayRound();

 private:
  using TaskQueue = std::list<std::pair<std::shared_ptr<PlayerCtx>, Trait*> >;

  const GameParams& params_;

  void PushTask(Trait* task);
  void RunTasks();
  void SwapPlayers();
  TaskCtx TaskContext(std::shared_ptr<PlayerCtx> owner);

  std::shared_ptr<PlayerCtx> current_;
  std::shared_ptr<PlayerCtx> opponent_;

  CardsQueue played_queue_;
  TaskQueue tasks_;
};

class Game {
 public:
  Game(const GameParams& params);

  bool InitGame();
  GameResult Run();

 private:
  const GameParams& params_;

  void SetState(GameResult::StateType state);

  std::unique_ptr<Table> table_;
  GameState state_;
};

#endif  // CCGS_GAME_HPP_
