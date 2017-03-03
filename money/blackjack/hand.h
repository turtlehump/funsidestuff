#ifndef HAND_H
#define HAND_H

#include "card.h"
#include "deck.h"
#include <iostream>
#include <vector>
#include <unistd.h>
using namespace std;

class Hand
{
    public:
        Hand(int bet)
        {m_bet = bet; m_bust = false; m_cur_playing = false; m_completed = false;}
        ~Hand();

        int     hit(Card* new_card);
        int     stand();
        bool    can_double_down()  {return (m_cards.size() < 3);}
        int     double_down(Card* new_card);
        //you can only split as your first action
        //you are guaranteed to have 2 cards
        bool    can_split();
        Hand*   split(Deck* deck);

        void    print();
        void    dealer_print(bool is_playing, bool final_print);

        int     value();
        int     soft_value();
        int     hard_value();

        int     determine_payout(int dealers_hand_value);


        int     get_bet()       {return m_bet;}
        bool    is_playing()    {return m_cur_playing;}
        bool    has_bust()      {return m_bust;}
        bool    is_completed()  {return m_completed;}
        void    start_playing() {m_cur_playing = true;}

    private:
        int           m_bet;
        bool          m_bust;
        bool          m_cur_playing;
        bool          m_completed;
        vector<Card*> m_cards;
};
#endif

