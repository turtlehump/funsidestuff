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
        int         play_blackjack(Deck* deck);
        int         dealer_play_blackjack(Deck* deck);

        bool        is_playing();
        void        announce_playing(); 

    private:
        vector<Card*> m_cards;
        string        m_name;
        int           m_money;
        bool          m_cur_playing;
        int           soft_hand_value();
        int           hard_hand_value();
};
#endif

