#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

bool    good_int(string tmp);
bool    good_double(string tmp);
int     s_to_i(string tmp);
double  s_to_d(string tmp);
int     c_to_i(char tmp);
int     pow(int base, int exp);
double  ne_pow(int base, int exp);

bool good_int(string tmp)
{
  for(unsigned int i = 0; i < tmp.size(); i++)
  {
    if(i == 0 && tmp[i] == '-')
      continue;
    if(!isdigit(tmp[i]))
      return false;
  }

  return true;
}

bool good_double(string tmp)
{
  bool decimal = false;
  for(unsigned int i = 0; i < tmp.size(); i++)
  {
    if(i == 0 && tmp[i] == '-')
      continue;
    if(!isdigit(tmp[i]))
    {
      if(tmp[i] == '.')
      {
        if(decimal)
          return false;
        else
          decimal = true;
      }
      else
       return false;
    }
  }

  return true;
}

//need to be sure that tmp is a good int
int s_to_i(string tmp)
{
  bool negative = false;
  int value = 0;
  for(unsigned int i = 0; i < tmp.size(); i++)
  {
    if(i == 0 && tmp[i] == '-')
      negative = true;
    else
    {
      int digit = c_to_i(tmp[i]);
      value += digit * pow(10, (tmp.size() - i) - 1);
    }
  }

  if(negative)
    value = -value;

  return value;
}

//need to be sure that tmp is a good double
double s_to_d(string tmp)
{
  double value = 0;
  bool negative = false;
  bool decimal = false;
  int decimal_index;
  for(unsigned int i = 0; i < tmp.size(); i++)
  {
    if(i == 0 && tmp[i] == '-')
      negative = true;
    if(tmp[i] == '.')
    {
      decimal = true;
      decimal_index = i;
      break;
    }
  }

  if(decimal)
  {
    int whole_number = 0;
    for(unsigned int i = 0; i < decimal_index; i++)
    {
      int digit = c_to_i(tmp[i]);
      whole_number += digit * pow(10, (decimal_index - i) - 1);
    }

    double decimal_number = 0;
    for(unsigned int i = decimal_index + 1; i < tmp.size(); i++)
    {
      int digit = c_to_i(tmp[i]);
      decimal_number += digit * pow(10, (tmp.size() - i) - 1);
    }
    decimal_number *= ne_pow(10, ((decimal_index + 1) - tmp.size()));

    value = double(whole_number) + double(decimal_number);
  }
  else
    value = double(s_to_i(tmp));

  if(negative)
    value = -value;

  return value;
}

int c_to_i(char tmp)
{
  if(tmp == '0')
    return 0;
  if(tmp == '1')
    return 1;
  if(tmp == '2')
    return 2;
  if(tmp == '3')
    return 3;
  if(tmp == '4')
    return 4;
  if(tmp == '5')
    return 5;
  if(tmp == '6')
    return 6;
  if(tmp == '7')
    return 7;
  if(tmp == '8')
    return 8;
  if(tmp == '9')
    return 9;

  return 0;
}

int pow(int base, int exp)
{
  if(exp < 0)
  {
    cout << "pow(base, exp) only accepts positive exponents.";
    cout << " Use ne_pow(base, exp) for negative exponents." << endl;
    return 0;
  }

  int value = 1;
  for(int i = 0; i < exp; i++)
    value *= base;

  return value;
}

double ne_pow(int base, int exp)
{
  double value = 1.0 / pow(base, -exp);
  return value;
}

int main()
{
  string tmp_int_str;
  string tmp_double_str;

  cout << "Int checking: ";
  cin >> tmp_int_str;
  while(!good_int(tmp_int_str))
  {
    cout << endl << "Bad int" << endl << endl;
    cin.clear();
    cin.ignore();
    cout << "Int checking: ";
    cin >> tmp_int_str;
  }
  int tmp_int = s_to_i(tmp_int_str);

  cout << "Double checking: ";
  cin >> tmp_double_str;
  while(!good_double(tmp_double_str))
  {
    cout << endl << "Bad double" << endl << endl;
    cin.clear();
    cin.ignore();
    cout << "Double checking: ";
    cin >> tmp_double_str;
  }
  double tmp_double = s_to_d(tmp_double_str);

  cout << tmp_int << " * " << tmp_double;
  cout << " = " << tmp_int * tmp_double << endl << endl;

  //***************************************************

  int base, exponent;
  cout << "Powers:" << endl;

  cout << "Base: ";
  cin >> tmp_int_str;
  while(!good_int(tmp_int_str))
  {
    cout << endl << "Bad int" << endl << endl;
    cin.clear();
    cin.ignore();
    cout << "Base: ";
    cin >> tmp_int_str;
  }
  base = s_to_i(tmp_int_str);

  cout << "Exponent: ";
  cin >> tmp_int_str;
  while(!good_int(tmp_int_str))
  {
    cout << endl << "Bad int" << endl << endl;
    cin.clear();
    cin.ignore();
    cout << "Exponent: ";
    cin >> tmp_int_str;
  }
  exponent = s_to_i(tmp_int_str);

  cout << base << " ^ " << exponent << " = ";
  if(exponent < 0)
    cout << ne_pow(base, exponent);
  else
    cout << pow(base, exponent);
  cout << endl;

  return 0;
}
