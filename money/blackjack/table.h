#ifndef TABLE_H
#define TABLE_H

#include "player.h"
#include "deck.h"
#include <iostream>
#include <vector>
#include <unistd.h>
using namespace std;

class Table
{
    public:
        Table() {m_deck = NULL; m_dealer = new Player("Dealer");}
        ~Table();

        void      set_deck();
        void      get_players();

        void      dealing_print();
        void      playing_print(Player* player);
        void      final_print();

        void      simulation();
        void      play_hand();
        void      play_blackjack(Player* player);

    private:
        void      starting_deal();

        Deck*           m_deck;
        Player*         m_dealer;
        vector<Player*> m_players;
};
#endif

