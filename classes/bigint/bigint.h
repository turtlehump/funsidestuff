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

    void set(int tmp);

    void   print();
    string str_value();

    int magnitude();

    void add(int addition);

    void multiply(int multiplier);
    //void multiply(BigInt multiplier);

  private:
    std::array<int, BIGINT_MAXDIGITS> m_int = {0};
};

#endif
