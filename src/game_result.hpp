#ifndef CCGS_GAME_RESULT_HPP_
#define CCGS_GAME_RESULT_HPP_

#include <string_view>

class GameResult {
 public:
  using StatusType = int;

  struct FinalPlayerResult {
    std::string_view name;
    float strength;
  };

  using FinalResultType = FinalPlayerResult[2];

  enum : StatusType {
    kUninitialised,
    kInitialised,
    kInProgress,
    kFail,  //?
    kDraw,
    kDone,
  };

  FinalResultType const& FinalResult() const;
  bool Ok() const;
  StatusType Status() const;
  std::string_view StatusStringify() const;

 protected:
  FinalPlayerResult final_[2]{};
  int status_{kUninitialised};
};

#endif  // CCGS_GAME_RESULT_HPP_
