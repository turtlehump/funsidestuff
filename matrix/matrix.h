#ifndef MATRIX_H
#define MATRIX_H

#include<iostream>
#include<stdlib.h>
#include<vector>

using namespace std;

class Matrix
{
  public:
    Matrix(int height, int width);
    Matrix* multiply(Matrix* other);
    void display();
    bool set(double value, unsigned int row, unsigned int column);

  private:
    int m_height, m_width, m_max_values;
    vector< vector<double> > m_values;
};

#endif
