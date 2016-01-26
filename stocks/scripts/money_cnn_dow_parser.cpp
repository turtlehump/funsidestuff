//DOW Parser
//http://money.cnn.com/data/dow30/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdlib.h>
using namespace std;

const string DOW_stocks[] = {"AAPL", "AXP", "BA", "CAT", "CSCO", "CVX", "DD", "DIS","GE", "GS", "HD", "IBM", "INTC", "JNJ", "JPM", "KO", "MCD", "MMM", "MRK", "MSFT", "NIKE", "PFE", "PG", "TRV", "UNH", "UTX", "V", "VZ", "WMT", "XOM"};

string chomp(string str);
string get_stock_nickname(string line);
double get_stock_price(string line);

int main()
{
  ifstream web_source;
  web_source.open("../DOW/dow.curdata");
  if(!web_source)
  {
    cout << "Couldnt open input file :(" << endl;
    return 0;
  }

  map<string, double> current_values;

  string line;
  int table_rows = 0;
  while(getline(web_source, line))
  {
    line = chomp(line);
    if(line == "<tr>")
    //this website (http://money.cnn.com/data/dow30/) goes line by line after a <tr>
    {
      table_rows++;
      if(table_rows > 1)
      {
        getline(web_source, line);
        line = chomp(line);
        string stock_nickname = get_stock_nickname(line);

        getline(web_source, line);
        line = chomp(line);
        double stock_price = get_stock_price(line);

        current_values[stock_nickname] = stock_price;
      }
    }
  }

  map<string, double>::iterator it;
  for(it = current_values.begin(); it != current_values.end(); it++)
  {
    cout << it->first << " " << it->second << endl;
  }

  return 0;
}

//eliminates whitespace
string chomp(string str)
{
  string new_string = "";
  for(int i = 0; i < str.length(); i++)
  {
    if( !(str[i] == ' ' || str[i] == '\t' || str[i] == '\n') )
      new_string += str[i];
  }
  return new_string;
}

string get_stock_nickname(string line)
{
  for(int i = 0; i < line.length(); i++)
  {
    if(line[i] == '>' && line[i+1] != '<')
    {
      string nickname = "";
      i++;
      do
      {
        nickname += line[i];
        i++;
      }while(line[i] != '<');
      return nickname;
    }
  }   
}

double get_stock_price(string line)
{
  for(int i = 0; i < line.length(); i++)
  {
    if(line[i] == '>' && line[i+1] != '<')
    {
      string price = "";
      i++;
      do
      {
        price += line[i];
        i++;
      }while(line[i] != '<');
      return strtod(price.c_str(), NULL);
    }
  }   
}
