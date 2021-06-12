#ifndef CCGS_PLAYER_HPP_
#define CCGS_PLAYER_HPP_

#include <memory>
#include <string>

#include "deck.hpp"
class Player {
 public:
  // Create player with deck and name.
  Player(std::unique_ptr<Deck> deck, std::string name = "UNNAMED")
      : name_(name), deck_(std::move(deck)) {
    LOGD("Creating player {} with deck size {}", name, deck_->Size());
  }

  // Check if the player has cards.
  bool HasCards() const { return deck_->Size(); }

  // Get the player's name.
  const char* Name() const { return name_.c_str(); }

  // Get a card from the player's deck.
  Card* PullCard() const { return deck_->Pull(); }

 private:
  const std::string name_;

  std::unique_ptr<Deck> deck_;
};

#endif  // CCGS_PLAYER_HPP_
