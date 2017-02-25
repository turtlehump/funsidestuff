#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "deck.h"
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

        bool        is_playing() {return m_cur_playing;}

        string      get_name() {return m_name;}

        void        start_playing() {m_cur_playing = true;}
        void        stand() {m_cur_playing = false;}

        int         soft_hand_value();
        int         hard_hand_value();

    private:
        vector<Card*> m_cards;
        string        m_name;
        int           m_money;
        bool          m_cur_playing;
};
#endif

