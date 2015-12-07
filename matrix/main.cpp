#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<ctype.h>
#include<sstream>
#include<string>
#include<stdlib.h>
#include<assert.h>
#include"matrix.h"

using namespace std;

int main()
{

  int x, y, n, m;
  double value;
  cout << "Enter in the dimentions for a Matrix:\n";
  cout << "Height: ";
  cin >> x ;
  cout << "Width: ";
  cin >> y; 
  Matrix* A = new Matrix(x, y);

  for(int i = 0; i < y; i++)
  {
    for(int j = 0; j < x; j++)
    {
      cout << "Value for (" << j << ", " << i << "): ";
      cin >> value;
      A->set(value, j, i);
      A->display();
    }
  }

  cout << "Enter in dimentions for another Matrix: \n";
  cout << "Height: ";
  cin >> m;
  cout << "Width: ";
  cin >> n; 
  Matrix* B = new Matrix(m, n);
  Matrix* C = A->multiply(B); //will throw and error and exit if the matrix cant be multiplied
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < m; j++)
    {
      cout << "Value for (" << j << ", " << i << "): ";
      cin >> value;
      B->set(value, j, i);
      B->display();
    }
  }

  C = A->multiply(B); //will throw and error and exit if the matrix cant be multiplied
  C->display();


  delete A;
  delete B;
  delete C;
  return 0;
}
