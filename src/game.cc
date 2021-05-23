#include "game.hpp"

#include <cmath>

namespace {

constexpr auto kDefaultBalance = 0;
constexpr auto kDefaultDeckSize = 30;
constexpr auto kDefaultPoolSize = 100;
constexpr auto kDefaultStartingCards = 5;
constexpr auto kDefaultTurnLimit = 10;
constexpr auto kDefaultPointsLeading = 50;

/// @note The function expects that the arguments are positive.
bool ApproximatelyEqual(float a, float b) {
  return std::abs(a - b) <= (a < b ? b : a) * 0.001F;
}

}  // namespace

class GameParams::InvalidParamValue : public std::runtime_error {
  std::string msg_;

 public:
  explicit InvalidParamValue(const std::string& param_name)
      : std::runtime_error("") {
    msg_ = "Invalid value for --" + param_name + ", see usage.";
  }
  const char* what() const noexcept override { return msg_.c_str(); }
};

std::optional<GameParams> GameParams::Parse(const Args& args) {
  GameParams p;
  try {
    p.balance_ = ParseParam(args, "balance", 0, 5, kDefaultBalance);
    p.deck_size_ = ParseParam(args, "deck-size", 15, 50, kDefaultDeckSize);
    p.pool_size_ = ParseParam(args, "pool-size", p.deck_size_ + 1,
                              p.deck_size_ * p.deck_size_, kDefaultPoolSize);
    p.starting_cards_ = ParseParam(args, "cards", 3, 10, kDefaultStartingCards);
    p.turn_limit_ = ParseParam(args, "turn-limit", 3, 20, kDefaultTurnLimit);
    p.points_leading_ =
        ParseParam(args, "leading", 0, 100, kDefaultPointsLeading);
    if (p.points_leading_ > 0 && p.points_leading_ < 20)
      throw InvalidParamValue{"leading"};
  } catch (const InvalidParamValue& e) {
    LOGE(e.what());
    return std::nullopt;
  }
  return p;
}

int GameParams::ParseParam(const Args& args, const std::string& name,
                           const int min, const int max,
                           const int& default_value) {
  auto arg_value = args.GetValue(name);
  if (arg_value == nullptr) return default_value;
  auto value = Parser::ParseInt(arg_value);
  if (!value || *value < min || *value > max) throw InvalidParamValue(name);
  return *value;
}
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

void Table::LogTurnInfo(const Export::RowLabel label) const {
  using Label = Export::RowLabel;
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
  LogTurnInfo(Export::RowLabel::kIn);
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
      LogTurnInfo(Export::RowLabel::kCard);
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
  LogTurnInfo(Export::RowLabel::kOut);
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

Game::Game(const GameParams& params) : params_(params){};

bool Game::InitGame(const char* output_dir) {
  if (state_.Status() != GameState::kUninitialised) {
    LOGE("Cannot initialise the game again");
    return false;
  }

  try {
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
