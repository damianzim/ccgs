#ifndef CCGS_GAME_PARAMS_HPP_
#define CCGS_GAME_PARAMS_HPP_

#include <optional>
#include <string>

#include "args.hpp"

class GameParams {
 public:
  // Return GameParams object. Look for valid game parameters in the `args`
  //  object and try to parse them. If some parameter is present but contains
  // invalid or illicit value at once return nullopt. By default the parameters
  // adopt default values.
  static std::optional<GameParams> Parse(const Args& args);

  int Balance() const { return balance_; }
  int DeckSize() const { return deck_size_; }
  int PoolSize() const { return pool_size_; }
  int StartingCards() const { return starting_cards_; }
  int TurnLimit() const { return turn_limit_; }
  int PointsLeading() const { return points_leading_; }

 private:
  class InvalidParamValue;

  // Construct GameParams object with uninitialised parameter values.
  GameParams(){};

  // Look for and parse game parameter `name` in `args`. Parsed parameter's
  // value should be in [min, max], otherwise throw
  // GameParams::InvalidParamValue exception. If the parameter is not present
  // return `default_value`.
  static int ParseParam(const Args& args, const std::string& name,
                        const int min, const int max, const int& default_value);

  int balance_;
  int deck_size_;
  int pool_size_;
  int starting_cards_;
  int turn_limit_;
  int points_leading_;
};

#endif  // CCGS_GAME_PARAMS_HPP_
