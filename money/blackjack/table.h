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

        void      play_a_hand();

        void      set_hands_for_players();
        void      starting_deal();

        void      player_play(Player* player);
        int       hand_play(Hand* hand, string player_name, int hand_num);

        int       dealer_play();

        void      dealing_print();
        void      playing_print(string name, int hand_num);
        void      final_print(int dealer_hand_value);

        int             m_min_bet;
        int             m_max_bet;
        Deck*           m_deck;
        Dealer*         m_dealer;
        vector<Player*> m_players;
};
#endif

