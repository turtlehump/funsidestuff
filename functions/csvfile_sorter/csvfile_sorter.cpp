#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

void Usage()
{
  cout << "Usage:\tcsvfile_sorter <input_file> <output_file>" << endl
  exit(1);
}

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    cerr << "Error: invalid number of command line arguments.\n";
    Usage();
    exit(1);
  }

  ifstream input(argv[2], ios::in);
  if(!input)
  {
    cerr << "Error: could not open input file <" << argv[2] << ">.\n";
    exit(1);
  }

  ofstream output(argv[3], ios::out);
  if(!output)
  {
    cerr << "Error: could not open output file <" << argv[3] << ">.\n";
    exit(1);
  }

  string line;
  stringstream line_stream;

  while(input >> line)
  {
    line_stream.str(line);
    string tmp;
    int column = 0;
    while(line_stream >> tmp)
    {
      //you are now marching through an individual row and "column" is your index
      column++;
    }
  }
  return 0;
}
