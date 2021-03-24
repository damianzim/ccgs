#include "random.hpp"

#include "cards.hpp"

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

  return std::make_unique<Card>(strength / 10.F, attrs,
                                Card::Traits::BuyTraits(power_level / 10));
}
