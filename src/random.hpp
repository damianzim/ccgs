#ifndef CCGS_RANDOM_HPP_
#define CCGS_RANDOM_HPP_

#include <initializer_list>
#include <memory>
#include <random>

#include "common.hpp"

class DeviceGenerator {
 public:
  DeviceGenerator() : gen_(std::random_device()()){};

  // Get the generator's reference represented by this object.
  inline std::mt19937& Engine() { return gen_; }

  // Generate random numbers 32-bit unsigned number.
  unsigned long Random();

  // Generate random number in [min, max].
  long long RandomBetween(unsigned long min, unsigned long max);

  // Generate random number in [0, max].
  unsigned long RandomMax(unsigned long max);

 private:
  std::mt19937 gen_;
};

class Card;
class GameParams;
class CardGenerator {
  static constexpr auto kPowerLevel = 250;

 public:
  // Construct object of card generator and use for this balance from given game
  // params object.
  CardGenerator(const GameParams& params);

  // Generate random card which power level depends on the game's balance
  // parameter.
  std::unique_ptr<Card> GetCard();

 private:
  const size_t balance_;
  const size_t max_;
  const size_t min_;

  DeviceGenerator gen_;
};

#endif  // CCGS_RANDOM_HPP_
