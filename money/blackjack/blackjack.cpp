#include "table.h"
#include "deck.h"
#include "player.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{
  Table* table = new Table();
  int num_decks, num_players;

  cout << "How many decks are we going to use? ";
  cin >> num_decks;
  Deck* deck = new Deck(num_decks * 52);
  cout << endl;

  cout << "How many players are at the table? ";
  cin >> num_players;
  cout << endl;
  for(int i = 0; i < num_players; i++)
  {
    string name;
    cout << "Player " << i << "'s name: ";
    cin.ignore();
    getline(cin, name);
    cin.unget();

    Player* new_player = new Player(name);
    table->add_player(new_player);
  }
  Player* dealer = new Player("Dealer");
  table->add_player(dealer);
  cout << endl << endl;

  deck->shuffle();
  deck->cut();

  table->set_deck(deck);

  table->starting_deal();

  table->print(true);

  delete deck;
  return 0;
}
