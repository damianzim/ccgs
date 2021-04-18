#ifndef CCGS_CARD_HPP_
#define CCGS_CARD_HPP_

#include <vector>

#include "common.hpp"

enum class TaskExecTime;
enum class TaskOwner;
struct TaskCtx;

class Trait {
 public:
  virtual ~Trait(){};

  virtual Trait* Clone() const = 0;
  virtual bool Exec(TaskCtx&) const = 0;
  virtual TaskExecTime ExecTime() const = 0;
  virtual TaskOwner Owner() const = 0;
};

class Swift : public Trait {
 public:
  Swift* Clone() const override;
  bool Exec(TaskCtx&) const override;
  TaskExecTime ExecTime() const override;
  TaskOwner Owner() const override;
};

class Symbiotic : public Trait {
 public:
  Symbiotic* Clone() const override;
  bool Exec(TaskCtx&) const override;
  TaskExecTime ExecTime() const override;
  TaskOwner Owner() const override;
};

class Poisonous : public Trait {
 public:
  Poisonous* Clone() const override;
  bool Exec(TaskCtx&) const override;
  TaskExecTime ExecTime() const override;
  TaskOwner Owner() const override;
};

class PoisonousAffect : public Trait {
 public:
  PoisonousAffect* Clone() const override;
  bool Exec(TaskCtx&) const override;
  TaskExecTime ExecTime() const override;
  TaskOwner Owner() const override;
};

class Empowering : public Trait {
 public:
  Empowering* Clone() const override;
  bool Exec(TaskCtx&) const override;
  TaskExecTime ExecTime() const override;
  TaskOwner Owner() const override;
};

class Sabotaging : public Trait {
 public:
  Sabotaging* Clone() const override;
  bool Exec(TaskCtx&) const override;
  TaskExecTime ExecTime() const override;
  TaskOwner Owner() const override;
};

class Supporting : public Trait {
 public:
  Supporting* Clone() const override;
  bool Exec(TaskCtx&) const override;
  TaskExecTime ExecTime() const override;
  TaskOwner Owner() const override;
};

class Table;

class Traits {
 public:
  struct TraitsTable {
    bool swift;
    bool symbiotic;
    bool poisonous;
    bool empowering;
    bool sabotaging;
    bool supporting;
  };

  Traits(int power_level);
  Traits(const Traits& other);
  Traits(Traits&& other);
  Traits& operator=(const Traits&) = delete;
  Traits& operator=(Traits&&) = delete;
  ~Traits();

  TraitsTable GetTraits() const { return table_; }

  void ApplyTraits(Table& table);
  void MarkPoisonousUneffective();

 private:
  TraitsTable table_{};
  std::vector<Trait*> traits_;

  template <class T>
  void BuyTrait(int& power_level, int cost, bool& trait_in_table) {
    static_assert(std::is_base_of<Trait, T>::value);
    if (power_level < cost) return;
    power_level -= cost;
    traits_.emplace_back(new T);
    trait_in_table = true;
  }
};

class Card : public Traits {
  friend class Table;

 public:
  using AttrValue = uint8_t;
  using Strength = float;

  struct Attributes {
    AttrValue water;
    AttrValue fire;
    AttrValue nature;
  };

  Card(Attributes attrs, Strength strength, Traits&& traits);
  Card(const Card&) = default;
  Card(Card&&) = default;
  Card& operator=(const Card&) = delete;
  Card& operator=(Card&&) = delete;

  Strength GetStrength() const { return strength_; }
  Attributes GetAttrs() const { return attrs_; }

  void ApplyAttrs(const Card& previous);

 private:
  Attributes attrs_;
  Strength strength_;

  Strength& StrengthRef() { return strength_; }
  Attributes& AttrsRef() { return attrs_; }
};

#endif  // CCGS_CARD_HPP_
