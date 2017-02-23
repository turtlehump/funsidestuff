#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Player
{
    public:
        Player(string name) {m_name = name; m_cur_playing = false;}
        ~Player();
        int         hit(Card* new_card);
        void        reset_hand();
        void        print();
        void        dealer_print();

    private:
        vector<Card*> m_cards;
        string        m_name;
        int           m_money;
        bool          m_cur_playing;
};
#endif

