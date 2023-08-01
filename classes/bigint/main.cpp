#include <iostream>
#include "bigint.h"

using namespace std;

int main()
{
  int base = 7;
  int power = 18;

  BigInt* bigint = new BigInt(base);;

  bigint->print();
  cout << endl << " **** " << endl;

  cout << "1: ";
  bigint->print();
  cout << endl;

  int i;
  for(i = 2; i <= power; i++)
  {
    bigint->multiply(base);
    cout << i << ": ";
    bigint->print();
    cout << endl;
  }

  cout << "********" << endl;
  cout << base << " to the " << power << " power is: ";
  bigint->print();
  cout << endl;

  cout << "********" << endl;

  bigint->add(91000);
  cout << "Adding 91000" << endl << endl;
  bigint->print();
  cout <<  endl << endl;
  cout << " - magnitude " << bigint->magnitude() << endl;
  cout << endl;

  return 0;
}
