#include <iostream>
#include <string>
#include "Card.h"
#include "Game.h"
#include "Player.h"

using namespace std;

bool loadDeck(Game &g)
{
  string filename;
  std::cout << "Choose a file to load the deck from:" << std::endl;
  std::cin >> filename;
  try
  {
    g.loadDeckFromFile(filename);
  }
  catch (std::invalid_argument const &)
  {
    std::cout << "The file was invalid. Aborting." << std::endl;
    return false;
  }
  return true;
}

int getPlayerCount()
{
  std::cout << "Enter number of players:" << std::endl;
  int numPlayers;
  while (true)
  {
    if ((!(std::cin >> numPlayers)) || numPlayers <= 0)
    {
      if (std::cin.fail())
      {
        // Clear the fail flag
        std::cin.clear();
        // Remove the non-integer data from the stream
        string garbage;
        std::cin >> garbage;
      }
      std::cout << "Please enter a positive number" << std::endl;
    }
    else
    {
      // break out of the loop once we've read a valid number
      break;
    }
  }
  return numPlayers;
}

void setupPlayers(Game &g, int numPlayers)
{
  // Loop through each player
  for (int i = 0; i < numPlayers; ++i)
  {
    char choice;
    bool isAI;

    // Prompt user to choose AI or human player
    std::cout << "Is player " << i << " an AI? (y/n)\n";
    std::cin >> choice;

    // Validate user input
    while (choice != 'y' && choice != 'n')
    {
      std::cout << "Please enter y or n\n";
      std::cin >> choice;
    }

    // Set isAI based on user choice
    isAI = (choice == 'y');

    // Add player to the game
    g.addPlayer(isAI);
  }
}

// infinite loop

void setupGame(Game &g)
{
  std::cout << "How many cards should each player start with?\n";
  int n;
  while (true)
  {
    if ((!(std::cin >> n)) || n <= 0)
    {
      if (std::cin.fail())
      {
        // Clear the fail flag
        std::cin.clear();
        // Remove the non-integer data from the stream
        string garbage;
        std::cin >> garbage;
      }
      std::cout << "Please enter a positive number\n";
    }
    else
    {
      break;
    }
  }

  Card *d = g.deal(n);
  cout << "The initial discard is " << d->getRank() << " " << d->getSuit() << endl;
}

int main()
{
  Game g;
  if (!loadDeck(g))
  {
    return 1;
  }
  int numPlayers = getPlayerCount();
  setupPlayers(g, numPlayers);
  setupGame(g);
  int winner = g.runGame();
  if (winner != -1)
  {
    std::cout << "Player " << winner << " wins!\n";
  }
  else
  {
    std::cout << "The game is a draw!\n";
  }
  std::cout << "The most played suit was " << g.mostPlayedSuit() << endl;
  return 0;
}