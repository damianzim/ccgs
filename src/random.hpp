#ifndef CCGS_RANDOM_HPP_
#define CCGS_RANDOM_HPP_

#include <initializer_list>
#include <memory>
#include <random>

#include "common.hpp"

class DeviceGenerator {
 public:
  DeviceGenerator() : gen_(std::random_device()()){};

  inline std::mt19937& Engine() { return gen_; }
  unsigned long Random();

  /// @brief Generate random number in [min, max].
  long long RandomBetween(unsigned long min, unsigned long max);

  /// @brief Generate random number in [0, max].
  unsigned long RandomMax(unsigned long max);

 private:
  std::mt19937 gen_;
};

class Card;
class GameParams;
class CardGenerator {
  static constexpr auto kPowerLevel = 250;

 public:
  CardGenerator(const GameParams& params);

  std::unique_ptr<Card> GetCard();

 private:
  const size_t balance_;
  const size_t max_;
  const size_t min_;

  DeviceGenerator gen_;
};

#endif  // CCGS_RANDOM_HPP_
