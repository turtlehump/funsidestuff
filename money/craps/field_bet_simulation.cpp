#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include "questionasker.h"

using namespace std;

QuestionAsker* qa = new QuestionAsker();

bool QUIET = false;	//if false, display all betstreams
			//if true, display only loop end results

void Usage()
{
  cout << "Usage:" << endl;
  cout << "field_bet_simulation <option>" << endl;
  cout << endl;
  cout << "   -d: use hardcoded values" << endl;
  cout << "   -q: quiet mode (no bet stream display)" << endl;
  exit(1);
}

void setup(int argc, char *argv[]);
void get_simulation_key_values();

int simulation();

//***** KEY VALUES FOR SIMULATION *****
int LOOPS = 1;
int STARTING_BET = 10;
int TABLE_MAX = 2000;
int STARTING_CASH = 1000;
int GOAL_WINNINGS = 500;
//***** KEY VALUES FOR SIMULATION *****

int main(int argc, char *argv[])
{
  srand(time(NULL));

  int winners = 0;
  int losers = 0;

  setup(argc, argv);

  for(int i = 0; i < LOOPS; i++)
  {
    int loop_winnings = simulation();
    //(QUIET) ? cout << "LOOP " << i << ": " :; //the quiet output
    if(loop_winnings > 0)
    {
      //(QUIET) ? cout << "Winner! (+$" << loop_winnings << ")" :;
      winners++;
    }
    else
    {
      //(QUIET) ? cout << "Loser..." :;
      losers++;
    }
    cout << endl;
  }

  cout << endl << endl;
  cout << "Out of " << LOOPS << " loops:" << endl;
  cout << "\t*" << winners << " won at least " << GOAL_WINNINGS << endl;
  cout << "\t*" << losers << " went to 0" << endl;

  return 0;
}

void setup(int argc, char *argv[])
{
  if(argc == 1)
    get_simulation_key_values();

  else
  {
    bool values_set = false;
    for(int i = 1; i < argc; i++)
    {
      if(!strcmp(argv[i], "-d"))
      {
        cout << "****Using Default Values****" << endl;
        cout << "Starting Bet: " << STARTING_BET << endl;
        cout << "Table Max: " << TABLE_MAX << endl;
        cout << "Starting Money: " << STARTING_CASH << endl;
        cout << "Goal Winnings: " << GOAL_WINNINGS << endl;
        values_set = true;
      }
      if(!strcmp(argv[i], "-q"))
      {
        cout << "Quite mode" << endl;
        QUIET = true;
      }
      if(!(!strcmp(argv[i], "-d") || !strcmp(argv[i], "-q")))
        Usage();
    }
    if(!values_set)
      get_simulation_key_values();
    else
      LOOPS = qa->aquire_int("How many times do you want to run this simulation? ");
  }
  return;
}

void get_simulation_key_values()
{
  do
  {
    STARTING_BET = qa->aquire_int("Starting Bet: ");
    if(STARTING_BET < 1)
      cout << "You have to have a positive bet.";
  }while(STARTING_BET < 1);

  do
  {
    TABLE_MAX = qa->aquire_int("Table Max: ");
    if(TABLE_MAX < STARTING_BET)
      cout << "The table max has to be above the starting bet.";
  }while(TABLE_MAX < STARTING_BET);

  do
  {
    STARTING_CASH = qa->aquire_int("Starting Bankroll: ");
    if(STARTING_CASH < STARTING_BET)
      cout << "You have to be able to cover your starting bet at least once.";
  }while(STARTING_CASH < STARTING_BET);

  do
  {
    GOAL_WINNINGS = qa->aquire_int("Goal Winnings: ");
    if(GOAL_WINNINGS < 1)
      cout << "CMON! You want to have some winnings...";
  }while(GOAL_WINNINGS < 1);

  do
  {
    LOOPS = qa->aquire_int("How many times do you want to run this simulation? ");
    if(LOOPS < 1)
      cout << "I have to run more than 0 loops.";
  }while(LOOPS < 1);

  return;
}

//IN PROGRESS: IMPLEMENTING QUIET MODE
int simulation()
{
  int roll, dice1, dice2, cash_at_start_of_betting_seq = STARTING_CASH;
  int segment_rolls = 0, total_rolls = 0, losses = 0;
  bool maxed = false;
  bool just_won;
  int current_cash = STARTING_CASH;
  int goal_cash = STARTING_CASH + GOAL_WINNINGS;

  int bet = STARTING_BET;
  //10, 20, 40,  80, 160, 320,  640, 1280, 2560,  5120, 10240   <- Bets
  //10, 30, 70, 150, 310, 630, 1270, 2550, 5110, 10230, 20470   <- Running total
  //20, 40, 80, 160, 320, 640, 1280, 2560, 5120, 10240, 20480   <- winnings on bet

  ostringstream betstream;
  while(current_cash < goal_cash && current_cash > 0)
  {
    if(just_won)
    {
      cash_at_start_of_betting_seq = current_cash;
    }

    current_cash -= bet;        //place your bet

    dice1 = (rand() % 6) + 1;
    dice2 = (rand() % 6) + 1;
    roll = dice1 + dice2;      //roll
    segment_rolls++;
    total_rolls++;

    if(segment_rolls == 0) betstream << bet;        //record the bet
    else 
    {
      betstream << ", ";
      if(current_cash == 0)  betstream << "ALL IN -> ";
      betstream << bet;
    }

    betstream << ":" << roll;

    if(roll >= 5 && roll <= 8) //lost
    {
      if(current_cash == 0) //we lost and have no more money
      {
        //(!QUIET) ? cout << betstream.str() << endl :;
        losses++;
        break;
      }

      if(maxed)
      {
        betstream << " *Lost maxed-out bet*\n";
        bet = STARTING_BET;      //reset the bet
        maxed = false;
      }
      else
        bet = bet * 2;           //double bet

      if(bet > TABLE_MAX)
      {
        bet = TABLE_MAX;
        betstream << " *Couldnt double our bet (table max reached)*";
        maxed = true;  //this flag shows that we have already made one bet in the current stream at our curent max value we could put on the table, we will restart the betting sequence next iteration
      }

      if(bet >= current_cash)
      {
        bet = current_cash;
        betstream << " *Couldnt double our bet (not enough money)*";
        maxed = true;  //this flag shows that we have already made one bet in the current stream at our curent max value we could put on the table, we will restart the betting sequence next iteration
      }
      just_won = false;
    }
    else //win
    {
      betstream << "\t";
      if(roll == 3 || roll == 4 || (roll >= 9 && roll <= 11))
        current_cash += bet * 2; //wins 1:1, we already put the money down and took it out of current_cash so double the bet and give it back
      else if(roll == 2)
        current_cash += bet * 3; //wins 1:2, tripple the bet and give it back
      else if(roll == 12)
        current_cash += bet * 4; //wins 1:3, quadrouple the bet and give it back

      int diff_in_money = current_cash - cash_at_start_of_betting_seq;
      if(diff_in_money >= 0)
        betstream << "+";
      else losses++;

      betstream << diff_in_money << " in " << segment_rolls;
      if(roll == 2)  betstream << ", got double 1's";
      if(roll == 12) betstream << ", got double 6's";
      betstream << endl;

      //(!QUIET) ? cout << betstream.str() : ; //the non quiet output
      betstream.str("");    //clears out the betstream
      segment_rolls = 0;
      bet = STARTING_BET;
      maxed = false;        //we won this stream
      just_won = true;      //we won this stream
    }
  }

  int winnings = current_cash - STARTING_CASH;

  //if(!QUIET)
  //{
    if(winnings > 0)
    {
      cout << endl << "WINNER" << endl;
      cout << "It took you " << total_rolls << " rolls ";
      cout << "to make " << winnings;
      cout << ", and there were " << losses << " losses" << endl;
    }
    else
    {
      cout << endl << "LOSER" << endl;
      cout << "You are broke, ";
      cout << "it happened in " << total_rolls << " rolls";
      cout << ", and there were " << losses << " losses" << endl;
    }
  //}
  return winnings;
}
