#ifndef CCGS_TRAITS_HPP_
#define CCGS_TRAITS_HPP_

#include <type_traits>
#include <vector>

#include "trait.hpp"

class Table;

class Traits {
 public:
  struct TraitsTable {
    bool swift;
    bool symbiotic;
    bool poisonous;
    bool empowering;
    bool sabotaging;
    bool supporting;
  };

  Traits(int power_level);
  Traits(const Traits& other);
  Traits(Traits&& other);
  Traits& operator=(const Traits&) = delete;
  Traits& operator=(Traits&&) = delete;
  ~Traits();

  TraitsTable GetTraits() const;

  void ApplyTraits(Table& table);
  void MarkPoisonousUneffective();

 private:
  TraitsTable table_{};
  std::vector<Trait*> traits_;

  template <class T>
  void BuyTrait(int& power_level, int cost, bool& trait_in_table) {
    static_assert(std::is_base_of<Trait, T>::value);
    if (power_level < cost) return;
    power_level -= cost;
    traits_.emplace_back(new T);
    trait_in_table = true;
  }
};

#endif  // CCGS_TRAITS_HPP_
