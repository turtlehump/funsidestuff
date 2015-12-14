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
#include <locale>
using namespace std;

class Sortinghat
{
  public:
    Sortinghat();
    //initial training, asks for catagories and files.
    //creates a trained.data file that you can rename and save for later
    void train(); 

    //use before catagorize()
    //pass this the trained.data file from the train()
    void train(char* filename);

    void train_group(string catagory, string line);
    void set_laplace(int laplace);

    string find_catagory(string group);

    void display();

  private:
    
    //private function used by train()
    void train_file(string catagory, string filename);
    
    //creates a trained.data file that you can rename and save for later
    void save();

    //math member functions
    double prob_of_group(string group);
    double group_given_catagory(string group, string catagory);
    double prob_of_catagory(string group);

    string good_line(string line);

    map< string, map< string, int > > m_catagories;
    map< string, int > m_all_words;
    map< string, int > m_groupings_in_catagory;

    int laplace;
};
#endif
