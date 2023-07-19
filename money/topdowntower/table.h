#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <unistd.h>
#include <string>
#include "card.h"
#include "deck.h"
#include "player.h"
using namespace std;

class Table
{
  public:
    Table();
    ~Table();

    void  play_round(Player* player);

  private:

    void  clear_screen() {for(int i = 0; i < 70; i++) cout << endl;}
    void  print_tower();

    int   get_bet(Player* player, int base_bet);
    void    announce_bet(int round_multiplier);

    void  deal(); 

    int   play_row(int current_row);  //returns payout value
    void    announce_row(int current_row);
    bool    conflict(Card* lower, Card* upper);
    int     handle_conflicts(int current_row); //called only when there is a conflict
    void      announce_handling_conflict();

    bool  continue_or_payout(int win_payment);

    void  display_winnings(int win_payment, int current_row);
    void    announce_win();
    void     reveal_all_cards();
    void    announce_loss();

    void  delete_cards_in_tower();

    vector<vector<Card*>> m_tower
    {
                               {NULL},                     //1 in row 1
                            {NULL, NULL},                  //2 in row 2
                         {NULL, NULL, NULL},               //3 in row 3
                      {NULL, NULL, NULL, NULL},            //4 in row 4
                   {NULL, NULL, NULL, NULL, NULL},         //5 in row 5
                {NULL, NULL, NULL, NULL, NULL, NULL},      //6 in row 6
              {NULL, NULL, NULL, NULL, NULL, NULL, NULL}   //7 in row 7
    };

    Card* m_savior_card;

    Deck* m_deck;
};
#endif
