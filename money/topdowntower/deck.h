#ifndef DECK_H
#define DECK_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "card.h"
using namespace std;

class Deck
{
    public:
        Deck();
        ~Deck();

        void          print();

        vector<Card*> shuffle();

        Card*         deal_top_card();

    private:
        vector<Card*>  m_deck;
        int            m_size = 0;
};
#endif
