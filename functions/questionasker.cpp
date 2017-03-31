#include "questionasker.h"

int QuestionAsker::aquire_int(string prompt)
{
  string tmp_int_str;
  cout << prompt;
  cin >> tmp_int_str;
  while(!valid_int(tmp_int_str))
  {
    cout << endl << "Bad int" << endl << endl;
    cin.clear();
    cin.ignore();
    cout << prompt;
    cin >> tmp_int_str;
  }
  return string_to_int(tmp_int_str);
}

double QuestionAsker::aquire_double(string prompt)
{
  string tmp_double_str;
  cout << prompt;
  cin >> tmp_double_str;
  while(!valid_double(tmp_double_str))
  {
    cout << endl << "Bad double" << endl << endl;
    cin.clear();
    cin.ignore();
    cout << prompt;
    cin >> tmp_double_str;
  }
  return string_to_double(tmp_double_str);
}

char QuestionAsker::aquire_char(string prompt)
{
  string tmp;
  cout << prompt;
  cin >> tmp;
  while(tmp.size() > 1)
  {
    cout << endl << "Bad input, looking for single character.";
    cout << endl << endl;
    cin.clear();
    cin.ignore();
    cout << prompt;
    cin >> tmp;
  }
  return tmp[0];
}

string QuestionAsker::aquire_string(string prompt)
{
  string tmp;
  cout << prompt;
  cin.ignore();
  getline(cin, tmp);
  cin.unget();
  return tmp;
}

bool QuestionAsker::valid_int(string tmp)
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

bool QuestionAsker::valid_double(string tmp)
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
int QuestionAsker::string_to_int(string tmp)
{
  bool negative = false;
  int value = 0;
  for(unsigned int i = 0; i < tmp.size(); i++)
  {
    if(i == 0 && tmp[i] == '-')
      negative = true;
    else
    {
      int digit = char_to_int(tmp[i]);
      value += digit * this->pow(10, (tmp.size() - i) - 1);
    }
  }

  if(negative)
    value = -value;

  return value;
}

//need to be sure that tmp is a good double
double QuestionAsker::string_to_double(string tmp)
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
      int digit = char_to_int(tmp[i]);
      whole_number += digit * this->pow(10, (decimal_index - i) - 1);
    }

    //make a whole number and then put it behind a decimal
    double decimal_number = 0;
    for(unsigned int i = decimal_index + 1; i < tmp.size(); i++)
    {
      int digit = char_to_int(tmp[i]);
      decimal_number += digit * this->pow(10, (tmp.size() - i) - 1);
    }
    decimal_number *= 
        this->ne_pow(10, ((decimal_index + 1) - tmp.size()));

    value = whole_number + decimal_number;
  }
  else
    value = double(this->string_to_int(tmp));

  if(negative)
    value = -value;

  return value;
}

int QuestionAsker::char_to_int(char tmp)
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

long QuestionAsker::pow(int base, int exp)
{
  if(exp < 0)
  {
    cout << "pow(base, exp) only accepts positive exponents.";
    cout << " Use ne_pow(base, exp) for negative exponents." << endl;
    return 0;
  }

  long value = 1;
  for(int i = 0; i < exp; i++)
    value *= base;

  return value;
}

double QuestionAsker::ne_pow(int base, int exp)
{
  double value = 1.0 / this->pow(base, -exp);
  return value;
}
