#ifndef CCGS_TABLE_HPP_
#define CCGS_TABLE_HPP_

#include <list>
#include <memory>

#include "cards_queue.hpp"
#include "common.hpp"
#include "game_state.hpp"

class MutableDeck;

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

class Export;
class GameParams;
enum class ExportRowLabel;
class Player;

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

  void LogTurnInfo(ExportRowLabel label) const;

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

#endif  // CCGS_TABLE_HPP_
