#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "hand.h"
#include "deck.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Player
{
    public:
        Player(string name) {m_name = name; m_cur_playing = false; m_next_hand = 0;}
        ~Player();

        void        add_hand(Hand* new_hand) {m_hands.push_back(new_hand);}
        void        reset_hands();
        int         get_num_hands() {return m_hands.size();}
        Hand*       get_next_hand();

        void        print();

        bool        is_playing() {return m_cur_playing;}

        string      get_name() {return m_name;}

        void        start_playing() {m_cur_playing = true;}
        void        stand() {m_cur_playing = false;}

    private:
        vector<Hand*> m_hands;
        unsigned int  m_next_hand;
        string        m_name;
        int           m_money;
        bool          m_cur_playing;
};
#endif

