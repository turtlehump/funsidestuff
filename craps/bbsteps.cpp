#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
  srand(time(NULL));
  const int STARTING_BET = 10;
  const int TABLE_MAX = 2000;
  int starting_cash = 5000, segmentRolls = 0, bet, roll, dice1, dice2, totalRolls = 0, losses = 0;
  bool maxed = false;
  bool just_won = true;
  int current_cash = starting_cash;
  int goalCash = starting_cash + 500;
  int cash_at_start_of_betting_seq;

  bet = STARTING_BET;
  //10, 20, 40,  80, 160, 320,  640, 1280, 2560,  5120, 10240   <- Bets
  //10, 30, 70, 150, 310, 630, 1270, 2550, 5110, 10230, 20470   <- Running total
  //20, 40, 80, 160, 320, 640, 1280, 2560, 5120, 10240, 20480   <- winnings

  ostringstream betstream;
  while(current_cash < goalCash && current_cash > 0)
  {
    if(just_won)
    {
      cash_at_start_of_betting_seq = current_cash;
    }

    current_cash -= bet;        //place your bet

    dice1 = (rand() % 6) + 1;
    dice2 = (rand() % 6) + 1;
    roll = dice1 + dice2;      //roll
    totalRolls++;

    if(bet == STARTING_BET) betstream << bet;        //record the bet
    else                    betstream << ", " << bet;

    segmentRolls++;          //havent won our 10 yet

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
        betstream << " *Lost maxed-out bet*, "; //add the comma and space in here because we reset the bet
        maxed = false;
      }
      else
      {
        bet = bet * 2;           //double bet
      }
      if(bet > TABLE_MAX)
      {
        bet = TABLE_MAX;
        betstream << " *Couldnt double our bet*";
        maxed = true;  //to show that we have already made one bet in the current stream at the max value, should restart
      }
      if(bet >= current_cash)
      {
        bet = current_cash;
        betstream << " MAKE IT OR BREAK IT! -> ";
        maxed = true;  //to show that we have already made one bet in the current stream at the max value, should restart
      }
      just_won = false;
    }
    else //win
    {
      //these displaying of winnings are correct if maxed == false
      betstream << "\t";
      int tmp;
      if(roll == 3 || roll == 4 || (roll >= 9 && roll <= 11))
      { 
        current_cash += bet * 2; //wins 1:1, double the bet and give it back, we already put the money down and took it out of current_cash
        tmp = current_cash - cash_at_start_of_betting_seq;
        if(tmp > 0)
          betstream << "+";
        else losses++;
        betstream << tmp << " in " << segmentRolls << endl;
      }
      else if(roll == 2)
      {
        current_cash += bet * 3; //wins 1:2, tripple the bet and give it back
        tmp = current_cash - cash_at_start_of_betting_seq;
        if(tmp > 0)
          betstream << "+";
        else losses++;
        betstream << tmp << " in " << segmentRolls << ", got double 1's\n";
      }
      else if(roll == 12)
      {
        current_cash += bet * 4; //wins 1:3, quadrouple the bet and give it back
        tmp = current_cash - cash_at_start_of_betting_seq;
        if(tmp > 0)
          betstream << "+";
        betstream << tmp << " in " <<  segmentRolls << ", got double 6's\n";
      }
      cout << betstream.str();
      betstream.str("");    //clears out the betstream
      segmentRolls = 0;
      bet = STARTING_BET;
      maxed = false;        //we won this stream
      just_won = true;      //we won this stream
    }
  }

  int winnings = current_cash - starting_cash;

  if(current_cash >= goalCash)
    cout << "It took you " << totalRolls << " rolls to make " << winnings << ", and there were " << losses << " losses\n";

  if(current_cash == 0)
    cout << "You are broke, it happened in " << totalRolls << " rolls, and there were " << losses << " losses\n";

  return 0;
}
