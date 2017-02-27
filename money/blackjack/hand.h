#ifndef HAND_H
#define HAND_H

#include "card.h"
#include <iostream>
#include <vector>
using namespace std;

class Hand
{
    public:
        Hand(int bet) {m_bet = bet; m_bust = false;}
        ~Hand();

        int     hit(Card* new_card);
        int     stand();

        //you can only split as your first action
        //you are guaranteed to have 2 cards
        Card*   split();

        void    print();
        void    dealer_print(bool is_playing, bool final_print);

        int     soft_value();
        int     hard_value();

        int     get_bet() {return m_bet;}
        int     determine_payout(int dealers_hand_value);

    private:
        int           m_bet;
        bool          m_bust;
        vector<Card*> m_cards;
};
#endif

