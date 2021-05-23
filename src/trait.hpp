#ifndef CCGS_TRAIT_HPP_
#define CCGS_TRAIT_HPP_

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

#endif  // CCGS_TRAIT_HPP_
