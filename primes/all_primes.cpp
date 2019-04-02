#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

bool is_prime(unsigned long long int number, vector<unsigned long long int> &prime_numbers, unsigned int size);

int output_counter = 10000;

int main()
{
  bool first_time = false;
  vector<unsigned long long int> prime_numbers;

  ifstream input_primes;
  input_primes.open ("list_of_primes.data");

  int input_num = 0;

  if(input_primes.is_open())
  {
    cout << "Opened the input file." << endl;
    unsigned long long int number;
    while(input_primes >> number)
    {
/*      if(number < 0)
      {
        cout << "You have found all prime unsigned long long ints" << endl;
        cout << "Now you need to increase the number of bits to find higher numbers." << endl;
        return 0;
      }
*/
      prime_numbers.push_back(number);
      input_num++;
      if((input_num % 50000000) == 0)
        cout << "Got 50000000 more: " << input_num << endl;
    }
    cout << "Got all the primes from the file." << endl;
    input_primes.close();
    cout << "There were " << prime_numbers.size() << " prime numbers already calculated" << endl;
    cout << "The largest prime we have found is " << prime_numbers.back() << endl;
  }
  else //input_primes did not open
  {
    prime_numbers.push_back(2);
    prime_numbers.push_back(3);
    first_time = true;          //added 2 and 3 to vector but not the file
    cout << "This seems to be the first time." << endl;
  }

  ofstream primes;
  primes.open ("list_of_primes.data", ios::out | ios::app); //WE ALWAYS WANT TO OPEN THE FILE

  if(first_time)                //add 2 and 3 to the file first because we will skip them in the for loop
  {
    primes << "2" << endl;
    primes << "3" << endl;
    cout << "Added the essentials" << endl;
  }

  cout << "Calculating more primes." << endl;

  for(unsigned long long int i = (prime_numbers.back() + 2); i > 0; i+=2)
  // all prime numbers after 2 are odd,
  // the last element in the vector will always be odd
  // no need to check that last element that we got, check the next odd number
  {
    if(is_prime(i, prime_numbers, prime_numbers.size()))
    {
      prime_numbers.push_back(i);
      primes << i << endl;
/*
      if((prime_numbers.size() % output_counter) == 0)
      {
        cout << output_counter << " more: " << prime_numbers.size() << " total. latest = " << prime_numbers.back() << endl;
      }
*/
    }
  }
  primes.close();
  return 0;
}

bool is_prime(unsigned long long int number, vector<unsigned long long int> &prime_numbers, unsigned int size)
{
  for(unsigned long long int i = 0; i < size && ((prime_numbers[i] * prime_numbers[i]) <= number); i++)
  {
    if((number % prime_numbers[i]) == 0)
    {
        return false;
    }
  }
  return true;
}
