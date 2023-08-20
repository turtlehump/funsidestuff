#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <unistd.h>
#include <string>
#include "card.h"
#include "deck.h"
#include "player.h"
#include "bigint.h"

using namespace std;

const int JACKPOT_MULTIPLIER      = 10;
const int MEGA_JACKPOT_MULTIPLIER = 50;

class Table
{
  public:
    Table();
    ~Table();

    void  play_round(Player* player);
    void  get_full_odds_breakdown();  //change the deck in the deck class (deck.cpp)

  private:

    void  clear_screen() {for(int i = 0; i < 70; i++) cout << endl;}
    void  print_tower();

    //  vvvvvv Playing Game vvvvvv

    void  delete_cards_in_tower();

    int   get_bet(Player* player, int base_bet);
    void    announce_bet(int round_multiplier);

    void  deal(); 

    int   play_row(int current_row);  //returns payout value
    void    announce_row(int current_row);
    bool    conflict(Card* lower, Card* upper);
    bool    handle_conflicts(int current_row);  //called only when there is a conflict
    void      announce_handling_conflict();
    int     evaluate_winnings(int current_row);

    bool  continue_or_payout(int win_payment);

    void  display_winnings(int win_payment, int current_row);
    void    announce_win();
    void      reveal_all_cards();
    void    announce_loss();

    void  get_odds(long unsigned int row,
                   long unsigned int spot_in_row,
                   CardFace          cardface,
                   long unsigned int first_conflict_row,
                   long unsigned int first_conflict_index,
                   long unsigned int second_conflict_row);
    bool    handle_conflict_for_odds(long unsigned int first_conflict_row,
                                     long unsigned int first_conflict_index);

    vector<vector<Card*>> m_tower
    {
                               {NULL},                     //1 in row 1
                            {NULL, NULL},                  //2 in row 2
                         {NULL, NULL, NULL},               //3 in row 3
                      {NULL, NULL, NULL, NULL},            //4 in row 4
                   {NULL, NULL, NULL, NULL, NULL},         //5 in row 5
                {NULL, NULL, NULL, NULL, NULL, NULL},      //6 in row 6
             {NULL, NULL, NULL, NULL, NULL, NULL, NULL}    //7 in row 7
    };

    long unsigned int m_TOWER_SIZE = 0;

    Card* m_savior_card;

    Deck* m_deck;

    BigInt* m_total_possibilities = NULL;

    vector<BigInt*> m_winning_sum_by_row;
    vector<BigInt*> m_max_row_before_loss;
    BigInt*         m_possible_jackpots;
    BigInt*         m_jackpots_payouts_sum;
    BigInt*         m_possible_mega_jackpots;
    BigInt*         m_mega_jackpots_payouts_sum;

};
#endif
