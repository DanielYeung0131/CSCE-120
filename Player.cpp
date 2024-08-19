#include <iostream>
#include <sstream>
#include <vector>
#include "Player.h"

using namespace std;

Player::Player(bool isAI) : isAI(isAI), hand()
{
}

void Player::addToHand(Card *c)
{
  hand.push_back(c);
}

size_t Player::getHandSize()
{
  return hand.size();
}

std::string Player::getHandString()
{
  std::string handStr;
  for (size_t i = 0; i < hand.size(); ++i)
  {
    handStr += hand[i]->getRank() + " " + hand[i]->getSuit();
    if (i != hand.size() - 1)
    {
      handStr += ", ";
    }
  }
  return handStr;
}

Card *Player::playCard(vector<string> const &suits, string &currentRank, string &currentSuit)
{
  if (isAI == false)
  {
    unsigned pos;

    // For human players, you must tell them the current rank and suit and ask them to choose a card to play (see the sample execution above)
    string rank;
    string suit;

    cout << "Your hand contains: " << this->getHandString() << endl;
    cout << "The next card played must be a " << currentRank << " or " << currentSuit << endl;
    cout << "What would you like to play? (enter \"draw card\" to draw a card)\n";

    while (true)
    {
      cin >> rank >> suit;

      if (rank == "draw" && suit == "card")
      {
        return nullptr;
      }

      bool in = false;

      for (unsigned i = 0; i < hand.size(); ++i)
      {
        if (hand[i]->getSuit() == suit && hand[i]->getRank() == rank)
        {
          in = true;
          pos = i;
          break;
        }
      }

      if (in == false)
      {
        cout << "That's not a card you have. Try again.\n";
        continue;
      }

      if (!(hand[pos]->canBePlayed(currentRank, currentSuit)))
      {
        cout << "You can't play that card. Try again.\n";
        continue;
      }

      if (hand[pos]->canBePlayed(currentRank, currentSuit))
      {
        Card *chosenCard = hand[pos];
        hand.erase(hand.begin() + pos);

        // Update currentRank and currentSuit
        currentRank = chosenCard->getRank();
        currentSuit = chosenCard->getSuit();

        string newSuit;
        if (chosenCard->getRank() == "8")
        {
          cout << "What suit would you like to declare?\n";
          while (true)
          {
            cin >> newSuit;
            bool s = false;
            for (unsigned i = 0; i < suits.size(); i++)
            {
              if (suits[i] == newSuit)
              {
                s = true;
                break;
              }
            }
            if (s == true)
            {
              currentSuit = newSuit;
              break;
            }
            cout << "That's not a suit in this deck. Try again.\n";
          }
        }

        // Increment timesPlayed
        chosenCard->play();
        return chosenCard; // Return the chosen card and exit the function
        break;             // Exit the loop after successfully playing a card
      }
    }
  }

  if (isAI == true)
  {
    for (unsigned int i = 0; i < hand.size(); ++i)
    {
      if (hand[i]->canBePlayed(currentRank, currentSuit))
      {
        Card *chosenCard = hand[i];
        hand.erase(hand.begin() + i);

        // Update currentRank and currentSuit
        currentRank = chosenCard->getRank();
        currentSuit = chosenCard->getSuit();

        // Increment timesPlayed
        chosenCard->play();

        // Handling playing an 8
        if (chosenCard->getRank() == "8")
        {
          // For AI players, choose the current suit
          currentSuit = chosenCard->getSuit(); // Choose the first suit
        }

        return chosenCard;
      }
    }
    return nullptr;
  }

  return nullptr;
}