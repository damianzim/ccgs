#ifndef CCGS_TRAIT_IMPL_HPP_
#define CCGS_TRAIT_IMPL_HPP_

#include "trait.hpp"

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

#endif  // CCGS_TRAIT_IMPL_HPP_
