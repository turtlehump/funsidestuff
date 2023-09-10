#include <iostream>
#include "player.h"
#include "table.h"

using namespace std;

void clear_screen()
{
  for(int i = 0; i < 70; i++)
    cout << endl;
  return;
}

void play_game(Table* table, Player* player)
{
  string play_again;
  do
  {
    clear_screen();

    table->play_round(player);

    cout << "Do you want to play again? (y/n)" << endl;
    cin >> play_again;

    if     (play_again == "y" || play_again == "Y") continue;
    else if(play_again == "n" || play_again == "N") break;
    else
      cout << endl << "That is not a valid input." << endl << endl;

  }while(1);

  return;
}

int main()
{
  clear_screen();

  Table* table = new Table();

  string name;
  cout << "Whats your name? ";
  cin >> name;

  if(name == "odds") table->get_odds();
  else
  {
    Player* player = new Player(name);

    play_game(table, player);

    cout << endl << endl;
    player->print();
    cout << endl;

    delete player;
  }

  cout << "Thanks for playing, " << name << endl;

  delete table;
  return 0;
}
