#include "questionasker.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
  QuestionAsker* qa = new QuestionAsker();

  int tmp_int = qa->aquire_int("Give me an int: ");
  double tmp_double = qa->aquire_double("Give me a double: ");

  double total = tmp_int + tmp_double;
  cout << tmp_int << " + " << tmp_double << " = " << total << endl;

  delete qa;

  return 0;
}
