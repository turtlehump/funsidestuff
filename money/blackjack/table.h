#ifndef TABLE_H
#define TABLE_H

#include "player.h"
#include "dealer.h"
#include "deck.h"
#include <iostream>
#include <vector>
#include <unistd.h>
using namespace std;

class Table
{
    public:
        Table()  {m_deck = NULL; m_dealer = new Dealer();}
        ~Table();

        void      setup_game();
        void      simulation();

    private:
        void      set_table_limits();
        void      set_deck();
        void      get_players();

        void      play_an_entire_hand(bool repeat_last_hand);

        void      set_hands_for_players(bool repeat_last_hand);
        void      starting_deal();

        void      player_play(Player* player);
        int       hand_play(Hand* hand, Player* player, int hand_num);

        bool      dealer_should_play();
        int       dealer_play();

        void      pay_winners(int dealer_hand_value);

        void      table_dealing_print();
        void      table_playing_print(string name, int hand_num);
        void      table_final_print(int dealer_hand_value);
        void      clear_screen_print();

        void      player_money_print();

        int             m_min_bet;
        int             m_max_bet;
        Deck*           m_deck;
        Dealer*         m_dealer;
        vector<Player*> m_players;
};
#endif

