#include "Game.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

Game::Game() : players({}), suits({}), ranks({}), deck({}), drawPile({}), discardPile({}) {}

void Game::loadDeckFromFile(string filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::invalid_argument("Unable to open file.");
  }

  // Clear existing data before loading new deck
  deck.clear();
  drawPile.clear();
  ranks.clear();
  suits.clear();

  std::string line;

  // Read suits from the first line
  if (!std::getline(file, line))
  {
    throw std::runtime_error("Invalid file format: missing suits.");
  }

  if (line.empty())
  {
    throw std::runtime_error("Invalid file format: missing suits.");
  }

  std::istringstream issSuits(line);
  std::string suit;
  while (issSuits >> suit)
  {
    suits.push_back(suit);
  }

  // Read ranks from the second line
  if (!std::getline(file, line))
  {
    throw std::runtime_error("Invalid file format: missing ranks.");
  }
  if (line.empty())
  {
    throw std::runtime_error("Invalid file format: missing ranks.");
  }
  std::istringstream issRanks(line);
  std::string rank;
  while (issRanks >> rank)
  {
    ranks.push_back(rank);
  }

  bool contain8 = false;

  for (unsigned int i = 0; i < ranks.size(); i++)
  {
    if (ranks[i] == "8")
    {
      contain8 = true;
    }
  }

  if (!contain8)
  {
    throw std::runtime_error("No eight in the deck.");
  }

  // Read cards from the remaining lines
  while (std::getline(file, line))
  {
    if (line.empty())
    {
      throw std::runtime_error("Invalid file format: missing suits.");
      break;
    }
    // Tokenize the line
    std::istringstream iss(line);
    std::string rank, suit;
    if (!(iss >> rank >> suit))
    {
      throw std::runtime_error("Invalid file format: missing rank or suit.");
    }

    string extra;
    if (iss >> extra)
    {
      throw std::runtime_error("Extra data.");
    }

    // Check if rank and suit are valid
    bool validRank = false;
    for (const auto &validRankValue : ranks)
    {
      if (validRankValue == rank)
      {
        validRank = true;
        break;
      }
    }

    bool validSuit = false;
    for (const auto &validSuitValue : suits)
    {
      if (validSuitValue == suit)
      {
        validSuit = true;
        break;
      }
    }

    if (!validRank || !validSuit)
    {
      throw std::runtime_error("Invalid rank or suit.");
    }

    try
    {
      // Create a new Card object and add it to both deck and drawPile
      Card *newCard = new Card(rank, suit);
      deck.push_back(newCard);
      drawPile.push_back(newCard);
    }
    catch (std::invalid_argument &e)
    {
      // Catch any invalid_argument thrown by the Card constructor
      throw std::runtime_error("Invalid card: " + std::string(e.what()));
    }
  }

  // Reverse drawPile to match the specified order
  int start = 0;
  int end = drawPile.size() - 1;
  while (start < end)
  {
    std::swap(drawPile[start], drawPile[end]);
    start++;
    end--;
  }

  file.close();
}

void Game::addPlayer(bool isAI)
{
  Player *player = new Player(isAI);
  players.push_back(player);
}

void Game::drawCard(Player *p)
{

  if (drawPile.empty())
  {
    std::cout << "Draw pile, empty, flipping the discard pile.\n";
    if (drawPile.empty() && discardPile.empty())
    {
      throw std::runtime_error("Draw and discard piles are empty");
    }
    while (!discardPile.empty())
    {
      drawPile.push_back(discardPile.back());
      discardPile.pop_back();
    }
  }
  p->addToHand(drawPile.back());
  drawPile.pop_back();
}

Card *Game::deal(int numCards)
{
  discardPile.push_back(drawPile.back());
  Card *dc = discardPile[0]; // set it to the first card
  drawPile.pop_back();

  for (int i = 0; i < numCards; ++i)
  {
    for (Player *player : players)
    {
      drawCard(player);
    }
  }

  return dc;
  // return discardPile.back();
}

std::string Game::mostPlayedSuit()
{
  const int numSuits = suits.size();
  std::vector<int> suitCounts(numSuits, 0);

  for (Card *card : deck)
  {
    std::string suit = card->getSuit();
    int index = -1;
    for (int i = 0; i < numSuits; ++i)
    {
      if (suits[i] == suit)
      {
        index = i;
        break;
      }
    }
    if (index != -1)
    {
      suitCounts[index] += card->getTimesPlayed();
    }
  }

  int maxCount = 0;
  int maxCountIndex = 0;
  for (int i = 0; i < numSuits; ++i)
  {
    if (suitCounts[i] > maxCount)
    {
      maxCount = suitCounts[i];
      maxCountIndex = i;
    }
  }

  return suits[maxCountIndex];
}

int Game::runGame()
{
  int cp = 0;
  string currentRank = "";
  string currentSuit = "";
  bool win = false;

  currentRank = discardPile.back()->getRank();
  currentSuit = discardPile.back()->getSuit();

  while (!win)
  {
    Player *p = players.at(cp);
    cout << "Player " << cp << "'s turn!\n";
    Card *playedCard = p->playCard(suits, currentRank, currentSuit);
    if (playedCard != nullptr)
    {
      if (playedCard->getRank() != "8")
      {
        cout << "Player " << cp << " plays " << playedCard->getRank() << " " << playedCard->getSuit() << ".\n";
      }
      else
      {
        cout << "Player " << cp << " plays " << playedCard->getRank() << " " << playedCard->getSuit() << " and changes the suit to " << currentSuit << ".\n";
      }

      discardPile.push_back(playedCard);
    }
    else
    {
      try
      {
        drawCard(p);
        cout << "Player " << cp << " draws a card.\n";
      }
      catch (runtime_error &e)
      {
        cout << "Player " << cp << " cannot draw a card.\n";
        return -1;
      }
    }
    if (p->getHandSize() == 0)
    {
      win = true;
      return cp;
    }
    cp = (cp + 1) % players.size();
  }
  return cp;
}

Game::~Game()
{
  for (unsigned int i = 0; i < deck.size(); i++)
  {
    delete deck.at(i);
  }
  for (unsigned int i = 0; i < players.size(); i++)
  {
    delete players.at(i);
  }
}
