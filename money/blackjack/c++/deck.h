#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
using namespace std;

class Deck
{
    public:
        Deck(int num_cards);
        ~Deck();

        void          print();
        int           get_count()       {return m_count;}
        int           get_cards_dealt() {return m_cards_dealt;}

        vector<Card*> shuffle();
        void          cut();

        Card*         deal_top_card();

    private:
        vector<Card*>  m_deck;
        int            m_original_card_count;
        unsigned int   m_cut;
        int            m_count;
        int            m_cards_dealt;
};
#endif

