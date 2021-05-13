#ifndef CCGS_PLAYER_HPP_
#define CCGS_PLAYER_HPP_

#include <memory>
#include <string>

#include "cards.hpp"

class Player {
 public:
  Player(std::unique_ptr<Deck> deck, std::string name = "UNNAMED")
      : name_(name), deck_(std::move(deck)) {
    LOGD("Creating player {} with deck size {}", name, deck_->Size());
  }

  bool HasCards() const { return deck_->Size(); }
  const char* Name() const { return name_.c_str(); }
  Card* PullCard() const { return deck_->Pull(); }

 private:
  const std::string name_;

  std::unique_ptr<Deck> deck_;
};

#endif  // CCGS_PLAYER_HPP_
