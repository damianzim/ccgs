#include "game.hpp"

namespace {

constexpr auto kDefaultBalance = 0;
constexpr auto kDefaultDeckSize = 30;
constexpr auto kDefaultPoolSize = 100;
constexpr auto kDefaultStartingCards = 5;
constexpr auto kDefaultTurnLimit = 10;
constexpr auto kDefaultPointsLeading = 50;

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
        ParseParam(args, "leading", 20, 100, kDefaultPointsLeading);
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

Table::Table(const GameParams& params, std::unique_ptr<Player> p1,
             std::unique_ptr<Player> p2)
    : params_(params),
      current_(std::make_shared<PlayerCtx>()),
      opponent_(std::make_shared<PlayerCtx>()),
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

bool Table::PlayTurn() {
  ScopeTrace scope{"PlayTurn"};
  if (turns_ == params_.TurnLimit()) {
    LOGW("The turn limit has been reached");
    return false;
  }
  for (int i = 0; i < 2; ++i)
    if (!PlaySubTurn()) return false;
  ++turns_;
  return true;
}

bool Table::PlaySubTurn() {
  ScopeTrace scope{"PlaySubTurn"};
  LOGD("controlled {} discarded {} hand {}", current_->controlled.Size(),
       current_->discarded.Size(), current_->hand.Size());
  Card* last_card;
  Card* card = current_->player->PullCard();
  if (card == nullptr) return false;

  played_queue_.Push(card);
  while ((card = played_queue_.Pull()) != nullptr) {
    current_->controlled.Push(card);
    current_->current_card = card;
    RunTasks();
    if (last_card_ != nullptr) {
      card->ApplyAttrs(*last_card_);
      last_card_ = nullptr;
    }
    card->ApplyTraits(*this);
    last_card = card;
  }
  last_card_ = last_card;  // Only the last card played by current player will
                           // impact opponent's next card.
  SwapPlayers();
  return true;
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
      assert("Unknown task owner");
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
      assert("Unknown task execution time");
  }
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
    }
    ++it;
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
          card->AttrsRef(),  card->StrengthRef(),
          played_queue_,     std::bind(&Table::PushTask, std::ref(*this), _1)};
}

Game::Game(const GameParams& params) : params_(params){};

bool Game::InitGame() {
  if (state_.Status() != GameState::kUninitialised) {
    LOGE("Cannot initialise the game again");
    return false;
  }

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

  table_ = std::make_unique<Table>(params_, std::move(players[0]),
                                   std::move(players[1]));

  state_.Set(GameState::kInitialised);
  LOGI("Game has been initialised");
  return true;
}

GameResult Game::Run() {
  while (state_.CanRun()) {
    if (!table_->PlayTurn()) state_.Set(GameResult::kDone);
  }

  return GameResult{state_};
}
