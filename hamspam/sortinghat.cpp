//Philip Vierhus
//pvierhus

#include "sortinghat.h"
using namespace std;

Sortinghat::Sortinghat(){}

void Sortinghat::train()
{
  string again;
  string catagory;
  string filename;
  ifstream input;
  do{
    cout << "Give me a catagory: ";
    cin >> catagory;
    bool bad_input = false;
    do{
      cout << "Give me a file for " << catagory << ": ";
      cin >> filename;
      input.open(filename.c_str());
      if(!input)
      {
        cout << "Bad file name";
        bad_input = true;
      }
    }while(bad_input);
    cout << "Mapping file " << filename << " to catagory " << catagory << endl;
    train_file(catagory, filename);
    input.close();

    cout << "Do you have more training? (Y/N) ";
    cin >> again;
  }while( again[0] == 'Y' || again[0] == 'y');
}


void Sortinghat::train_file(string catagory, string filename)
{
  ifstream input(filename.c_str(), ios::in);
  string line;
  while(getline(input, line))
  {
    //no_punc(line);
    //all_lower(line);
    train_group(catagory, line);
    m_exact_groupings[catagory] = line;
  }
}


void Sortinghat::train_group(string catagory, string line)
{
  stringstream group;
  group << line;
  string word;
  while(group >> word)
  {
    m_catagories[catagory][word]++;
    m_all_words[word]++;
  }
  return;
}

void Sortinghat::save()
{
  ofstream output("trained.data");
  map< string, map< string, int> >::iterator cat_it;
  for(cat_it = m_catagories.begin(); cat_it != m_catagories.end(); cat_it++)
  {
    map<string, int>::iterator word_it;
    for(word_it = cat_it->second.begin(); word_it != cat_it->second.end(); word_it++)
    {
      output << cat_it->first << ":" << word_it->first << ":" << word_it->second << endl;
      cout << cat_it->first << ":" << word_it->first << ":" << word_it->second << endl;
    }
  }
}

vector<double> Sortinghat::find_catagory(string group)
{
  vector<double> probs;
  probs.push_back(0.5);
  return probs;
}
