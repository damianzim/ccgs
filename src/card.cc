#include "card.hpp"

#include "game.hpp"

namespace cost {

constexpr auto kSwift = 15;
constexpr auto kSymbiotic = 11;
constexpr auto kPoisonous = 7;
constexpr auto kEmpowering = 4;
constexpr auto kSabotaging = 3;
constexpr auto kSupporting = 2;

}  // namespace cost

Swift* Swift::Clone() const { return new Swift(*this); }

bool Swift::Exec(TaskCtx& ctx) const {
  ScopeTrace scope{"Exec:Swift"};
  auto next_card = ctx.hand.Pull();
  if (next_card != nullptr)
    ctx.played_queue.Push(next_card);
  else
    LOGD("[Exec:Swift] No more cards to play");
  return true;
}

TaskExecTime Swift::ExecTime() const { return TaskExecTime::kNow; }
TaskOwner Swift::Owner() const { return TaskOwner::kCurrentPlayer; }

Symbiotic* Symbiotic::Clone() const { return new Symbiotic(*this); }

bool Symbiotic::Exec(TaskCtx& ctx) const {
  ScopeTrace scope{"Exec:Symbiotic"};
  auto ttable = ctx.card->GetTraits();
  if (ttable.symbiotic) {
    ctx.card->SetStrength(ctx.card->GetStrength() * 2);
    return true;
  }
  return false;
}

TaskExecTime Symbiotic::ExecTime() const { return TaskExecTime::kLater; }
TaskOwner Symbiotic::Owner() const { return TaskOwner::kCurrentPlayer; }

Poisonous* Poisonous::Clone() const { return new Poisonous(*this); }

bool Poisonous::Exec(TaskCtx& ctx) const {
  class PoisonousAffect : public Trait {
   public:
    PoisonousAffect* Clone() const override {
      return new PoisonousAffect(*this);
    }
    bool Exec(TaskCtx& ctx) const override {
      ScopeTrace scope{"Exec:PoisonousAffect"};
      auto discarded_card = ctx.controlled.PullRandom();
      if (discarded_card == nullptr)
        LOGW("[Exec:PoisonousAffect] There is no card to discard");  //?
      else
        ctx.discarded.Push(discarded_card);
      return true;
    }
    TaskExecTime ExecTime() const override { return TaskExecTime::kNow; }
    TaskOwner Owner() const override { return TaskOwner::kOpponent; }
  };

  ScopeTrace scope{"Exec:Poisonous"};
  auto poisonous = ctx.controlled.Filter(
      [](Card* card) { return card->GetTraits().poisonous; });
  if (poisonous.size() == 2) {
    ctx.card->MarkPoisonousUneffective();
    for (auto card : poisonous) card->MarkPoisonousUneffective();
    ctx.task_queue_push(new PoisonousAffect());
  }
  return true;
}

TaskExecTime Poisonous::ExecTime() const { return TaskExecTime::kNow; }
TaskOwner Poisonous::Owner() const { return TaskOwner::kCurrentPlayer; }

Empowering* Empowering::Clone() const { return new Empowering(*this); }

bool Empowering::Exec(TaskCtx& ctx) const {
  ScopeTrace scope{"Exec:Empowering"};
  auto attrs = ctx.card->GetAttrs();
  attrs.water += 1;
  attrs.fire += 1;
  attrs.nature += 1;
  ctx.card->SetAttrs(attrs);
  return true;
}

TaskExecTime Empowering::ExecTime() const { return TaskExecTime::kLater; }
TaskOwner Empowering::Owner() const { return TaskOwner::kCurrentPlayer; }

Sabotaging* Sabotaging::Clone() const { return new Sabotaging(*this); }

bool Sabotaging::Exec(TaskCtx& ctx) const {
  ScopeTrace scope{"Exec:Sabotaging"};
  auto attrs = ctx.card->GetAttrs();
  auto i = DeviceGenerator{}.Random() % 3;
  if (i == 0)
    attrs.water = 0;
  else if (i == 1)
    attrs.fire = 0;
  else
    attrs.nature = 0;
  ctx.card->SetAttrs(attrs);
  return true;
}

TaskExecTime Sabotaging::ExecTime() const { return TaskExecTime::kLater; }
TaskOwner Sabotaging::Owner() const { return TaskOwner::kOpponent; }

Supporting* Supporting::Clone() const { return new Supporting(*this); }

bool Supporting::Exec(TaskCtx& ctx) const {
  ScopeTrace scope{"Exec:Supporting"};
  ctx.card->SetStrength(ctx.card->GetStrength() + 1);
  return true;
}

TaskExecTime Supporting::ExecTime() const { return TaskExecTime::kLater; }
TaskOwner Supporting::Owner() const { return TaskOwner::kCurrentPlayer; }

Traits::Traits(int power_level) {
  using namespace cost;

  traits_.reserve(6);
  BuyTrait<Swift>(power_level, kSwift, table_.swift);
  BuyTrait<Symbiotic>(power_level, kSymbiotic, table_.symbiotic);
  BuyTrait<Poisonous>(power_level, kPoisonous, table_.poisonous);
  BuyTrait<Empowering>(power_level, kEmpowering, table_.empowering);
  BuyTrait<Sabotaging>(power_level, kSabotaging, table_.sabotaging);
  BuyTrait<Supporting>(power_level, kSupporting, table_.supporting);
  traits_.shrink_to_fit();
}

Traits::Traits(const Traits& other) : table_(other.table_) {
  auto size = other.traits_.size();
  traits_.reserve(size);
  for (size_t i = 0; i < size; ++i)
    traits_.emplace_back(other.traits_[i]->Clone());
}

Traits::Traits(Traits&& other) : table_(other.table_), traits_(other.traits_) {
  other.traits_.clear();
}

Traits::~Traits() {
  for (auto trait : traits_) delete trait;
}

void Traits::ApplyTraits(Table& table) {
  ScopeTrace scope{"ApplyTraits"};
  for (auto trait : traits_) table.PushTask(trait);
  traits_.clear();
}

void Traits::MarkPoisonousUneffective() {
  if (!traits_.size()) table_.poisonous = false;
}

Card::Card(Attributes attrs, Strength strength, Traits&& traits)
    : Traits(std::move(traits)), attrs_(attrs), strength_(strength) {
#ifdef DDEBUG
  auto tr = [](bool trait) -> char { return trait ? 'Y' : 'N'; };
  auto ttable = traits.GetTraits();
  LOGD(
      "ST={:04.1f} W={:d} F={:d} N={:d} sw={:c} sy={:c} po={:c} em={:c} "
      "sa={:c} su={:c}",
      strength, attrs.water, attrs.fire, attrs.nature, tr(ttable.swift),
      tr(ttable.symbiotic), tr(ttable.poisonous), tr(ttable.empowering),
      tr(ttable.sabotaging), tr(ttable.supporting));
#endif
};

void Card::ApplyAttrs(const Card& previous) {
  ScopeTrace scope{"ApplyAttrs"};
  auto other = previous.GetAttrs();
  float to_reduce = other.water * attrs_.fire + other.fire * attrs_.nature +
                    other.nature * attrs_.water;
  to_reduce /= 100;
  to_reduce *= strength_;
  LOGD("[ApplyAttrs] {:.2f} - {:.2f} = {:.2f}", strength_, to_reduce,
       strength_ - to_reduce);
  strength_ -= to_reduce;
}

void Card::SetAttrs(const Attributes& attrs) { attrs_ = attrs; }

void Card::SetStrength(const Strength& strength) { strength_ = strength; }
