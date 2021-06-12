#ifndef CCGS_DECK_HPP_
#define CCGS_DECK_HPP_

#include <utility>
#include <vector>

#include "card.hpp"

class Deck {
 public:
  // Deck cards container is characterised by the fact that it can only be
  // filled while constructing the object.
  Deck(std::vector<Card*> cards);
  Deck(const Deck&) = delete;
  Deck& operator=(const Deck&) = delete;

  // Destruct the object and the cards if it contains them.
  ~Deck();

  // Get total strength of the cards present in the deck.
  Card::Strength GetStrength() const;
  size_t Size() const;

  // Pull random card from the deck.
  Card* PullRandom();

  // Pull a card from the end of the deck.
  Card* Pull();

 protected:
  // Construct empty deck (only for inheritance!).
  Deck() = default;

  std::vector<Card*> cards_;
};

class MutableDeck : public Deck {
 public:
  // Construct an empty MutableDeck.
  MutableDeck() = default;

  // Run the `condition` function for every card in the deck. Return a vector of
  // all cards satisfying the condtion, in other words, for cards for which the
  // function returned true.
  std::vector<Card*> Filter(Func<bool(Card*)> condition) const;


  // Push the `card` at the end of the deck.
  void Push(Card* card);
};

#endif  // CCGS_DECK_HPP_
