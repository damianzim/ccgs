#ifndef CCGS_RANDOM_HPP_
#define CCGS_RANDOM_HPP_

#include <memory>
#include <random>

#include "common.hpp"

class DeviceGenerator {
 public:
  DeviceGenerator() : gen_(std::random_device()()){};

  inline std::mt19937& Engine() { return gen_; }
  unsigned long Random() { return gen_(); }

  /// @brief Generate random number in [min, max].
  long long RandomBetween(unsigned long min, unsigned long max);

  /// @brief Generate random number in [0, max].
  unsigned long RandomMax(unsigned long max);

 private:
  std::mt19937 gen_;
};

class Card;

class CardGenerator {
  static constexpr auto kPowerLevel = 250;

 public:
  CardGenerator(size_t balance)
      : balance_(balance * 10),
        max_(kPowerLevel + balance_),
        min_(kPowerLevel - balance_){};

  std::unique_ptr<Card> GetCard();

 private:
  const size_t balance_;
  const size_t max_;
  const size_t min_;

  DeviceGenerator gen_;
};

#endif  // CCGS_RANDOM_HPP_
