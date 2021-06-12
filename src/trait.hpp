#ifndef CCGS_TRAIT_HPP_
#define CCGS_TRAIT_HPP_

enum class TaskExecTime;
enum class TaskOwner;
struct TaskCtx;

class Trait {
 public:
  virtual ~Trait(){};

  // Duplicate the trait.
  virtual Trait* Clone() const = 0;

  // Execute the command. If the task has been executed correctly return true,
  // or false if it should be retried.
  virtual bool Exec(TaskCtx&) const = 0;

  // Indicate the time when the `Exec()` method is to be run for the first time.
  virtual TaskExecTime ExecTime() const = 0;

  // Indicate the player for whom this trait should be performed.
  virtual TaskOwner Owner() const = 0;
};

#endif  // CCGS_TRAIT_HPP_
