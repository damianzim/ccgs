#ifndef CCGS_GAME_HPP_
#define CCGS_GAME_HPP_

#include <list>
#include <memory>
#include <optional>
#include <string>

#include "args.hpp"
#include "cards.hpp"
#include "common.hpp"
#include "export.hpp"
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
  using StatusType = int;

  struct FinalPlayerResult {
    std::string_view name;
    float strength;
  };

  using FinalResultType = FinalPlayerResult[2];

  enum : StatusType {
    kUninitialised,
    kInitialised,
    kInProgress,
    kFail,  //?
    kDraw,
    kDone,
  };

  FinalResultType const& FinalResult() const { return final_; }
  inline bool Ok() const { return status_ == kDone || status_ == kDraw; }
  StatusType Status() const { return status_; }
  std::string_view StatusStringify() const {
    switch (status_) {
      case kUninitialised:
        return "uninitialised";
      case kInitialised:
        return "initialised";
      case kInProgress:
        return "in progress";
      case kFail:
        return "fail";
      case kDraw:
        return "draw";
      case kDone:
        return "done";
      default:
        return "unknown";
    }
  }

 protected:
  FinalPlayerResult final_[2]{};
  int status_{kUninitialised};
};

class GameState : public GameResult {
 public:
  inline bool CanRun() const {
    return status_ == kInProgress || status_ == kInitialised;
  }

  void Set(StatusType state) { status_ = state; }
  void Set(const std::string_view& p1_name, const float p1_strength,
           const std::string_view& p2_name, const float p2_strength) {
    final_[0].name = p1_name;
    final_[0].strength = p1_strength;
    final_[1].name = p2_name;
    final_[1].strength = p2_strength;
  }
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

  CardsQueue& played_queue;
  Func<void(Trait*)> task_queue_push;
};

class Table {
  struct PlayerCtx;
  friend void Traits::ApplyTraits(Table&);

 public:
  Table(const GameParams& params, Export& export_, std::unique_ptr<Player> p1,
        std::unique_ptr<Player> p2);
  ~Table();

  void GetFinalResult(GameState& state);
  GameState::StatusType PlayTurn();

 private:
  using TaskQueue = std::list<std::pair<std::shared_ptr<PlayerCtx>, Trait*> >;

  const GameParams& params_;

  void LogTurnInfo(Export::RowLabel label) const;

  void PlaySubTurn();
  void PushTask(Trait* task);
  GameState::StatusType ResolveFinalResult();
  bool ResolveLeadingCondition();
  void RunTasks();
  void SwapPlayers();
  TaskCtx TaskContext(std::shared_ptr<PlayerCtx> owner);

  std::shared_ptr<PlayerCtx> current_;
  std::shared_ptr<PlayerCtx> opponent_;

  Export& export_;
  Card const* last_card_;
  CardsQueue played_queue_;
  int subturn_{1};
  TaskQueue tasks_;
  int turns_{0};
};

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
