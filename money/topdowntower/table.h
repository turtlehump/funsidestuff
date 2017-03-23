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

        void  set_player();
        void  simulation();

    private:

        void  ask_for_bet();
        void  fill_tower();

        int   flip_and_evaluate_row(int row);
        int   joker_max_value(int row, int index);
        bool  ask_continue(int takehome_payment);

        void  print_tower();

        void  flip_all_cards();
        bool  ask_play_again();

        int   m_bet_multiplier;

        Card* m_row1[1];
        Card* m_row2[2];
        Card* m_row3[3];
        Card* m_row4[4];
        Card* m_row5[5];
        Card* m_row6[6];
        Card* m_row7[7];
        Card* m_savior_card;

        Deck* m_deck;
        Player* m_player;
};
#endif
