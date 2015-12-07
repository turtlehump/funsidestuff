#include "matrix.h"

Matrix::Matrix(int height, int width)
{
  m_height = height;
  m_width = width;
  m_max_values = height * width;
  for(int i = 0; i < height; i++)
  {
    vector<double> newrow;
    for(int j = 0; j < width; j++)
      newrow.push_back(0);
    m_values.push_back(newrow);
  }
}

//new is called in this. delete must be called on whatever this is assigned to
Matrix* Matrix::multiply(Matrix* other)
{
  if(this->m_width != other->m_height)
  {
    cout << "Something went wrong in multiplying matricies\n";
    cout << "this->m_width = " << this->m_width << endl;
    cout << "other->m_height = " << other->m_height << endl;
    exit(1);
  }
  cout << "they can multiply!\n";
  Matrix* result = new Matrix(this->m_height, other->m_width);
  for(int i = 0; i < this->m_height; i++)
  {
    for(int j = 0; j < other->m_width; j++)
    {
      for(int k = 0; k < other->m_height; k++)
      {
        result->m_values[i][j] += this->m_values[i][k] * other->m_values[k][j];
      }
    }
  }
  return result;
}

void Matrix::display()
{
  vector< vector<double> >::iterator mainIT;
  for(mainIT = m_values.begin(); mainIT != m_values.end(); mainIT++)
  {
    vector<double>::iterator it;
    for(it = mainIT->begin(); it != mainIT->end(); it++)
    {
      cout << *it << " ";
    }
    cout << endl;
  }
  return;
}

bool Matrix::set(double value, unsigned int row, unsigned int column)
{
  //size() returns one more that the last valid value, so if the row == size() then there is an error
  if(row >= m_values.size())
    return false;
  if(column >= m_values[row].size())
    return false;
  m_values[row][column] = value;
  return true;
}
