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
  // Construct an object of Table, assign players to it and initialise their
  // hands with appropriate number of cards.
  Table(const GameParams& params, Export& export_, std::unique_ptr<Player> p1,
        std::unique_ptr<Player> p2);

  // Destruct the object and its owned traits.
  ~Table();

  // Save final result to the state object.
  void GetFinalResult(GameState& state);

  // Play a turn which consists of two sub turns. At the end check if the game
  // end condition is met.
  GameState::StatusType PlayTurn();

 private:
  using TaskQueue = std::list<std::pair<std::shared_ptr<PlayerCtx>, Trait*> >;

  const GameParams& params_;

  // Export current simulation snapshot depending on the `label` parameter.
  void LogTurnInfo(ExportRowLabel label) const;

  // Play a sub turn and log the current simulation state to the file. During
  // the sub turn there may be played more cards than one (played_queue_), but
  // the limit is when the player's hand ran out of cards.
  void PlaySubTurn();

  // Add a trait task to the tasks queue.
  void PushTask(Trait* task);

  // Check if it is a draw or if there is a winner.
  GameState::StatusType ResolveFinalResult();

  // Check if one of the two palyers is leading.
  bool ResolveLeadingCondition();

  // Run the traits' tasks towards current player.
  void RunTasks();

  // Swap current and opponent player.
  void SwapPlayers();

  // Get the current game's context used to execute the traits.
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
