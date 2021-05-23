#include "game_result.hpp"

GameResult::FinalResultType const& GameResult::FinalResult() const {
  return final_;
}

bool GameResult::Ok() const { return status_ == kDone || status_ == kDraw; }

GameResult::StatusType GameResult::Status() const { return status_; }

std::string_view GameResult::StatusStringify() const {
  switch (status_) {
    case kUninitialised:
      return "uninitialised";
    case kInitialised:
      return "initialised";
    case kInProgress:
      return "in progress";
    case kFail:
      return "fail";
    case kDraw:
      return "draw";
    case kDone:
      return "done";
    default:
      return "unknown";
  }
}
