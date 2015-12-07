#include "sortinghat.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
  Sortinghat sorter;

  sorter.train();
  sorter.save();

  return 0;
}
