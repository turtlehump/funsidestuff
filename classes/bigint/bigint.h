#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <array>
#include <string>
using namespace std;

const int BIGINT_MAXDIGITS = 100;

class BigInt
{
  public:
    BigInt();
    BigInt(int tmp);
    BigInt(string tmp);
    BigInt(BigInt* tmp);

    void set(int tmp);
    void set(BigInt* tmp);

    void   print();
    string str_value();

    int magnitude();
    int get_digit(int i);

    void add(int addition);
    void add(BigInt* adition);

    void multiply(int multiplier);
    //void multiply(BigInt* multiplier);

    bool equal_to(int operand);
    bool equal_to(BigInt* operand);

    bool greater_than(int operand);
    bool greater_than(BigInt* operand);

    bool less_than(int operand);
    bool less_than(BigInt* operand);

  private:
    std::array<int, BIGINT_MAXDIGITS> m_int = {0};
};

#endif
