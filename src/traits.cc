#include "traits.hpp"

#include "common.hpp"
#include "table.hpp"
#include "trait_impl.hpp"

namespace cost {

constexpr auto kSwift = 15;
constexpr auto kSymbiotic = 11;
constexpr auto kPoisonous = 7;
constexpr auto kEmpowering = 4;
constexpr auto kSabotaging = 3;
constexpr auto kSupporting = 2;

}  // namespace cost

Traits::Traits(int power_level) {
  using namespace cost;

  traits_.reserve(6);
  BuyTrait<Swift>(power_level, kSwift, table_.swift);
  BuyTrait<Symbiotic>(power_level, kSymbiotic, table_.symbiotic);
  BuyTrait<Poisonous>(power_level, kPoisonous, table_.poisonous);
  BuyTrait<Empowering>(power_level, kEmpowering, table_.empowering);
  BuyTrait<Sabotaging>(power_level, kSabotaging, table_.sabotaging);
  BuyTrait<Supporting>(power_level, kSupporting, table_.supporting);
  traits_.shrink_to_fit();
}

Traits::Traits(const Traits& other) : table_(other.table_) {
  auto size = other.traits_.size();
  traits_.reserve(size);
  for (size_t i = 0; i < size; ++i)
    traits_.emplace_back(other.traits_[i]->Clone());
}

Traits::Traits(Traits&& other) : table_(other.table_), traits_(other.traits_) {
  other.traits_.clear();
}

Traits::~Traits() {
  for (auto trait : traits_) delete trait;
}

Traits::TraitsTable Traits::GetTraits() const { return table_; }

void Traits::ApplyTraits(Table& table) {
  ScopeTrace scope{"ApplyTraits"};
  for (auto trait : traits_) table.PushTask(trait);
  traits_.clear();
}

void Traits::MarkPoisonousUneffective() {
  if (!traits_.size()) table_.poisonous = false;
}
