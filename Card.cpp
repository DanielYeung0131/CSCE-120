#include <iostream>

#include "Card.h"

using std::string;

Card::Card(string rank, string suit) : rank(rank), suit(suit), timesPlayed(0)
{
  // Check if rank or suit are empty
  if (rank.empty() || suit.empty())
  {
    throw std::invalid_argument("Rank or suit cannot be empty.");
  }
  // Check if rank or suit contain non-alphanumeric characters
  for (char c : rank)
  {
    if (!isalnum(c))
    {
      throw std::invalid_argument("Rank contains non-alphanumeric characters.");
    }
  }
  for (char c : suit)
  {
    if (!isalnum(c))
    {
      throw std::invalid_argument("Suit contains non-alphanumeric characters.");
    }
  }
}

string Card::getRank()
{
  return rank;
}

string Card::getSuit()
{
  return suit;
}

int Card::getTimesPlayed()
{
  return timesPlayed;
}

bool Card::canBePlayed(string currentRank, string currentSuit)
{
  // If the current card is an 8, it can always be played
  if (rank == "8")
  {
    return true;
  }

  // Check if the card matches the current rank or suit
  return (rank == currentRank || suit == currentSuit);
}

void Card::play()
{
  timesPlayed++;
}
