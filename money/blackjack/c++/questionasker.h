#ifndef QUESTIONASKER_H
#define QUESTIONASKER_H
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

class QuestionAsker
{
  public:
    QuestionAsker() {}
    ~QuestionAsker() {}

    int    aquire_int(string prompt);
    double aquire_double(string prompt);
    char   aquire_char(string prompt);
    string aquire_string(string prompt);

  private:
    bool   valid_int(string tmp);
    bool   valid_double(string tmp);
    int    string_to_int(string tmp);
    double string_to_double(string tmp);
    int    char_to_int(char tmp);
    long   pow(int base, int exp);
    double ne_pow(int base, int exp);
};

#endif
