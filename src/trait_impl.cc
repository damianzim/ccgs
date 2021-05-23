#include "trait_impl.hpp"

#include "common.hpp"
#include "deck.hpp"
#include "random.hpp"
#include "table.hpp"

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
