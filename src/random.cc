#include "random.hpp"

#include "card.hpp"
#include "game.hpp"

unsigned long DeviceGenerator::Random() { return gen_(); }

long long DeviceGenerator::RandomBetween(unsigned long lr, unsigned long rr) {
  if (lr > rr)
    return -1;
  else if (lr == rr)
    return lr;
  rr -= lr;
  return (Random() % ++rr) + lr;
}

unsigned long DeviceGenerator::RandomMax(unsigned long max) {
  if (!max) return 0;
  return Random() % (max + 1);
}

CardGenerator::CardGenerator(const GameParams& params)
    : balance_(params.Balance() * 10),
      max_(kPowerLevel + balance_),
      min_(kPowerLevel - balance_){};

std::unique_ptr<Card> CardGenerator::GetCard() {
  int power_level = !balance_ ? kPowerLevel : gen_.RandomBetween(min_, max_);

  int strength = gen_.RandomBetween(10, 90);
  power_level -= strength;

  Card::Attributes attrs{};
  for (auto attr : {&attrs.water, &attrs.fire, &attrs.nature}) {
    int attr_val = gen_.RandomMax(std::min<int>(5, power_level / 10));
    power_level -= attr_val * 10;
    *attr = attr_val;
  }

  power_level /= 10;  // Return to the natural scale of Power Level. Forget the
                      // fractional part because it will not be needed anymore.
  return std::make_unique<Card>(attrs, strength / 10.F, Traits(power_level));
}
