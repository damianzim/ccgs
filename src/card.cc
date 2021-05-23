#include "card.hpp"

#include "table.hpp"

Card::Card(Attributes attrs, Strength strength, Traits&& traits)
    : Traits(std::move(traits)), attrs_(attrs), strength_(strength) {
#ifdef DDEBUG
  auto tr = [](bool trait) -> char { return trait ? 'Y' : 'N'; };
  auto ttable = traits.GetTraits();
  LOGD(
      "ST={:04.1f} W={:d} F={:d} N={:d} sw={:c} sy={:c} po={:c} em={:c} "
      "sa={:c} su={:c}",
      strength, attrs.water, attrs.fire, attrs.nature, tr(ttable.swift),
      tr(ttable.symbiotic), tr(ttable.poisonous), tr(ttable.empowering),
      tr(ttable.sabotaging), tr(ttable.supporting));
#endif
};

Card::Strength Card::GetStrength() const { return strength_; }

Card::Attributes Card::GetAttrs() const { return attrs_; }

void Card::ApplyAttrs(const Card& previous) {
  ScopeTrace scope{"ApplyAttrs"};
  auto other = previous.GetAttrs();
  float to_reduce = other.water * attrs_.fire + other.fire * attrs_.nature +
                    other.nature * attrs_.water;
  to_reduce /= 100;
  to_reduce *= strength_;
  LOGD("[ApplyAttrs] {:.2f} - {:.2f} = {:.2f}", strength_, to_reduce,
       strength_ - to_reduce);
  strength_ -= to_reduce;
}

void Card::SetAttrs(const Attributes& attrs) { attrs_ = attrs; }

void Card::SetStrength(const Strength& strength) { strength_ = strength; }
