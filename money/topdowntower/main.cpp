#include <iostream>
#include "table.h"

using namespace std;

int main()
{
  Table* table = new Table();

  table->set_player();
  table->simulation();

  delete table;
  return 0;
}
