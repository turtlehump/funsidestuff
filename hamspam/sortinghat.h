//sortinghat.h
//Philip Vierhus
//pvierhus

#ifndef SORTINGHAT_H
#define SORTINGHAT_H

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
using namespace std;

class Sortinghat
{
  public:
    Sortinghat();
    void train();
    void train_group(string catagory, string line);
    
    vector<double> find_catagory(string group);

  private:
    void train_file(string catagory, string filename);
    void save();
    map<string, map< string, int > > m_catagories;
    map<string, int > m_all_words;
    map<string, string > m_exact_groupings;

};
#endif
