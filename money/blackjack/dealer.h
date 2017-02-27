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

        int       hit(Card* new_card);

        int       soft_hand_value() {return m_hand->soft_value();}
        int       hard_hand_value() {return m_hand->hard_value();}

        void      start_playing() {m_cur_playing = true;}
        void      done_playing()  {m_cur_playing = false;}
        bool      is_playing()    {return m_cur_playing;}

    private:
        Hand* m_hand;
        bool  m_cur_playing;
};
#endif

