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

        void      set_deck();
        void      get_players();
        void      simulation();

    private:
        void      play_a_hand();
        void      set_hands_for_players();
        void      starting_deal();
        void      player_play(Player* player);
        int       dealer_play();

        void      dealing_print();
        void      playing_print(string name);
        void      final_print();

        Deck*           m_deck;
        Dealer*         m_dealer;
        vector<Player*> m_players;
};
#endif

