#include <iostream>
#include "bigint.h"

using namespace std;

int main()
{
  int base = 7;
  int power = 18;

  BigInt* bigint1 = new BigInt(base);;

  bigint1->print();
  cout << endl << " **** " << endl;

  cout << "1: ";
  bigint1->print();
  cout << endl;

  int i;
  for(i = 2; i <= power; i++)
  {
    bigint1->multiply(base);
    cout << i << ": ";
    bigint1->print();
    cout << endl;
  }

  cout << "********" << endl;
  cout << base << " to the " << power << " power is: ";
  bigint1->print();
  cout << endl;

  cout << "********" << endl;

  cout << "Adding (int) 91000" << endl << endl;
  bigint1->add(91000);
  bigint1->print();
  cout <<  endl << endl;

  cout << "********" << endl;

  cout << "Making new BigInt" << endl << endl;

  BigInt* bigint2 = new BigInt(0);;
  cout << "bigint2: ";
  bigint2->print();
  cout <<  endl << endl;

  cout << "Setting to 551" << endl << endl;
  bigint2->set(551);
  cout << "bigint2: ";
  bigint2->print();
  cout <<  endl << endl;

  cout << "Adding bigint2 to bigint1" << endl << endl;
  bigint1->add(bigint2);
  cout << "bigint1: ";
  bigint1->print();
  cout <<  endl << endl;

  cout << "Setting bigint2 to bigint1" << endl << endl;
  bigint2->set(bigint1);
  cout << "bigint2: ";
  bigint2->print();
  cout <<  endl << endl;

  cout << "********" << endl;

  cout << " - magnitude of bigint1: " << bigint1->magnitude() << endl;
  cout << " - magnitude of bigint2: " << bigint2->magnitude() << endl;
  cout << endl;

  return 0;
}
