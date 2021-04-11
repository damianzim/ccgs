#ifndef CCGS_GAME_HPP_
#define CCGS_GAME_HPP_

#include <optional>
#include <string>

#include "args.hpp"
#include "common.hpp"

class GameParams {
 public:
  static std::optional<GameParams> Parse(const Args& args);

  int Balance() const { return balance_; }
  int DeckSize() const { return deck_size_; }
  int PoolSize() const { return pool_size_; }
  int StartingCards() const { return starting_cards_; }
  int TurnLimit() const { return turn_limit_; }
  int PointsLeading() const { return points_leading_; }

 private:
  class InvalidParamValue : public std::runtime_error {
    std::string msg_;

   public:
    explicit InvalidParamValue(const std::string& param_name)
        : std::runtime_error("") {
      msg_ = "Invalid value for --" + param_name + ", see usage.";
    }
    const char* what() const noexcept override { return msg_.c_str(); }
  };

  GameParams(){};

  static int ParseParam(const Args& args, const std::string& name,
                        const int min, const int max,
                        const int& default_value) {
    auto arg_value = args.GetValue(name);
    if (arg_value == nullptr) return default_value;
    auto value = Parser::ParseInt(arg_value);
    if (!value || *value < min || *value > max) throw InvalidParamValue(name);
    return *value;
  }

  int balance_;
  int deck_size_;
  int pool_size_;
  int starting_cards_;
  int turn_limit_;
  int points_leading_;
};

#endif  // CCGS_GAME_HPP_
