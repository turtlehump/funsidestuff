#include "sortinghat.h"
#include <iostream> 
#include <stdlib.h> 
using namespace std;

void Usage()
{
  cout << "Usage:" << endl;
  cout << "hamspam -t                                                                 (for training)" << endl;
  cout << "hamspam -c (\"<string to catagorize>\" || -f <filename>) -s <training_file>  (for checking)" << endl;
  exit(0);
}

bool Correct_usage(int argc, char* argv[]);


//**************************************************************************************************
int main(int argc, char* argv[])
{
  Sortinghat sorter;

  bool should_train;
  should_train = Correct_usage(argc, argv);

  if(should_train)
  {
    sorter.train();
    sorter.display();
  }
  else
  {
    if(argc == 5)
    {
      sorter.train(argv[4]);
      string group = argv[2];
      string catagory = sorter.find_catagory(group);
      cout << endl << "\"" << group << "\" is most likely " << catagory << endl;
    }
    if(argc == 6)
    {
      sorter.train(argv[5]);

      ifstream input(argv[3], ios::in);
      string group;
      map<string, int> score;
      while(getline(input, group))
      {
        string catagory = sorter.find_catagory(group);
        cout << endl;
        score[catagory]++;
      }

      cout << "***********************************************" << endl;
      cout << "TOTALS" << endl;
      map<string, int>::iterator it;
      for(it = score.begin(); it != score.end(); it++)
      {
        cout << it->first << ": " << it->second << endl;
      }
    }
  }

  return 0;
}
//**************************************************************************************************

//retruns true for training
//retruns false for catagorizing
//Usage will exit if its not one of those ^
bool Correct_usage(int argc, char* argv[])
{
  if(argc < 2)
    Usage();
  string tmp1 = argv[1]; //it doesnt work when I compare argv[i] == "-t"
  if(tmp1 == "-t")
  {
    if(argc != 2)
      Usage();
    return true;
  }
  else if(tmp1 == "-c")
  {
    if(argc != 5 && argc != 6)
    {
      Usage();
    }
    else
    {
      string tmp2 = argv[2];
      string tmp3 = argv[3];
      string tmp4 = argv[4];
      if(tmp2 == "-f")
      {
        if(argc != 6)
          Usage();
        cout << "Getting file as comparison input: " << tmp3 << endl << endl;
        string tmp5 = argv[5];
        ifstream cat_input;
        cat_input.open(argv[3]);
        if(!cat_input)
        {
          cout << "Your input file for checking (" << argv[3] << ") was not valid" << endl;
          Usage();
        }
        else
          cat_input.close();
        if(tmp4 != "-s")
          Usage();
        ifstream seed_input;
        seed_input.open(argv[5]);
        if(!seed_input)
        {
          cout << "Your input file for seeding (" << argv[5]<< ") was not valid" << endl;
          Usage();
        }
        seed_input.close();
      }
      else
      {
        if(argc != 5)
          Usage();
        if(tmp3 != "-s")
          Usage();
        ifstream seed_input;
        seed_input.open(argv[4]);
        if(!seed_input)
        {
          cout << "Your input file for seeding (" << argv[4] << ") was not valid" << endl;
          Usage();
        }
        seed_input.close();
      }
    }
    return false;
  }
  else
  {
    Usage();
  }
  exit(1);
}
