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
        Hand(double bet)
        {m_bet = bet; m_bust = false; m_cur_playing = false; m_completed = false;}
        ~Hand();

        int     hit(Card* new_card);
        int     stand();

        //you can only double down as your first action
        //you are guaranteed to have only 2 cards
        bool    can_double_down()  {return (m_cards.size() < 3);}
        int     double_down(Card* new_card);

        //you can only split as your first action
        //you are guaranteed to have only 2 cards
        bool    can_split();
        Hand*   split(Deck* deck);

        int     value();
        int     soft_value();
        int     hard_value();

        double  determine_payout(int dealers_hand_value);

        double  get_bet()       {return m_bet;}
        bool    is_playing()    {return m_cur_playing;}
        bool    has_bust()      {return m_bust;}
        bool    is_completed()  {return m_completed;}
        void    start_playing() {m_cur_playing = true;}

        bool    is_first_card_ace();
        bool    is_blackjack();

        void    print();
        void    dealer_print(bool is_playing, bool final_print);

    private:
        double        m_bet;
        bool          m_bust;
        bool          m_cur_playing;
        bool          m_completed;
        vector<Card*> m_cards;
};
#endif

