#include <iostream>
#include "player.h"
#include "table.h"

using namespace std;

int main()
{
  for(int i = 0; i < 70; i++)
    cout << endl;

  Table* table = new Table();
  int base_bet = 15;

  string name;
  cout << "Whats your name? ";
  cin >> name;

  if(name == "odds") table->get_odds();
  else
  {
    Player* player = new Player(name);

    table->play_game(player, base_bet);

    cout << endl << endl;
    player->print();
    cout << endl << endl;

    delete player;
  }

  cout << "Thanks for playing, " << name << endl;

  delete table;
  return 0;
}
