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

  table->set_deck();
  table->get_players();
  table->play_blackjack();

  delete table;
  return 0;
}
