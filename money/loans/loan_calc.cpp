#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>

using namespace std;

void Usage()
{
  cout << "Usage:" << endl;
  cout << "./loan -m     (finding months to pay off at given payment rate)" << endl;
  cout << "./loan -p     (finding payment per month given number of months)" << endl;
  exit(1);
}

bool Correct_Usage(int argc, char* argv[]);

int main(int argc, char* argv[])
{
  if(!Correct_Usage(argc, argv))
    Usage();
  int money_borrowed;
  double intrest_rate;
  double monthly_intrest_rate;
  int months;
  int payment;

  cout << "How much money are you borrowing? ";
  cin >> money_borrowed;
  while(1)
  {
    cout << "Whats the intrest rate? (express as decimal)  ";
    cin >> intrest_rate;
    if(intrest_rate < 1 && intrest_rate > 0)
      break;
    else cout << "Intrest rate must be between 0 and 1" << endl;
  }
  monthly_intrest_rate = intrest_rate / 12;
  string tmp = argv[1];
  if(tmp == "-m")
  {
    cout << "What do you want your payment per month to be? ";
    cin >> payment;
    int months = 0;
    double total_paid = 0;
    double money_left = money_borrowed;
    while(money_left > payment)
    {
      money_left -= payment;
      money_left += (money_left * monthly_intrest_rate);
      total_paid += payment;
      months++;
      cout << "After " << months << " payments (months) of " << payment << ", you still owe " << setprecision(2) << fixed << money_left << endl;
    }
    total_paid += money_left;
    cout << endl;
    cout << "This loan of " << money_borrowed << " will take " << months << " months to pay off when making equal payments of " << payment << endl;
    cout << "Your last payment will be " << setprecision(2) << fixed << money_left << endl;
    cout << "You end up paying " << setprecision(2) << fixed << total_paid << endl;

    double bank_money_made = total_paid - money_borrowed;
    double bank_percent_made = 100 * (bank_money_made / money_borrowed);
    cout << "The bank makes " << bank_money_made << ", or %" << bank_percent_made << ", over " << months << " months." << endl;
    double bank_percent_made_per_month = bank_percent_made / months;
    cout << "(Thats %" << bank_percent_made_per_month << " per month.)" << endl;
  }
  else //tmp == "-p"
  {
    cout << "How many months do you want it to take to pay off your loan of " << money_borrowed << "? ";
    cin >> months;
    cout << endl << endl << "Sorry, this is not implemented yet." << endl;
  }
  return 0;
}

bool Correct_Usage(int argc, char* argv[])
{
  if(argc != 2)
    return false;
  string tmp = argv[1];
  if(tmp != "-m" && tmp != "-p")
    return false;
  return true;
}
