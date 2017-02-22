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
        Table() {m_num_players = 0; m_cur_player_index = 0; m_deck = NULL;}
        ~Table();
        bool      set_deck(Deck* deck);
        void      add_player(Player* new_player);
        Player*   get_next_player();

        void      starting_deal();

        void      print(bool final_print);

    private:
        Deck*           m_deck;
        vector<Player*> m_players;
        int             m_num_players;
        int             m_cur_player_index;
};
#endif

