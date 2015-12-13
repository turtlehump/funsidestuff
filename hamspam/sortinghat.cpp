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
    bool bad_input;
    do{
      bad_input = false;
      cout << "Give me a file for " << catagory << ": ";
      cin >> filename;
      input.open(filename.c_str());
      if(!input)
      {
        cout << "Bad file name" << endl;
        bad_input = true;
      }
      input.close();
    }while(bad_input);
    cout << "Mapping file " << filename << " to catagory " << catagory << endl;
    train_file(catagory, filename);

    cout << "Do you have more training? (Y/N) ";
    cin >> again;
  }while( again[0] == 'Y' || again[0] == 'y');
  save();
}


void Sortinghat::train_file(string catagory, string filename)
{
  ifstream input(filename.c_str(), ios::in);
  string line;
  while(getline(input, line))
  {
    line = good_line(line);
    train_group(catagory, line);
  }
  input.close();
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
  m_groupings_in_catagory[catagory]++;
  return;
}


void Sortinghat::train(char* filename)
{
  ifstream input(filename, ios::in);
  string line;
  string catagory;
  string word;
  int count;
  while(getline(input, line))
  {
    stringstream group;
    group << line;
    if(line[0] != '*')
    {
      group >> catagory;
      group >> word;
      group >> count;
      m_catagories[catagory][word] = count;
      m_all_words[word] += count;
    }
    else
    {
      char tmp;
      group >> tmp; //consume the *
      group >> count;
      m_groupings_in_catagory[catagory] = count; //catagories will still have old value
    }
  }
  input.close();
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
      output << cat_it->first << " " << word_it->first << " " << word_it->second << endl;
    }
    output << "* " << m_groupings_in_catagory[cat_it->first] << endl;
  }
  output.close();
}


void Sortinghat::display()
{
  map< string, map< string, int> >::iterator cat_it;
  for(cat_it = m_catagories.begin(); cat_it != m_catagories.end(); cat_it++)
  {
    map<string, int>::iterator word_it;
    for(word_it = cat_it->second.begin(); word_it != cat_it->second.end(); word_it++)
    {
      cout << cat_it->first << " : " << word_it->first << " : " << word_it->second << endl;
    }
    cout << "* " << m_groupings_in_catagory[cat_it->first] << endl;
  }
}

string Sortinghat::find_catagory(string group)
{
  double max = 0, total_prob = 0;
  int max_index, i = 0;
  vector<double> probs;

  group = good_line(group);

  double prob_group = prob_of_group(group); 

  map<string, map<string, int> >::iterator cat_it;
  for(cat_it = m_catagories.begin(); cat_it != m_catagories.end(); cat_it++)
  {
    double group_given_cat = group_given_catagory(group, cat_it->first);
    double prob_cat = prob_of_catagory(cat_it->first);

    double cat_given_group = (group_given_cat * prob_cat) / prob_group;

    probs.push_back(cat_given_group);
    total_prob += cat_given_group;
    if(cat_given_group > max)
    {
      max = cat_given_group;
      max_index = i;
    }
    i++;
  }

  string catagory;
  cat_it = m_catagories.begin();
  cout << "Group: " << group << endl;
  for(unsigned int i = 0; i < probs.size(); i++)
  {
    cout << "Catagory: " << cat_it->first << "   ";
    cout << "Probability: " << probs[i] / total_prob; 
    if( i == max_index)
    {
       catagory = cat_it->first;
       cout << "  ***MAX***";
    }
    cout << endl;
    cat_it++;
  }
  return catagory;
}

double Sortinghat::prob_of_group(string group)
{
  double total = 0;
  map<string, map<string, int> >::iterator it;
  for(it = m_catagories.begin(); it != m_catagories.end(); it++)
  {
    total += group_given_catagory(group, it->first);
  }
  return total;
}

double Sortinghat::group_given_catagory(string group, string catagory)
{
  int catagory_total_word_count = 0;
  map<string, int>::iterator it;
  for(it = m_catagories[catagory].begin(); it != m_catagories[catagory].end(); it++)
  {
    catagory_total_word_count += it->second;
  }
  stringstream message;
  message << group;
  double group_prob_running_total = 1; //init to one because we are multiplying
  string word;
  while(message >> word)
  {
    group_prob_running_total *= (1.0 * m_catagories[catagory][word]) / catagory_total_word_count;
  }
  return group_prob_running_total;
}

double Sortinghat::prob_of_catagory(string catagory)
{
  int total_groupings = 0;
  map<string, int>::iterator it;
  for(it = m_groupings_in_catagory.begin(); it != m_groupings_in_catagory.end(); it++)
  {
    total_groupings += it->second;
  }
  return (1.0 * m_groupings_in_catagory[catagory]) / total_groupings;
}

string Sortinghat::good_line(string line)
{
  for(unsigned int i = 0; i < line.size(); i++)
  {
    if(!isalpha(line[i]) && !isspace(line[i]))
    {
      string tmp1 = line.substr(0, i);
      string tmp2 = line.substr(i + 1);
      line = tmp1 + tmp2;
      i--;
    }
    if(isalpha(line[i]))
      line[i] = tolower(line[i]);
  }
  return line;
}
