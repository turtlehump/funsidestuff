#ifndef DEALER_H
#define DEALER_H

#include "card.h"
#include "hand.h"
#include <iostream>
#include <vector>
using namespace std;

class Dealer
{
    public:
        Dealer()  {m_cur_playing = false; m_hand = new Hand(0);}
        ~Dealer() {delete m_hand;}

        void      reset_hand();

        void      print(bool final_print);

        int       hit(Card* new_card) {return m_hand->hit(new_card);}

        int       soft_hand_value() {return m_hand->soft_value();}
        int       hard_hand_value() {return m_hand->hard_value();}

        void      start_playing() {m_cur_playing = true;}
        void      done_playing()  {m_cur_playing = false;}
        bool      is_playing()    {return m_cur_playing;}

        //this is called after the initial deal
        //gaurenteed to have 2 cards
        bool      should_offer_insurance() {return m_hand->is_first_card_ace();}
        bool      got_blackjack() {return m_hand->is_blackjack();}

    private:
        Hand* m_hand;
        bool  m_cur_playing;
};
#endif

