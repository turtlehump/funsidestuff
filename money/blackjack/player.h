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
        Player(string name);
        Player(string name, double money);
        ~Player();

        string      get_name()               {return m_name;}

        bool        is_playing()             {return m_cur_playing;}
        void        start_playing()          {m_cur_playing = true;}
        void        stand()                  {m_cur_playing = false;}

        int         get_num_hands()          {return m_hands.size();}

        double      get_money_count()        {return m_money;}
        void        take_money(double money) {m_money -= money;}
        void        give_money(double money) {m_money += money;}

        bool        can_match_bet(double bet) {return (m_money > bet);}

        void        add_hand(Hand* new_hand);
        Hand*       get_next_hand(bool still_dealing_starting_cards);
        bool        all_hands_are_bust();
        void        reset_hands();

        void        set_insurance();
        double      get_insurance() {return m_insurance;}

        void        print();

        double      determine_payout(int dealers_hand_value);

    private:
        vector<Hand*> m_hands;
        unsigned int  m_next_hand;
        string        m_name;
        double        m_money;
        double        m_insurance;
        bool          m_cur_playing;
};
#endif
