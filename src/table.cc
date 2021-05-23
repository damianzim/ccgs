#include "table.hpp"

#include <cmath>
#include <utility>

#include "deck.hpp"
#include "export.hpp"
#include "game_params.hpp"
#include "player.hpp"
#include "trait.hpp"

namespace {

/// @note The function expects that the arguments are positive.
bool ApproximatelyEqual(float a, float b) {
  return std::abs(a - b) <= (a < b ? b : a) * 0.001F;
}

}  // namespace

struct Table::PlayerCtx {
  MutableDeck controlled;
  CardsQueue discarded;
  CardsQueue hand;

  Card* current_card;

  std::unique_ptr<Player> player;
};

Table::Table(const GameParams& params, Export& export_,
             std::unique_ptr<Player> p1, std::unique_ptr<Player> p2)
    : params_(params),
      current_(std::make_shared<PlayerCtx>()),
      opponent_(std::make_shared<PlayerCtx>()),
      export_(export_),
      last_card_(nullptr) {
  current_->player = std::move(p1);
  opponent_->player = std::move(p2);
  for (auto& ctx : {current_, opponent_})
    for (int i = 0; i < params_.StartingCards(); ++i)
      ctx->hand.Push(ctx->player->PullCard());
}

Table::~Table() {
  for (auto& [owner, task] : tasks_) delete task;
}

void Table::GetFinalResult(GameState& state) {
  state.Set(current_->player->Name(), current_->controlled.GetStrength(),
            opponent_->player->Name(), opponent_->controlled.GetStrength());
}

GameState::StatusType Table::PlayTurn() {
  ScopeTrace scope{"PlayTurn"};
  ++turns_;
  PlaySubTurn();
  // Do not begin next turn if the 'first' player has no more cards.
  if (last_card_ == nullptr) return ResolveFinalResult();
  PlaySubTurn();
  if (turns_ == params_.TurnLimit()) {
    LOGW("The turn limit has been reached");
    return ResolveFinalResult();
  } else if (!opponent_->hand.Size() && !opponent_->player->HasCards()) {
    LOGW("{} will run out of cards in the next turn",
         opponent_->player->Name());
    return ResolveFinalResult();
  }
  return ResolveLeadingCondition() ? GameState::kDone : GameState::kInProgress;
}

void Table::LogTurnInfo(const ExportRowLabel label) const {
  using Label = ExportRowLabel;
  switch (label) {
    case Label::kIn:
    case Label::kOut: {
      auto l = label == Label::kIn ? "IN" : "OUT";
      export_.PushRow(turns_, subturn_, current_->player->Name(), l,
                      current_->controlled.Size(), current_->discarded.Size(),
                      current_->hand.Size(), std::nullopt, std::nullopt,
                      std::nullopt, std::nullopt, std::nullopt, std::nullopt,
                      std::nullopt, std::nullopt, std::nullopt, std::nullopt);
      break;
    }
    case Label::kCard: {
      auto card = current_->current_card;
      auto attrs = card->GetAttrs();
      auto traits = card->GetTraits();
      export_.PushRow(turns_, subturn_, current_->player->Name(), "CARD",
                      std::nullopt, std::nullopt, std::nullopt,
                      card->GetStrength(), attrs.water, attrs.fire,
                      attrs.nature, traits.swift, traits.symbiotic,
                      traits.poisonous, traits.empowering, traits.sabotaging,
                      traits.supporting);
      break;
    }
    default:
      LOGW("Unknown label for turn log");
  }
}

void Table::PlaySubTurn() {
  ScopeTrace scope{"PlaySubTurn"};
  LogTurnInfo(ExportRowLabel::kIn);
  LOGD("controlled {} discarded {} hand {}", current_->controlled.Size(),
       current_->discarded.Size(), current_->hand.Size());
  Card* last_card = nullptr;
  current_->hand.Push(current_->player->PullCard());
  Card* card = current_->hand.Pull();
  if (card != nullptr) {
    played_queue_.Push(card);
    while ((card = played_queue_.Pull()) != nullptr) {
      current_->controlled.Push(card);
      current_->current_card = card;
      LogTurnInfo(ExportRowLabel::kCard);
      RunTasks();
      if (last_card_ != nullptr) {
        card->ApplyAttrs(*last_card_);
        last_card_ = nullptr;
      }
      card->ApplyTraits(*this);
      last_card = card;
    }
  } else {
    LOGW("{} ran out of cards", current_->player->Name());
  }
  last_card_ = last_card;  // Only the last card played by current player will
                           // impact opponent's next card.
  LOGI("Strength after subturn {} {:6.2f} {} {:6.2f}", current_->player->Name(),
       current_->controlled.GetStrength(), opponent_->player->Name(),
       opponent_->controlled.GetStrength());
  SwapPlayers();
  // When last_card is null, it indicates that no card was played this subturn.
  LogTurnInfo(ExportRowLabel::kOut);
  subturn_ = subturn_ == 1 ? 2 : 1;
}

void Table::PushTask(Trait* task) {
  ScopeTrace scope{"PushTask"};
  std::shared_ptr<PlayerCtx> owner;
  switch (task->Owner()) {
    case TaskOwner::kCurrentPlayer:
      owner = current_;
      break;
    case TaskOwner::kOpponent:
      owner = opponent_;
      break;
    default:
      LOGC("Unknown task owner");
      return;
  }

  switch (task->ExecTime()) {
    case TaskExecTime::kNow: {
      // Maybe it will not be necessary to check the return statement in this
      // case. It depends whenever any case with time execution set to now will
      // return false.
      auto ctx = TaskContext(owner);
      if (task->Exec(ctx)) {
        delete task;
        break;
      }
      [[fallthrough]];
    }
    case TaskExecTime::kLater:
      tasks_.push_back(std::make_pair(std::move(owner), task));
      break;
    default:
      LOGC("Unknown task execution time");
  }
}

GameState::StatusType Table::ResolveFinalResult() {
  auto current = current_->controlled.GetStrength();
  auto opponent = opponent_->controlled.GetStrength();
  if (ApproximatelyEqual(current, opponent)) {
    LOGI("There is a draw {} {:6.2f} {} {:6.2f}", current_->player->Name(),
         current, opponent_->player->Name(), opponent);
    return GameState::kDraw;
  }
  auto diff = current - opponent;
  // If a winner can be emerged, then set current player as the winner.
  if (diff < 0) current_.swap(opponent_);
  LOGI("{} leads with {:.2f} points", current_->player->Name(), std::abs(diff));
  return GameState::kDone;
}

bool Table::ResolveLeadingCondition() {
  if (params_.PointsLeading() == 0) return false;
  auto current = current_->controlled.GetStrength();
  auto opponent = opponent_->controlled.GetStrength();
  auto diff = current - opponent;
  auto abs_diff = std::abs(diff);
  if (ApproximatelyEqual(abs_diff, params_.PointsLeading()) ||
      abs_diff < params_.PointsLeading())
    return false;
  // If a winner can be emerged, then set current player as the winner.
  if (diff < 0) current_.swap(opponent_);
  LOGI("Points leading condition met");
  LOGI("{} leads with {:.2f} points", current_->player->Name(), abs_diff);
  return true;
}

void Table::RunTasks() {
  ScopeTrace scope{"RunTasks"};
  LOGD("Task queue size {} (before)", tasks_.size());
  auto ctx = TaskContext(current_);
  auto it = tasks_.begin();
  while (it != tasks_.end()) {
    auto& [player, task] = *it;
    if (player == current_ && task->Exec(ctx)) {
      delete task;
      it = tasks_.erase(it);
    } else {
      ++it;
    }
  }
  LOGD("Task queue size {} (after)", tasks_.size());
}

void Table::SwapPlayers() {
  current_.swap(opponent_);
  LOGD("Players swapped: current {} opponent {}", current_->player->Name(),
       opponent_->player->Name());
}

TaskCtx Table::TaskContext(std::shared_ptr<PlayerCtx> owner) {
  using namespace std::placeholders;

  auto card = owner->current_card;
  return {owner->controlled, owner->discarded,
          owner->hand,       card,
          played_queue_,     std::bind(&Table::PushTask, std::ref(*this), _1)};
}
