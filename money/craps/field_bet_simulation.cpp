#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include "questionasker.h"

using namespace std;

QuestionAsker* qa = new QuestionAsker();

int STARTING_BET = 10;
int TABLE_MAX = 2000;
int STARTING_CASH = 5000;
int GOAL_WINNINGS = 500;

void Usage()
{
  cout << "Usage:" << endl;
  cout << "field_bet_simulation <option>" << endl;
  cout << endl;
  cout << "   -d - use hardcoded values" << endl;
  return;
}

void setup(int argc, char *argv[]);
void simulation();

int main(int argc, char *argv[])
{
  setup(argc, argv);
  simulation();

  return 0;
}

void setup(int argc, char *argv[])
{
  if(argc == 1)
  {
    STARTING_BET = qa->aquire_int("Starting Bet: ");
    TABLE_MAX = qa->aquire_int("Table Max: ");
    STARTING_CASH = qa->aquire_int("Starting Bankroll: ");
    GOAL_WINNINGS = qa->aquire_int("Goal Winnings: ");
    cout << endl << endl << endl;
  }
  else
  {
    if(!strcmp(argv[1], "-d"))
    {
      cout << "****Using Default Values****" << endl;
      cout << "Starting Bet: " << STARTING_BET << endl;
      cout << "Table Max: " << TABLE_MAX << endl;
      cout << "Starting Money: " << STARTING_CASH << endl;
      cout << "Goal Winnings: " << GOAL_WINNINGS << endl;
      cout << endl << endl << endl;
    }
    else Usage();
  }
  return;
}

void simulation()
{
  srand(time(NULL));
  int roll, dice1, dice2, cash_at_start_of_betting_seq = STARTING_CASH;
  int segment_rolls = 0, total_rolls = 0, losses = 0;
  bool maxed = false;
  bool just_won;
  int current_cash = STARTING_CASH;
  int goal_cash = STARTING_CASH + GOAL_WINNINGS;

  int bet = STARTING_BET;
  //10, 20, 40,  80, 160, 320,  640, 1280, 2560,  5120, 10240   <- Bets
  //10, 30, 70, 150, 310, 630, 1270, 2550, 5110, 10230, 20470   <- Running total
  //20, 40, 80, 160, 320, 640, 1280, 2560, 5120, 10240, 20480   <- winnings

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
    total_rolls++;

    if(segment_rolls == 0) betstream << bet;        //record the bet
    else 
    {
      betstream << ", ";
      if(current_cash == 0)  betstream << "ALL IN -> ";
      betstream << bet;
    }

    segment_rolls++;        //havent won our 10 yet

    betstream << ":" << roll;

    if(roll >= 5 && roll <= 8) //lost
    {
      if(current_cash == 0) //we lost and have no more money
      {
        cout << betstream.str() << endl;
        losses++;
        break;
      }

      if(maxed)
      {
        bet = STARTING_BET;
        betstream << " *Lost maxed-out bet*";
        maxed = false;
      }
      else
        bet = bet * 2;           //double bet

      if(bet > TABLE_MAX)
      {
        bet = TABLE_MAX;
        betstream << " *Couldnt double our bet*";
        maxed = true;  //to show that we have already made one bet in the current stream at the max value, should restart
      }

      if(bet >= current_cash)
      {
        bet = current_cash;
        maxed = true;  //to show that we have already made one bet in the current stream at the max value, should restart
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

      cout << betstream.str();
      betstream.str("");    //clears out the betstream
      segment_rolls = 0;
      bet = STARTING_BET;
      maxed = false;        //we won this stream
      just_won = true;      //we won this stream
    }
  }

  int winnings = current_cash - STARTING_CASH;

  if(current_cash >= goal_cash)
  {
    cout << endl << "WINNER" << endl;
    cout << "It took you " << total_rolls << " rolls ";
    cout << "to make " << winnings;
    cout << ", and there were " << losses << " losses" << endl;
    return;
  }
  if(current_cash == 0)
  {
    cout << endl << "LOSER" << endl;
    cout << "You are broke, ";
    cout << "it happened in " << total_rolls << " rolls";
    cout << ", and there were " << losses << " losses" << endl;
    return;
  }
}
