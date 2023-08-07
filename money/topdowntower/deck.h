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

        //for playing the game
        Card*         deal_top_card();

        //for calculating the odds
        void          reveal_all_for_odds();
        Card*         deal_card_by_face(CardFace face);
        void          put_card_back(Card* card);
        int           get_card_count(CardFace cardface) { return m_cards_by_faces[cardface].size(); }

    private:
        //data structure for playing the game
        vector<Card*>  m_deck;

        //data structure for calculating the odds
        vector<vector<Card*>>  m_cards_by_faces;
        int                    m_size = 0;

};
#endif
